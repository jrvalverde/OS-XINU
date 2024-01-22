/* sigsetmask.c - sigsetmask */
  
#include <kernel.h>
#include <errno.h>
  
sigsetmask()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
