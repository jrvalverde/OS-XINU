/* execv.c - execv */
  
#include <kernel.h>
#include <errno.h>
  
execv()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
