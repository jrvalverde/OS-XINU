/* Ovadvise.c - Ovadvise */
  
#include <kernel.h>
#include <errno.h>
  
Ovadvise()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
