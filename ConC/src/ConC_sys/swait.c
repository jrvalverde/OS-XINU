/* swait.c - swait */

#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <errno.h>

/*------------------------------------------------------------------------
 * swait  --  make current process wait on a semaphore
 *------------------------------------------------------------------------
 */
SYSCALL	swait(sem)
	int	sem;
{
	long	ps;
	register struct	sentry	*sptr;
	register struct	pentry	*pptr;

	disable(ps);
	if (isbadsem(sem) || (sptr= &semaph[sem])->sstate==SFREE) {
		errno = CEBADSEM;
		restore(ps);
		return(SYSERR);
	}
	if (--(sptr->semcnt) < 0) {
		(pptr = &proctab[currpid])->pstate = PRWAIT;
		pptr->psem = sem;
		enqueue(currpid,sptr->sqtail);
		resched();
	}
	restore(ps);

	return(OK);
}
