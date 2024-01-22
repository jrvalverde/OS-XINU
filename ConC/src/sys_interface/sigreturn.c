/* sigreturn.c - sigreturn */
  
#include <kernel.h>
#include <errno.h>
  
sigreturn()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
