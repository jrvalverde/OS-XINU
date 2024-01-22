/********************************
 *				*
 *   execvp.c			*
 *   Not allowed in ConcurrenC	*
 *				*
 ********************************/

#include <kernel.h>
#include <errno.h>

execlp(name, argv)
char *name, *argv;
{
    errno = CENOTALLOWED;
    return(SYSERR);
}

execvp(name, argv)
char *name, **argv;
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
