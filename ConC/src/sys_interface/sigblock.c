/* sigblock.c - sigblock */
  
#include <kernel.h>
#include <errno.h>
  
sigblock()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
