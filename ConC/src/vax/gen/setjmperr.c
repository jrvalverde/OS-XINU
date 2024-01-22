/********************************
 *				*
 *   setjmperr.c		*
 *   Not allowed in ConcurrenC	*
 *   (We shouldn't have it	*
 *   called, because setjmp	*
 *   is not allowed.)		*
 *				*
 ********************************/

#include <kernel.h>
#undef NULL
#include <errno.h>

longjmperror()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
