/* tsendf.c - tsendf */

#include <kernel.h>
#include <proc.h>
#include <errno.h>

/*------------------------------------------------------------------------
 *  tsendf  --  sendf a message to another process, forcing delivery
 *------------------------------------------------------------------------
 */
SYSCALL	tsendf(pid, msg)
int	pid;
int	msg;
{
	struct	pentry	*pptr;
	long	ps;

	disable(ps);
	if (isbadpid(pid)||((pptr= &proctab[pid])->pstate == PRFREE)) {
		errno = CEBADTID;
		restore(ps);
		return(SYSERR);
	}
	pptr->pmsg = msg;
	pptr->phasmsg++;
	if (pptr->pstate == PRRECV)
		ready(pid, RESCHYES);

	return(OK);
}
