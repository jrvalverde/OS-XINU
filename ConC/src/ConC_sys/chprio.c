/* chprio.c - chprio */

#include <kernel.h>
#include <proc.h>
#include <errno.h>

/*------------------------------------------------------------------------
 * chprio  --  change the scheduling priority of a process
 *------------------------------------------------------------------------
 */
SYSCALL chprio(pid,newprio)
	int	pid;
	int	newprio;		/* newprio > 0			*/
{
	int	oldprio;
	struct	pentry	*pptr;
        long    ps;

	disable(ps);
	if (isbadpid(pid) || newprio<=0 ||
	    (pptr = &proctab[pid])->pstate == PRFREE) {
		errno = (newprio <= 0) ? EINVAL : CEBADTID;
		restore(ps);
		return(SYSERR);
	}
	oldprio = pptr->pprio;
	pptr->pprio = newprio;
	restore(ps);
	return(oldprio);
}
