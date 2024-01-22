/********************************
 *				*
 *   pause.c			*
 *   Not allowed in ConcurrenC	*
 *				*
 ********************************/

#include <kernel.h>
#include <errno.h>

pause()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
