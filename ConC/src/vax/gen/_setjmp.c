/********************************
 *				*
 *   _setjmp.c			*
 *   _setjmp and _longjmp	*
 *   Not allowed in ConcurrenC	*
 *   (We might have inside a	*
 *   mutex area, in which case	*
 *   the associated semaphore	*
 *   would not get signaled)	*
 *				*
 ********************************/

#include <kernel.h>
#undef NULL
#include <errno.h>

_setjmp()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}

_longjmp()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
