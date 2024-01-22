/* ready.c - ready */

#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <errno.h>

/*------------------------------------------------------------------------
 * ready  --  make a process eligible for CPU service
 *------------------------------------------------------------------------
 */
int	ready (pid, resch)
	int	pid;			/* id of process to make ready	*/
	int	resch;			/* reschedule afterward?	*/
{
	register struct	pentry	*pptr;

	if (isbadpid(pid)) {
		errno = CEBADTID;
		return(SYSERR);
		}

	pptr = &proctab[pid];
	pptr->pstate = PRREADY;
	insert(pid,rdyhead,pptr->pprio);
	if (resch)
		resched();

	return(OK);
}
