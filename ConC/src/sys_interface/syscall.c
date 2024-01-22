/* syscall.c - syscall */
  
#include <kernel.h>
#include <errno.h>
  
syscall()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
