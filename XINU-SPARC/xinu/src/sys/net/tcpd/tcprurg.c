/* tcprurg.c - tcprurg */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>
#include <nettcp.h>
#include <q.h>

/*------------------------------------------------------------------------
 *  tcprurg  -  read pending urgent data
 *------------------------------------------------------------------------
 */
int tcprurg(ptcb, pch, len)
struct	tcb	*ptcb;
char		*pch;
int		len;
{
	struct	uqe	*puqe;
	int		cc, uc;
	tcpseq		cseq, eseq;

	puqe = (struct uqe *)deq(ptcb->tcb_rudq);
	cseq = puqe->uq_seq;
	for (cc=uc=0; cc < len && puqe;) {
		*pch++ = puqe->uq_data[uc++];
		++cc;
		if (uc >= puqe->uq_len) {
			while (puqe) {
				eseq = puqe->uq_seq + puqe->uq_len - 1;
				if (SEQCMP(cseq, eseq) < 0)
					break;
				tcpaddhole(ptcb, puqe);
				puqe = (struct uqe *)deq(ptcb->tcb_rudq);
			}
			if (puqe == 0)
				break;
			if (SEQCMP(cseq, puqe->uq_seq) < 0) {
				cseq = puqe->uq_seq;
				uc = 0;
			} else
				uc = cseq - puqe->uq_seq;
		} else
			cseq++;
	}
	if (puqe == 0) {
		freeq(ptcb->tcb_rudq);
		ptcb->tcb_rudq = EMPTY;
		return cc;
	}
	if (uc) {
		/* we need to adjust puqe */

		puqe->uq_seq = cseq;
		puqe->uq_len -= uc;
		blkcopy(puqe->uq_data, &puqe->uq_data[uc], puqe->uq_len);
	}
	if (enq(ptcb->tcb_rudq, puqe, RUDK(ptcb, puqe->uq_seq)) < 0)
		uqfree(puqe);	/* shouldn't happen */
	return cc;
}
