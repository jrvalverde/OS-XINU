/* sendf.c - sendf */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

/*------------------------------------------------------------------------
 *  sendf  --  sendf a message to another process, forcing delivery
 *------------------------------------------------------------------------
 */
SYSCALL	sendf(pid, msg)
int	pid;
int	msg;
{
	struct	pentry	*pptr;
	PStype	ps;

	disable(ps);
	if (isbadpid(pid)||((pptr= &proctab[pid])->pstate == PRFREE)) {
		restore(ps);
		return(SYSERR);
	}
	pptr->pmsg = msg;
	pptr->phasmsg = TRUE;
	if (pptr->pstate == PRRECV)
		ready(pid, RESCHYES);
#ifdef RTCLOCK
	else if (pptr->pstate == PRTRECV) {
		unsleep(pid);
		ready(pid, RESCHYES);
	}
#endif
	restore(ps);
	return(OK);
}
