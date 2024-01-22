/* ssignaln.c - ssignaln */

#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <errno.h>

/*------------------------------------------------------------------------
 *  ssignaln -- signal a semaphore n times
 *------------------------------------------------------------------------
 */
SYSCALL ssignaln(sem,count)
	int	sem;
	int	count;
{
	struct	sentry	*sptr;
	long	ps;

	disable(ps);
	if (isbadsem(sem) || semaph[sem].sstate==SFREE || count<=0) {
		errno = (count <= 0) ? EINVAL : CEBADSEM;
		restore(ps);
		return(SYSERR);
	}
	sptr = &semaph[sem];
	for (; count > 0  ; count--)
		if ((sptr->semcnt++) < 0)
			ready(getfirst(sptr->sqhead), RESCHNO);
	resched();
	restore(ps);

	return(OK);
}
