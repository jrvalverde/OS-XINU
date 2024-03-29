/* kill.c - kill */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <io.h>

/*------------------------------------------------------------------------
 * kill  --  kill a process and remove it from the system
 *------------------------------------------------------------------------
 */
SYSCALL kill(pid)
	int	pid;			/* process to kill		*/
{
	struct	pentry	*pptr;		/* points to proc. table for pid*/
	int	dev;
	PStype	ps;

	disable(ps);
	if (isbadpid(pid) || (pptr= &proctab[pid])->pstate==PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	if (--numproc == 0)
		xdone();
	dev = pptr->pdevs[0];
	if (! isbaddev(dev) )
		close(dev);
	dev = pptr->pdevs[1];
	if (! isbaddev(dev) )
		close(dev);
	send(pptr->pnxtkin, pid);
	freestk(pptr->pbase, pptr->pstklen);
	switch (pptr->pstate) {

	  case PRCURR:	pptr->pstate = PRFREE;	/* suicide */
			resched();

	  case PRWAIT:	semaph[pptr->psem].semcnt++;
						/* fall through */
	  case PRREADY:	dequeue(pid);
			pptr->pstate = PRFREE;
			break;

#ifdef	RTCLOCK
	  case PRSLEEP:
	  case PRTRECV:	unsleep(pid);
#endif
						/* fall through	*/
	  default:	pptr->pstate = PRFREE;
	}
	restore(ps);
	return(OK);
}
