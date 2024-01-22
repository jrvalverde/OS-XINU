/* ptrace.c - ptrace */
  
#include <kernel.h>
#include <errno.h>
  
ptrace()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
