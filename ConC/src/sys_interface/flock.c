/* flock.c - flock */
  
#include <kernel.h>
#include <errno.h>
  
flock()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
