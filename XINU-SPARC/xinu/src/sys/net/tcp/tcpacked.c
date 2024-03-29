/* tcpacked.c - tcpacked */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>

/*------------------------------------------------------------------------
 *  tcpacked - handle in-bound ACKs and do round trip estimates
 *------------------------------------------------------------------------
 */
int tcpacked(ptcb, pep)
     struct	tcb	*ptcb;
     struct	ep	*pep;
{
    struct	ip	*pip = (struct ip *)pep->ep_data;
    struct	tcp	*ptcp = (struct tcp *)pip->ip_data;
    int		acked, cacked;
    
    if (!(ptcp->tcp_code & TCPF_ACK))
	return SYSERR;
    
    acked = ptcp->tcp_ack - ptcb->tcb_suna;
    cacked = 0;
    if (acked <= 0)
	return 0;	/* duplicate ACK */
    
    if (SEQCMP(ptcp->tcp_ack, ptcb->tcb_snext) > 0)  /* ACK of unsent data! */
	if (ptcb->tcb_state == TCPS_SYNRCVD)	     
	    return tcpreset(pep);		     /* RFC-793 p.72 */
	else
	    return tcpackit(ptcb, pep);

    tcprtt(ptcb);
    
    ptcb->tcb_suna = ptcp->tcp_ack;
    if (acked && ptcb->tcb_code & TCPF_SYN) {
	acked--;
	cacked++;
	ptcb->tcb_code &= ~TCPF_SYN;
	ptcb->tcb_flags &= ~TCBF_FIRSTSEND;
    }
    
    if ((ptcb->tcb_code & TCPF_FIN) &&
	SEQCMP(ptcp->tcp_ack, ptcb->tcb_snext) == 0) {
	acked--;
	cacked++;
	ptcb->tcb_code &= ~TCPF_FIN;
	ptcb->tcb_flags &= ~TCBF_SNDFIN;
    }
    
    ptcb->tcb_sbstart = (ptcb->tcb_sbstart+acked) % ptcb->tcb_sbsize;
    ptcb->tcb_sbcount -= acked;
    if (acked)
	tcpwakeup(WRITERS,ptcb);
    tcpostate(ptcb, acked+cacked);
    return acked;
}
