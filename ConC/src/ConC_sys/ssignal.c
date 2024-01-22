/* ssignal.c - ssignal */

#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <errno.h>

/*------------------------------------------------------------------------
 * ssignal  --  signal a semaphore, releasing one waiting process
 *------------------------------------------------------------------------
 */
SYSCALL ssignal(sem)
register int	sem;
{
	register struct	sentry	*sptr;
	long	ps;

	disable(ps);
	if (isbadsem(sem) || (sptr= &semaph[sem])->sstate==SFREE) {
		errno = CEBADSEM;
		restore(ps);
		return(SYSERR);
	}
	if ((sptr->semcnt++) < 0)
		ready(getfirst(sptr->sqhead), RESCHYES);

	restore(ps);

	return(OK);
}
