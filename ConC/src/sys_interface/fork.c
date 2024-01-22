/* fork.c - fork */
  
#include <kernel.h>
#include <errno.h>
  
fork()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
