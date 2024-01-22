/* sreset.c - sreset */

#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <errno.h>

/*------------------------------------------------------------------------
 *  sreset  --  reset the count and queue of a semaphore
 *------------------------------------------------------------------------
 */
SYSCALL sreset(sem,count)
	int	sem;
	int	count;
{
	struct	sentry	*sptr;
	long	ps;
	int	pid;
	int	slist;

	disable(ps);
	if (isbadsem(sem) || count<0 || semaph[sem].sstate==SFREE) {
		errno = (count < 0) ? EINVAL : CEBADSEM;
		restore(ps);
		return(SYSERR);
	}
	sptr = &semaph[sem];
	slist = sptr->sqhead;
	while ((pid=getfirst(slist)) != EMPTY)
		ready(pid,RESCHNO);
	sptr->semcnt = count;
	resched();
	restore(ps);

	return(OK);
}
