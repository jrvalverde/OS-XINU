/* select.c - select */
  
#include <kernel.h>
#include <errno.h>
  
select()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
