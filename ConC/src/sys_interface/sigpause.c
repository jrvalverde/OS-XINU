/* sigpause.c - sigpause */
  
#include <kernel.h>
#include <errno.h>
  
sigpause()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
