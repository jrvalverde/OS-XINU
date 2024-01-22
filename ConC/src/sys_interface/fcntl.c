/* fcntl.c - fcntl */
  
#include <kernel.h>
#include <errno.h>
  
fcntl()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
