/* resched.c  -  resched */

#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sys/time.h>

/*------------------------------------------------------------------------
 * resched  --  reschedule processor to highest priority ready process
 *
 * Notes:	Upon entry, currpid gives current process id.
 *		Proctab[currpid].pstate gives correct NEXT state for
 *			current process if other than PRCURR.
 *------------------------------------------------------------------------
 */
int	resched()
{
	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */

	/* no switch needed if current process priority higher than next*/

	if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
						(lastkey(rdytail)<optr->pprio))
		return(OK);

	/* force context switch */

	if (optr->pstate == PRCURR) {
		optr->pstate = PRREADY;
		insert(currpid,rdyhead,optr->pprio);
	}

	/* remove highest priority process at end of ready list */

	nptr = &proctab[ (currpid = getlast(rdytail)) ];
	nptr->pstate = PRCURR;		/* mark it currently running	*/

	if (is_serv_init) {		/* set new time quantum		*/
		vtival.it_value.tv_sec  = squantum;
		vtival.it_value.tv_usec = usquantum;

		sys_setitimer(ITIMER_VIRTUAL,&vtival,&vtioval);
		}

	optr->sys_errno = errno;
	errno = nptr->sys_errno;
	ctxsw(optr->pregs,nptr->pregs);

	/* The OLD process returns here when resumed. */

	return(OK);
}
