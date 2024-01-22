/********************************
 *				*
 *   ualarm.c			*
 *   Not allowed in ConcurrenC	*
 *				*
 ********************************/

#include <kernel.h>
#undef NULL
#include <errno.h>

unsigned
ualarm(usecs, reload)
	register unsigned usecs;
	register unsigned reload;
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
