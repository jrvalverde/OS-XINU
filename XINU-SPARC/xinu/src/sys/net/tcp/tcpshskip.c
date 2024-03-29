/* tcpshskip.c - tcpshskip */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>
#include <q.h>

/*------------------------------------------------------------------------
 *  tcpshskip - skip urgent data holes in send buffer
 *------------------------------------------------------------------------
 */
int tcpshskip(ptcb, datalen, poff)
struct	tcb	*ptcb;
int	datalen;
int	*poff;
{
	struct 	uqe	*puqe;
	tcpseq		lseq;
	int		resid = 0;

	if (ptcb->tcb_suhq == EMPTY)
		return datalen;
	puqe = (struct uqe *)deq(ptcb->tcb_suhq);
	while (puqe) {
		lseq = ptcb->tcb_snext + datalen - 1;
		if (SEQCMP(lseq, puqe->uq_seq) > 0)
			resid = lseq - puqe->uq_seq;
		/* chop off at urgent boundary, but save the hole...	*/
		if (resid < datalen) {
			datalen -= resid;
			if (enq(ptcb->tcb_suhq, puqe,
					SUHK(ptcb, puqe->uq_seq)) < 0)
				uqfree(puqe);	/* shouldn't happen */
			return datalen;
		}
		/*
		 * else, we're skipping a hole now adjust the beginning
		 * of the segment.
		 */
		lseq = puqe->uq_seq + puqe->uq_len;
		resid = lseq - ptcb->tcb_snext;
		if (resid > 0) {
			/* overlap... */
			*poff += resid;
			if (*poff > ptcb->tcb_sbsize)
				*poff -= ptcb->tcb_sbsize;
			datalen -= resid;
			if (datalen < 0)
				datalen = 0;
		}
		uqfree(puqe);
		puqe = (struct uqe *)deq(ptcb->tcb_suhq);
	}
	/* puqe == 0 (No more holes left) */
	freeq(ptcb->tcb_suhq);
	ptcb->tcb_suhq = EMPTY;
	return datalen;
}
