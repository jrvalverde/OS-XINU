/* tcprcvurg.c - tcprcvurg */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>
#include <q.h>

struct	uqe	*uqalloc();

/*------------------------------------------------------------------------
 *  tcprcvurg -  handle TCP urgent data
 *------------------------------------------------------------------------
 */
int tcprcvurg(ptcb, pep)
struct	tcb	*ptcb;
struct	ep	*pep;
{
	struct	ip	*pip = (struct ip *)pep->ep_data;
	struct	tcp	*ptcp = (struct tcp *)pip->ip_data;
	struct	uqe	*puqe;
	int		datalen, offset;

	datalen = pip->ip_len - IP_HLEN(pip) - TCP_HLEN(ptcp);
	/*
	 * Berkeley machines treat the urgent pointer as "last+1", but
	 * RFC 1122 says just "last." Defining "BSDURG" causes Berkeley
	 * semantics.
	 */
#ifdef	BSDURG
	ptcp->tcp_urgptr -= 1;
#endif	/* BSDURG */
	if (ptcp->tcp_urgptr >= datalen || ptcp->tcp_urgptr >= TCPMAXURG)
		ptcp->tcp_urgptr = datalen-1;	/* quietly fix it	*/
	puqe = uqalloc();
	if (puqe == (struct uqe *)SYSERR)
		return SYSERR;		/* out of buffer space!!!	*/
	puqe->uq_seq = ptcp->tcp_seq;
	puqe->uq_len = ptcp->tcp_urgptr + 1;
	puqe->uq_data = (char *)getbuf(Net.netpool);
	if (puqe->uq_data == (char *)SYSERR) {
		puqe->uq_data = 0;
		uqfree(puqe);
		return SYSERR;
	}
	blkcopy(puqe->uq_data, &pip->ip_data[TCP_HLEN(ptcp)],
		puqe->uq_len);
	if (ptcb->tcb_rudq < 0) {
		ptcb->tcb_rudseq = ptcb->tcb_rnext;
		ptcb->tcb_rudq = newq(TCPUQLEN, QF_WAIT);
		if (ptcb->tcb_rudq < 0) {
			uqfree(puqe);
			return SYSERR;	/* treat it like normal data	*/
		}
	}
	if (datalen > puqe->uq_len) {
		/* some non-urgent data left, edit the packet		*/

		ptcp->tcp_seq += puqe->uq_len;
		offset = TCP_HLEN(ptcp) + puqe->uq_len;
		blkcopy(&pip->ip_data[TCP_HLEN(ptcp)],
			&pip->ip_data[offset], datalen-puqe->uq_len);
		ptcp->tcp_urgptr = 0;
		pip->ip_len -= puqe->uq_len;
		/* checksums already checked, so no need to redo them	*/
	}
	if (enq(ptcb->tcb_rudq,puqe,ptcb->tcb_rudseq-puqe->uq_seq) < 0) {
		uqfree(puqe);
		return SYSERR;
	}
	return OK;
}
