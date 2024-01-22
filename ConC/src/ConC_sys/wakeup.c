/* wakeup.c - wakeup */

#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sys/time.h>

/*------------------------------------------------------------------------
 * wakeup  --  called by clock interrupt dispatcher to awaken processes
 *------------------------------------------------------------------------
 */
INTPROC	wakeup()
{
	ready(getfirst(clockq),RESCHNO);
        while (nonempty(clockq) && firstkey(clockq) <= 0)
                ready(getfirst(clockq),RESCHNO);
	if ( slnempty = nonempty(clockq) ) {
		s10_to_time(q[q[clockq].qnext].qkey,rtival.it_value.tv_sec,
						    rtival.it_value.tv_usec);
		sys_setitimer(ITIMER_REAL,&rtival,&rtioval);
		}

	resched();
}
