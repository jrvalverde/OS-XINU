/********************************
 *				*
 *   alarm.c			*
 *   Not allowed in ConcurrenC.	*
 *   ConcurrenC uses the	*
 *   interval timers.		*
 *				*
 ********************************/

#include <kernel.h>
#include <errno.h>

alarm(secs)
	int secs;
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
