/* sleep10.c - sleep10 */

#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sys/time.h>
#include <errno.h>

/*------------------------------------------------------------------------
 * sleep10  --  delay the caller for a time specified in tenths of seconds
 *------------------------------------------------------------------------
 */
SYSCALL	sleep10(n)
	int n;
{
	long ps;

	if (n < 0) {
		errno = EINVAL;
		return(SYSERR);
		}
	if (n == 0) {
        	disable(ps);
		resched();		/* sleep10(0) -> end time slice	*/
        	restore(ps);
		return(OK);
		}
        disable(ps);
	if (slnempty) {			/* adjust task at head of queue */
		sys_getitimer(ITIMER_REAL,&rtival);
		q[q[clockq].qnext].qkey = time_to_s10(rtival.it_value.tv_sec,
						      rtival.it_value.tv_usec);
		}
        insertd(currpid,clockq,n);
	slnempty = TRUE;
	if (firstid(clockq) == currpid) {	/* reset timer if this task  */
						/* was inserted at the head  */
						/* of the queue.	     */
		s10_to_time(q[q[clockq].qnext].qkey,rtival.it_value.tv_sec,
						    rtival.it_value.tv_usec);
		sys_setitimer(ITIMER_REAL,&rtival,&rtioval);
		}
        proctab[currpid].pstate = PRSLEEP;

	resched();
        restore(ps);

	return(OK);
}
