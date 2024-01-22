/* execl.c - execl */
  
#include <kernel.h>
#include <errno.h>
  
execl()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
