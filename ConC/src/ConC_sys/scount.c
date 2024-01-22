/* scount.c - scount */

#include <kernel.h>
#include <sem.h>
#include <errno.h>

/*------------------------------------------------------------------------
 *  scount  --  return a semaphore count
 *------------------------------------------------------------------------
 */
SYSCALL scount(sem)
	int	sem;
{
	errno = 0;	/* to distinguish between error and valid sem count */

	if (isbadsem(sem) || semaph[sem].sstate==SFREE) {
		errno = CEBADSEM;
		return(SYSERR);
		}

	return(semaph[sem].semcnt);
}
