/* resume.c - resume */

#include <kernel.h>
#include <proc.h>
#include <errno.h>

/*------------------------------------------------------------------------
 * resume  --  unsuspend a process, making it ready; return the priority
 *------------------------------------------------------------------------
 */
SYSCALL resume(pid)
	int	pid;
{
	long	ps;			/* saved processor status	*/
	struct	pentry	*pptr;		/* pointer to proc. tab. entry	*/
	int	prio;			/* priority to return		*/

	disable(ps);
	if (isbadpid(pid)) {
		errno = CEBADTID;
		restore(ps);
		return(SYSERR);
	}
	if ((pptr = &proctab[pid])->pstate != PRSUSP) {
		errno = CENOTSUSP;
		restore(ps);
		return(SYSERR);
	}
	prio = pptr->pprio;
	ready(pid, RESCHYES);
	restore(ps);

	return(prio);
}
