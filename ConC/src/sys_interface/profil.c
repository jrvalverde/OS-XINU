/* profil.c - profil */
  
#include <kernel.h>
#include <errno.h>
  
profil()
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
