/* execve.c - execve */
  
#include <kernel.h>
#include <errno.h>
  
execve()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
