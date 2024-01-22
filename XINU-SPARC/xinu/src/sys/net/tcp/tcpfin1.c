/* tcpfin1.c - tcpfin1 */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>

/*------------------------------------------------------------------------
 *  tcpfin1 -  do FIN_WAIT_1 state input processing
 *------------------------------------------------------------------------
 */
int tcpfin1(ptcb, pep)
struct	tcb	*ptcb;
struct	ep	*pep;
{
	struct	ip	*pip	= (struct ip *)pep->ep_data;
	struct	tcp	*ptcp	= (struct tcp *)pip->ip_data;

	if (ptcp->tcp_code & TCPF_RST)
		return tcpabort(ptcb, TCPE_RESET);
	if (ptcp->tcp_code & TCPF_SYN) {
		tcpreset(pep);
		return tcpabort(ptcb, TCPE_RESET);
	}
	if (tcpacked(ptcb, pep) == SYSERR)
		return OK;
	tcpdata(ptcb, pep);
	if (ptcb->tcb_rbcount && (ptcb->tcb_flags & TCBF_NOREAD)) {
#ifdef DEBUG
		kprintf("FIN1(%d): resetting connection\n",
			ptcb->tcb_dvnum);
#endif
		tcpreset(pep);		/* no new data allowed	*/
		return tcpabort(ptcb, TCPE_RESET);
	}

	if (ptcb->tcb_flags & TCBF_RDONE) {
		if (ptcb->tcb_code & TCPF_FIN)		/* FIN not ACKed*/
			ptcb->tcb_state = TCPS_CLOSING;
		else {
			ptcb->tcb_state = TCPS_TIMEWAIT;
			if (!ptcb->tcb_nbio) 
			    signal(ptcb->tcb_ocsem);	/* wake closer	*/
			tcpwait(ptcb);
		}
	} else if ((ptcb->tcb_code & TCPF_FIN) == 0) {
		ptcb->tcb_state = TCPS_FINWAIT2;
		if (!ptcb->tcb_nbio)
		    signal(ptcb->tcb_ocsem); /* wake closer	*/
	}
	return OK;
}
