/* getprio.c - getprio */

#include <kernel.h>
#include <proc.h>
#include <errno.h>

/*------------------------------------------------------------------------
 * getprio -- return the scheduling priority of a given process
 *------------------------------------------------------------------------
 */
SYSCALL getprio(pid)
	int	pid;
{
	int	prio;
	struct	pentry	*pptr;
	long	ps;

	disable(ps);
	if (isbadpid(pid) || (pptr = &proctab[pid])->pstate == PRFREE) {
		errno = CEBADTID;
		restore(ps);
		return(SYSERR);
	}
	prio = pptr -> pprio;
	restore(ps);
	return(prio);
}
