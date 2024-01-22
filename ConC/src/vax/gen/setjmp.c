/********************************
 *				*
 *   setjmp.c			*
 *   setjmp and longjmp		*
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

setjmp()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}

longjmp()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
