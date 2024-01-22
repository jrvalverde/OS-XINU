/* vhangup.c - vhangup */
  
#include <kernel.h>
#include <errno.h>
  
vhangup()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
