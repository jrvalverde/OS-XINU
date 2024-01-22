/* tkill.c - tkill */

#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <errno.h>

/*------------------------------------------------------------------------
 * tkill  --  kill a process and remove it from the system
 *------------------------------------------------------------------------
 */
SYSCALL tkill(pid)
	int	pid;			/* process to kill		*/
{
	struct	pentry	*pptr;		/* points to proc. table for pid*/
	long	ps;			/* saved processor status	*/

	disable(ps);
	if (isbadpid(pid) || (pptr= &proctab[pid])->pstate==PRFREE) {
		errno = CEBADTID;
		restore(ps);
		return(SYSERR);
	}

	if (--numproc <= 0)
		xdone();
	freestk(pptr->pbase, pptr->pstklen);
	switch (pptr->pstate) {

	case PRCURR:	pptr->pstate = PRFREE;	/* suicide */
			resched();

	case PRWAIT:	if (pptr -> psem == pptr -> sys_sem)  /* I/O pending */
				Flush_IO_Pending(pid);

			semaph[pptr->psem].semcnt++;
						/* fall through */
	case PRSLEEP:
	case PRREADY:	dequeue(pid);
						/* fall through */
	default:	pptr->pstate = PRFREE;
	}
	restore(ps);

	return(OK);
}
