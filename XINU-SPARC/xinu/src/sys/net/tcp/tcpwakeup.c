/* tcpwakeup.c - tcpwakeup */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>
#ifdef XSELECT
#include <proc.h>
#endif XSELECT

/*#define DEBUG_WRITERS*/
/*#define DEBUG*/

#ifdef DEBUG
#include <sem.h>
#include <q.h>
#endif

/*------------------------------------------------------------------------
 *  tcpwakeup -  wake up processes sleeping for TCP, if necessary
 *	NB: Called with tcb_mutex HELD
 *------------------------------------------------------------------------
 */
int tcpwakeup(type, ptcb)
int		type;
struct	tcb	*ptcb;
{
	int	freelen;

	if (type & READERS) {
#ifdef DEBUG
		kprintf("tcpwakeup: waking up readers.\n");
#endif
#ifdef XSELECT
		if (((ptcb->tcb_flags & TCBF_RDONE) ||
		     ptcb->tcb_rbcount > 0 ||
		     ptcb->tcb_rudq >= 0)) {
#ifdef DEBUG
			if (!isbadpid(ptcb->tcb_readpid) {
				kprintf("tcpdata: someone is waiting for us.\n");
			} else {
				kprintf("tcpdata: no one selecting.\n");
			}
#endif		
			if (!isbadpid(ptcb->tcb_readpid)) {
				setbit(ptcb->tcb_dvnum,
				       &proctab[ptcb->tcb_readpid].infds);
				send(ptcb->tcb_readpid, IOREADY);
			}

			if (scount(ptcb->tcb_rsema) <= 0) {

#ifdef DEBUG
				kprintf("tcpwakeup: waking up blocked readers\n");
				kprintf("tcpwakeup: blocked proc = %d.\n",
					q[semaph[ptcb->tcb_rsema].sqhead].qnext);
#endif

				signal(ptcb->tcb_rsema);
			}
		}

#else XSELECT
		if (((ptcb->tcb_flags & TCBF_RDONE) ||
		    ptcb->tcb_rbcount > 0 || ptcb->tcb_rudq >= 0) &&
		    scount(ptcb->tcb_rsema) <= 0)
			signal(ptcb->tcb_rsema);
#endif XSELECT
	}
	if (type & WRITERS) {
		freelen = ptcb->tcb_sbsize - ptcb->tcb_sbcount;
#ifdef XSELECT
		if ((ptcb->tcb_flags & TCBF_SDONE) || (freelen > 0)) {
			/*
			 * we can write.  The question is whether
			 * there is a blocked writer, or someone doing
			 * select.
			 */

#ifdef DEBUG_WRITERS
			if (!isbadpid(ptcb->tcb_writepid)) {
				kprintf("tcpwakeup: waking writepid: %d, len: %d\n",
					ptcb->tcb_writepid, freelen);
			} else {
			    kprintf("tcpwakeup: no one selecting for us.\n");
			}
#endif	    
			if (!isbadpid(ptcb->tcb_writepid)) {
			    setbit(ptcb->tcb_dvnum,
				   &proctab[ptcb->tcb_writepid].outfds);
			    send(ptcb->tcb_writepid, IOREADY);
			}
			
			if (scount(ptcb->tcb_ssema) <= 0) {
			    
#ifdef DEBUG
			    kprintf("tcpwakeup: blocked writers.\n");
#endif	    
			    signal(ptcb->tcb_ssema);
			    
			}
		    }
#else XSELECT
		if (((ptcb->tcb_flags & TCBF_SDONE) || freelen > 0) &&
		    scount(ptcb->tcb_ssema) <= 0)
		    signal(ptcb->tcb_ssema);
#endif XSELECT
		/* special for abort */
		if (ptcb->tcb_error && ptcb->tcb_ocsem > 0) {
		    if (ptcb->tcb_nbio &&
			(ptcb->tcb_state >= TCPS_ESTABLISHED))
			tcbdealloc(ptcb); /* closer is gone */
		    else if ((ptcb->tcb_flags & TCBF_NOREAD) &&
			     (ptcb->tcb_flags & TCBF_NOWRITE))
			tcbdealloc(ptcb);
		    else
			signal(ptcb->tcb_ocsem);
		}
	    }
		return OK;
	    }
