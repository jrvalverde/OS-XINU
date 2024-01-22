/* suspend.c - suspend */

#include <kernel.h>
#include <proc.h>
#include <errno.h>

/*------------------------------------------------------------------------
 *  suspend  --  suspend a process, placing it in hibernation
 *------------------------------------------------------------------------
 */
SYSCALL	suspend(pid)
	int	pid;			/* id of process to suspend	*/
{
	struct	pentry	*pptr;		/* pointer to proc. tab. entry	*/
	long	ps;			/* saved processor status	*/
	int	prio;			/* priority returned		*/

	disable(ps);
	if (isbadpid(pid) ||
	 ((pptr= &proctab[pid])->pstate!=PRCURR && pptr->pstate!=PRREADY)) {
		errno = (isbadpid(pid)) ? CEBADTID : CENOTREADY;
		restore(ps);
		return(SYSERR);
	}
	if (pptr->pstate == PRREADY) {
		dequeue(pid);
		pptr->pstate = PRSUSP;
	} else {
		pptr->pstate = PRSUSP;
		resched();
	}
	prio = pptr->pprio;
	restore(ps);

	return(prio);
}
