/* tcpread.c - tcpread */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>
#include <nettcp.h>

/*#define DEBUG*/
/*#define PRINTERRORS*/


/*------------------------------------------------------------------------
 *  tcpread  -  read one buffer from a TCP pseudo-device
 *------------------------------------------------------------------------
 */
tcpread(pdev, pch, len)
     struct	devsw	*pdev;
     char		*pch;
     int		len;
{
    struct	tcb	*ptcb = (struct tcb *)pdev->dvioblk;
    int		state = ptcb->tcb_state;
    int		cc;

#ifdef DEBUG
    kprintf("tcpread(dev:%d, buf, len:%d, state=%d) called\n",
	    pdev->dvnum, len, state);
#endif
    
    if ((state != TCPS_ESTABLISHED) && (state != TCPS_CLOSEWAIT)) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("tcpread(%d): state = %d, returning SYSERR\n", 
		pdev->dvnum, ptcb->tcb_state);
#endif
	return SYSERR;
    }
    
  retry:
#ifdef XSELECT
    if (!ptcb->tcb_nbio)	/* non-blocking I/O */
#endif
	wait(ptcb->tcb_rsema);
    wait(ptcb->tcb_mutex);
    
    if (ptcb->tcb_state == TCPS_FREE) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("tcpread: state = FREE, returning SYSERR\n");
#endif
	return SYSERR;			/* gone		*/
    }
    
    if (ptcb->tcb_error) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("tcpread: error = %d, rudq = %d\n", 
		ptcb->tcb_error, ptcb->tcb_rudq);
#endif 
	tcpwakeup(READERS, ptcb);	/* propagate it */
	signal(ptcb->tcb_mutex);
	return ptcb->tcb_error;
    }
    
    if (ptcb->tcb_rudq < 0) {
	if (proctab[currpid].ptcpumode) {
	    proctab[currpid].ptcpumode = FALSE;
	    cc = TCPE_NORMALMODE;	/* normal mode */
	} else if (len > ptcb->tcb_rbcount) {
	    /* want more than available */
	    if (ptcb->tcb_nbio) {
		if (ptcb->tcb_rbcount == 0) {
		    if ((ptcb->tcb_flags & TCBF_RDONE) == 0) {
			/* more to read, but not yet */
			cc = BLOCKERR;
		    } else {
			/* connection closed, return EOF */
			cc = EOF;
		    }
#ifdef DEBUG
		    kprintf("tcpread: returning %d\n", cc);
#endif
		    signal(ptcb->tcb_mutex);
		    return(cc);
		}
		
		/* readadjust length to available */
		len = ptcb->tcb_rbcount;
#ifdef DEBUG
		kprintf("tcpread: in nbio, len == %d\n", len);
#endif
	    } else {
		/* NOT nbio */
		if (ptcb->tcb_flags & TCBF_BUFFER &&
		    (ptcb->tcb_flags & TCBF_PUSH|TCBF_RDONE) == 0) {
		    signal(ptcb->tcb_mutex);
		    goto retry;
		}
	    }
	    cc = tcpgetdata(ptcb, pch, len);
	} else {
	    /* plenty there, just take it */
	    cc = tcpgetdata(ptcb, pch, len);
	}
    } else {
	/* do urgent data */
	if (proctab[currpid].ptcpumode)
	    cc = tcprurg(ptcb, pch, len);
	else {
	    proctab[currpid].ptcpumode = TRUE;
	    cc = TCPE_URGENTMODE;	/* tell to go in urgent mode */
	}
    }
    if (cc == 0 && (ptcb->tcb_flags & TCBF_RDONE) == 0) {
#ifdef XSELECT
	/* no chars found, but connection still open */
	if (ptcb->tcb_nbio) {
	    cc = BLOCKERR;
	} else {
#endif (XSELECT)
	    /*
	     * Have to block after all. Holes can cause
	     * rbcount != 0, but no real data available.
	     */
	    signal(ptcb->tcb_mutex);
	    goto retry;
#ifdef XSELECT
	}
#endif
    }
    tcpwakeup(READERS, ptcb);
    signal(ptcb->tcb_mutex);
#ifdef DEBUG
    kprintf("tcpread: returning cc = %d\n", cc);
#endif
    return cc;
}
