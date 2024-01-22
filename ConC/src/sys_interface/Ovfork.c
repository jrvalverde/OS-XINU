/* Ovfork.c - Ovfork */
  
#include <kernel.h>
#include <errno.h>
  
Ovfork()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
