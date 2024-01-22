/* getitimer.c - getitimer */
  
#include <kernel.h>
#include <errno.h>
  
getitimer()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
