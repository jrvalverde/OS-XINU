/* tcpsndlen.c - tcpsndlen */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>
#include <q.h>

struct	uqe	*uqalloc();

/*------------------------------------------------------------------------
 *  tcpsndlen - compute the packet length and offset in sndbuf
 *------------------------------------------------------------------------
 */
int tcpsndlen(ptcb, rexmt, poff)
     struct	tcb	*ptcb;
     Bool	rexmt;
     int	*poff;
{
    struct 	uqe	*puqe, *puqe2;
    int		datalen;
    
    if (rexmt || ptcb->tcb_sudq == EMPTY) {
	if (rexmt || (ptcb->tcb_code & TCPF_SYN))
	    *poff = 0;
	else
	    *poff = ptcb->tcb_snext - ptcb->tcb_suna;
	datalen = ptcb->tcb_sbcount - *poff;
	/* remove urgent data holes */
	if (!rexmt) {
	    datalen = tcpshskip(ptcb, datalen, poff);
	    datalen = min(datalen, ptcb->tcb_swindow);
	}
	return min(datalen, ptcb->tcb_smss);
    }
    /* else, URGENT data */
    
    puqe = (struct uqe *)deq(ptcb->tcb_sudq);
    *poff = ptcb->tcb_sbstart + puqe->uq_seq - ptcb->tcb_suna;
    if (*poff > ptcb->tcb_sbsize)
	*poff -= ptcb->tcb_sbsize;
    datalen = puqe->uq_len;
    if (datalen > ptcb->tcb_smss) {
	datalen = ptcb->tcb_smss;
	puqe2 = uqalloc();
	if (puqe2 == (struct uqe *)SYSERR) {
	    uqfree(puqe);		/* bail out and		*/
	    return ptcb->tcb_smss;	/* try as normal data	*/
	}
	puqe2->uq_seq = puqe->uq_seq;
	puqe2->uq_len = datalen;
	
	/* put back what we can't use */
	puqe->uq_seq += datalen;
	puqe->uq_len -= datalen;
    } else {
	puqe2 = puqe;
	puqe = (struct uqe *)deq(ptcb->tcb_sudq);
    }
    if (puqe == 0) {
	freeq(ptcb->tcb_sudq);
	ptcb->tcb_sudq = -1;
    } else if (enq(ptcb->tcb_sudq, puqe, SUDK(ptcb, puqe->uq_seq)) < 0)
	uqfree(puqe);	/* shouldn't happen */
    if (ptcb->tcb_suhq == EMPTY) {
	ptcb->tcb_suhq = newq(TCPUQLEN, QF_WAIT);
	ptcb->tcb_suhseq = puqe2->uq_seq;
    }
    if (enq(ptcb->tcb_suhq, puqe2, SUHK(ptcb, puqe2->uq_seq)) < 0)
	uqfree(puqe2);
    return datalen;
}
