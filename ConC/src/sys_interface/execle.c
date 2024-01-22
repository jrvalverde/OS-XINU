/* execle.c - execle */
  
#include <kernel.h>
#include <errno.h>
  
execle()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
