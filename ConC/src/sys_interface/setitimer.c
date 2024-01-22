/* setitimer.c - setitimer */
  
#include <kernel.h>
#include <errno.h>
  
setitimer()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
