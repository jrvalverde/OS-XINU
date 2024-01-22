/* wait3.c - wait3 */
  
#include <kernel.h>
#include <errno.h>
  
wait3()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
