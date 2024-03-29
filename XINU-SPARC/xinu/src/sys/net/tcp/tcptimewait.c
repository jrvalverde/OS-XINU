/* tcptimewait.c - tcptimewait */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>

/*------------------------------------------------------------------------
 *  tcptimewait -  do TIME_WAIT state input processing
 *------------------------------------------------------------------------
 */
int tcptimewait(ptcb, pep)
     struct	tcb	*ptcb;
     struct	ep	*pep;
{
    struct	ip	*pip = (struct ip *)pep->ep_data;
    struct	tcp	*ptcp = (struct tcp *)pip->ip_data;
    
    if (ptcp->tcp_code & TCPF_RST)
	return tcbdealloc(ptcb);
    
    if (ptcp->tcp_code & TCPF_SYN) {
	tcpreset(pep);
	return tcbdealloc(ptcb);
    }
    
    tcpacked(ptcb, pep);
    tcpdata(ptcb, pep);		/* just ACK any packets */
    tcpwait(ptcb);		/* restart timer 	*/
    return OK;
}
