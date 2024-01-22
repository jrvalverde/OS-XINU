/* wait.c - wait */
  
#include <kernel.h>
#include <errno.h>
  
wait()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
