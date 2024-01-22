/* exect.c - exect */
  
#include <kernel.h>
#include <errno.h>
  
exect()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
