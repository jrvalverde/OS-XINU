/* fdopen.c - fdopen() */

#include <stdio.h>
#include <sys/errno.h>

extern long lseek();
extern FILE *_findiop();

/*---------------------------------------------------------------------------
 * fdopen - reopen a file that is already open given a file descriptor
 *---------------------------------------------------------------------------
 */
FILE *fdopen(fd, mode)
int	fd;
char *mode;
{
	register FILE *ioptr;

	if(fd < 0 || fd >= getdtablesize())
		return(NULL);

	if((ioptr = _findiop()) == NULL)
		return(NULL);

	ioptr->_file = fd;
	ioptr->_cnt = 0;
	ioptr->_base = NULL;
	ioptr->_ptr = NULL;
	ioptr->_bufsiz = 0;
	/* ignore the mode */
	return(ioptr);
}
