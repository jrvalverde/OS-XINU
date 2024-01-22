/* tcplisten.c - tcplisten */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>

/*------------------------------------------------------------------------
 *  tcplisten -  do LISTEN state processing
 *------------------------------------------------------------------------
 */
int tcplisten(ptcb, pep)
struct	tcb	*ptcb;
struct	ep	*pep;
{
	struct	tcb	*newptcb, *tcballoc();
	struct	ip	*pip = (struct ip *)pep->ep_data;
	struct	tcp	*ptcp = (struct tcp *)pip->ip_data;

#ifdef DEBUG
	kprintf("tcplisten: called...\n");
#endif
	if (ptcp->tcp_code & TCPF_RST)
		return OK;		/* "parent" TCB still in LISTEN */
	if ((ptcp->tcp_code & TCPF_ACK) ||
	    (ptcp->tcp_code & TCPF_SYN) == 0)
		return tcpreset(pep);
	newptcb = tcballoc();
	if (newptcb == (struct tcb *)SYSERR || tcpsync(newptcb) == SYSERR)
		return SYSERR;
	newptcb->tcb_state = TCPS_SYNRCVD;
	newptcb->tcb_ostate = TCPO_IDLE;
	newptcb->tcb_error = 0;
	newptcb->tcb_pptcb = ptcb;			/* for ACCEPT	*/

	IP_COPYADDR(newptcb->tcb_rip, pip->ip_src);
	newptcb->tcb_rport = ptcp->tcp_sport;
	IP_COPYADDR(newptcb->tcb_lip, pip->ip_dst);
	newptcb->tcb_lport = ptcp->tcp_dport;

	tcpwinit(ptcb, newptcb, pep);	/* initialize window data	*/

	newptcb->tcb_finseq = newptcb->tcb_pushseq = 0;
	newptcb->tcb_flags = TCBF_NEEDOUT;
	TcpPassiveOpens++;
	ptcp->tcp_code &= ~TCPF_FIN;	/* don't process FINs in LISTEN */
	tcpdata(newptcb, pep);
	signal(newptcb->tcb_mutex);
	return OK;
}
