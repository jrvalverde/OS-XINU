/* access.c - accessability */

#include <conf.h>
#include <kernel.h>
#include <file.h>
#include <name.h>

/*------------------------------------------------------------------------
 *  access  -  determine accessability given file name and desired mode
 *------------------------------------------------------------------------
 */
SYSCALL	access(name, mode)
char	*name;
char	*mode;
{
	char	fullnam[NAMLEN];
	struct	devsw	*devptr;
	int	dev;

	if ( (dev=nammap(name, fullnam)) == SYSERR)
		return(SYSERR);
	devptr = &devtab[dev];
	return(	(*devptr->dvcntl)(devptr, FLACCESS, fullnam, mode) );
}

