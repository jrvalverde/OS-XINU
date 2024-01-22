/********************************
 *				*
 *   reset.c			*
 *   setexit and reset		*
 *   Not allowed in ConcurrenC	*
 *				*
 ********************************/

#include <kernel.h>
#undef NULL
#include <errno.h>

setexit()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}

reset()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
