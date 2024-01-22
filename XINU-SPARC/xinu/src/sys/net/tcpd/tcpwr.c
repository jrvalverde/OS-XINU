/* tcpwr.c - tcpwr */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>
#include <nettcp.h>

#define PRINTERRORS
/*#define DEBUG*/
#define USEFASTCOPY

/*------------------------------------------------------------------------
 *  tcpwr  -  write urgent and normal data to TCP buffers
 *------------------------------------------------------------------------
 */
int tcpwr(pdev, pch, len, isurg)
     struct	devsw	*pdev;
     char		*pch;
     int		len;
     Bool		isurg;
{
    struct	tcb	*ptcb = (struct tcb *)pdev->dvioblk;
    int		state = ptcb->tcb_state;
    int		sboff, tocopy;
    
    if (state != TCPS_ESTABLISHED && state != TCPS_CLOSEWAIT) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("tcpwr(%d): state (%d) != TCPS_ESTABLISHED, SYSERR\n",
		pdev->dvnum, ptcb->tcb_state);
#endif
	return SYSERR;
    }
    
#ifdef DEBUG
    kprintf("tcpwr(dev:%d, buf, len:%d) called\n", pdev->dvnum, len);
#endif
#ifdef XSELECT
    if (ptcb->tcb_nbio) {
#ifdef DEBUG
	kprintf("tcpwrite: in nbio\n");
#endif
	wait(ptcb->tcb_mutex);
	if (ptcb->tcb_error) {
	    tcpwakeup(WRITERS, ptcb);	/* propagate it */
	    signal(ptcb->tcb_mutex);
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("tcpwr: error found (%d)\n", ptcb->tcb_error);
#endif
	    return ptcb->tcb_error;
	}
	tocopy = min(len, ptcb->tcb_sbsize - ptcb->tcb_sbcount);
#ifdef DEBUG
	kprintf("tcpwrite: tocopy == %d\n", tocopy);
#endif
	if (tocopy <= 0) {
#ifdef DEBUG
	    kprintf("tcpwr: BLOCKERR, tocopy is %d\n", tocopy);
#endif
	    signal(ptcb->tcb_mutex);
	    return(BLOCKERR);
	}
#ifdef DEBUG
	if (tocopy < len)
	    kprintf("tcpwr: BLOCKERR, wanted %d, only %d left\n",
		    len, tocopy);
#endif
	len = tocopy;
    } else {
	tocopy = tcpgetspace(ptcb, len);
#if defined(DEBUG) || defined(PRINTERRORS)
	if (tocopy <= 0)
	    kprintf("tcpwr: tocopy is %d (at 2)\n", tocopy);
#endif
	if (tocopy <= 0)
	    return tocopy;
    }
#else XSELECT
    tocopy = tcpgetspace(ptcb, len);	/* acquires tcb_mutex	*/
    if (tocopy <= 0) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("tcpwr: NOT XSELECT, tocopy is %d\n", tocopy);
#endif
	return tocopy;
    }
#endif XSELECT
    sboff = (ptcb->tcb_sbstart+ptcb->tcb_sbcount) % ptcb->tcb_sbsize;
    if (isurg)
	len = tcpwurg(ptcb, sboff, len);
#ifdef USEFASTCOPY
    while (tocopy > 0) {
	int cc;
	
	cc = min(tocopy,(ptcb->tcb_sbsize - sboff));
	
	blkcopy(&(ptcb->tcb_sndbuf[sboff]),pch,cc);
	ptcb->tcb_sbcount += cc;
	tocopy -= cc;
	pch += cc;
	sboff = 0;
    }
#else
    while (tocopy--) {
	ptcb->tcb_sndbuf[sboff] = *pch++;
	++ptcb->tcb_sbcount;
	if (++sboff >= ptcb->tcb_sbsize)
	    sboff = 0;
    }
#endif
    ptcb->tcb_flags |= TCBF_NEEDOUT;
    tcpwakeup(WRITERS, ptcb);
    signal(ptcb->tcb_mutex);
    
    if (isurg || ptcb->tcb_snext == ptcb->tcb_suna)
	tcpkick(ptcb);
    return len;
}
