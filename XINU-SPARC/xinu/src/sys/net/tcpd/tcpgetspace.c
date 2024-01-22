/* tcpgetspace.c - tcpgetspace */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>
#include <nettcp.h>

/*------------------------------------------------------------------------
 *  tcpgetspace  -  wait for space in the send buffer
 *	N.B. - returns with tcb_mutex HELD
 *------------------------------------------------------------------------
 */
int tcpgetspace(ptcb, len)
     struct	tcb	*ptcb;
     int		len;
{
    if (len > ptcb->tcb_sbsize)
	return TCPE_TOOBIG;	/* we'll never have this much	*/
    while (1) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("tcpgetspace: wait(ssema)...");
#endif
	wait(ptcb->tcb_ssema);
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("wait(mutex)...");
#endif
	wait(ptcb->tcb_mutex);
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("done\n");
#endif
	if (ptcb->tcb_state == TCPS_FREE)
	    return SYSERR;			/* gone		*/
	if (ptcb->tcb_error) {
	    tcpwakeup(WRITERS, ptcb);	/* propagate it */
	    signal(ptcb->tcb_mutex);
	    return ptcb->tcb_error;
	}
	/* applied fix from dls - sdo - Mon Nov 23, 1992 */
	if (len <= ptcb->tcb_sbsize - ptcb->tcb_sbcount)
	    return len;
	signal(ptcb->tcb_mutex);
    }
}
