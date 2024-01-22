/********************************
 *				*
 *   popen.c			*
 *   Not allowed in ConcurrenC	*
 *				*
 ********************************/

#include <kernel.h>
#undef NULL
#include <errno.h>
#include <stdio.h>

FILE *
popen(cmd,mode)
	char *cmd;
	char *mode;
{
    errno = CENOTALLOWED;
    return((FILE *)NULL);
}

pclose(ptr)
	FILE *ptr;
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
