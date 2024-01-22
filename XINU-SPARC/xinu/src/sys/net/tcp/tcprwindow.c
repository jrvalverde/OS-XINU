/* tcprwindow.c - tcprwindow */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>

/*------------------------------------------------------------------------
 *  tcprwindow - do receive window processing for a TCB
 *------------------------------------------------------------------------
 */
int tcprwindow(ptcb)
     struct	tcb	*ptcb;
{
    int	window;
    
    window = ptcb->tcb_rbsize - ptcb->tcb_rbcount;
    
    if (ptcb->tcb_state < TCPS_ESTABLISHED)
	return window;
    /*
     *	Receiver-Side Silly Window Syndrome Avoidance:
     *  Never shrink an already-advertised window, but wait for at
     *  least 1/4 receiver buffer and 1 max-sized segment before
     *  opening a zero window.
     */
    if (window*4 < ptcb->tcb_rbsize || window < ptcb->tcb_rmss)
	window = 0;
#ifdef SJCDEBUG
    rec_tcp_seg(0, 0, "TCPRWINDOW win = %d rmss = %d",
		window, ptcb->tcb_rmss);
    rec_tcp_seg(0, 0, "TCPRWINDOW cwin = %08x, rnext = %08x",
		ptcb->tcb_cwin, ptcb->tcb_rnext);
#endif
    window = max(window, ptcb->tcb_cwin - ptcb->tcb_rnext);
    ptcb->tcb_cwin = ptcb->tcb_rnext + window;
#ifdef SJCDEBUG
    rec_tcp_seg(0, 0, "TCPRWINDOW before return window = %d",
		window);
#endif

    return window;
}
