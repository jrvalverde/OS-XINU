/* tcpinp.c - tcpinp */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>

#define PRINTERRORS
/*#define DEBUG_RESET*/
/*#define DEBUG*/

/*------------------------------------------------------------------------
 *  tcpinp  -  handle TCP segment coming in from IP
 *------------------------------------------------------------------------
 */
PROCESS tcpinp()
{
    struct	ep	*pep;
    struct	ip	*pip;
    struct	tcp	*ptcp;
    struct	tcb	*ptcb;
    
    tcps_iport = pcreate(TCPQLEN);
    signal(Net.sema);
    
    while (TRUE) {
	pep = (struct ep *)preceive(tcps_iport);
	if (pep == (struct ep *)SYSERR)
	    break;
	pip = (struct ip *)pep->ep_data;
	if (tcpcksum(pip)) {
#ifdef PRINTERRORS
	    kprintf("tcpin: dropping packet with bad checksum\n");
#endif
	    freebuf(pep);
	    continue;
	}
	
	ptcp = (struct tcp *)pip->ip_data;
	tcpnet2h(ptcp); /* convert all fields to host order */
	ptcb = (struct tcb *) tcpdemux(pep);	/* search for a TCB */
	if (ptcb == 0) {			/* no TCB exists */
#ifdef DEBUG_RESET
	    kprintf("tcpin: resetting TCP connection\n");
	    ipdump(pep);
#endif
	    tcpreset(pep);		/* generate a RESET segment */
	    freebuf(pep);
	    continue;
	}
	if (!tcpok(ptcb, pep))		/* check if segment is in range */
	    tcpackit(ptcb, pep);
	else {
	    tcpopts(ptcb, pep);
	    tcpswitch[ptcb->tcb_state](ptcb, pep);	/* state transition */
	}
	
	if (ptcb->tcb_state != TCPS_FREE)
	    signal(ptcb->tcb_mutex);
	freebuf(pep);
    }
}

int tcps_oport, tcps_iport, tcps_lqsize, tcps_tmutex;
