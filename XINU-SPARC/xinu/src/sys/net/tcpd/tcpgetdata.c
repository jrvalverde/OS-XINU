/* tcpgetdata.c - tcpgetdata */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>

#define USEFASTCOPY

/*------------------------------------------------------------------------
 *  tcpgetdata  -  copy data from a TCP receive buffer to a user buffer
 *------------------------------------------------------------------------
 */
int tcpgetdata(ptcb, pch, len)
struct	tcb	*ptcb;
char		*pch;
int		len;
{
	struct	uqe	*puqe, *tcprhskip();
	tcpseq		seq;
	int		cc;

	if (ptcb->tcb_ruhq >= 0)
		puqe = (struct uqe *)deq(ptcb->tcb_ruhq);
	else
		puqe = 0;
	seq = ptcb->tcb_rnext - ptcb->tcb_rbcount; /* start sequence	*/
#ifdef USEFASTCOPY
	if (!puqe) {
		cc = 0;
		len = min(len,ptcb->tcb_rbcount);
		while (cc < len) {
			int ccblk;

			ccblk = min((len-cc),
				    (ptcb->tcb_rbsize - ptcb->tcb_rbstart));
		
			blkcopy(pch,
				&(ptcb->tcb_rcvbuf[ptcb->tcb_rbstart]),
				ccblk);
			ptcb->tcb_rbcount -= ccblk;
			ptcb->tcb_rbstart += ccblk;
			if (ptcb->tcb_rbstart >= ptcb->tcb_rbsize)
			    ptcb->tcb_rbstart = 0;
			pch += ccblk;
			cc += ccblk;
		}
	} else 
#endif
	for (cc=0; ptcb->tcb_rbcount && cc < len;) {
		/* see if we're at an urgent data hole */
		if (puqe && SEQCMP(puqe->uq_seq, seq) <= 0) {
			puqe = tcprhskip(ptcb, puqe, seq);
			continue;
		}
		/* ...now normal data processing */

		*pch++ = ptcb->tcb_rcvbuf[ptcb->tcb_rbstart];
		--ptcb->tcb_rbcount;
		if (++ptcb->tcb_rbstart >= ptcb->tcb_rbsize)
			ptcb->tcb_rbstart = 0;
		++cc;
	}
	if (puqe)
		if (enq(ptcb->tcb_ruhq,puqe,RUHK(ptcb,puqe->uq_seq)) < 0)
			uqfree(puqe);	/* shouldn't happen... */
	if (ptcb->tcb_rbcount == 0)
		ptcb->tcb_flags &= ~TCBF_PUSH;
	/*
	 * open the receive window, if it's closed and we've made
	 * enough space to fit a segment.
	 */
#ifdef SJCDEBUG
	rec_tcp_seg(0, 1, "TCPGETDATA: tcb_cwin:%08x  tcb_rnext:%08x",
		    ptcb->tcb_cwin, ptcb->tcb_rnext);
	rec_tcp_seg(0, 1, "TCPGETDATA: SEQCMP(): %d",
		    SEQCMP(ptcb->tcb_cwin, ptcb->tcb_rnext));
#endif
#ifdef ORIG
	/* this is dls's version, behaves badly */
        if (SEQCMP(ptcb->tcb_cwin, ptcb->tcb_rnext) <= 0 &&
            tcprwindow(ptcb)) {
#else
	/* this is Shawn's version, it causes fewer delays */
        if (SEQCMP(ptcb->tcb_cwin, ptcb->tcb_rnext) < ptcb->tcb_rmss &&
            (tcprwindow(ptcb) > ptcb->tcb_rmss)) {
#endif
		ptcb->tcb_flags |= TCBF_NEEDOUT;
#ifdef SJCDEBUG
		rec_tcp_seg(0, 1, "TCPGETDATA, calling tcpkick()");
#endif
		tcpkick(ptcb);
	}
	return cc;
}
