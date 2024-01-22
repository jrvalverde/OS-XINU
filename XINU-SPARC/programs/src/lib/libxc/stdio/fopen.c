/* fopen.c - fopen(), freopen() */

#include <stdio.h>
#include <fcntl.h>

extern FILE *_findiop(), *_endopen();

#ifdef OLDWAY
FILE *freopen(file, mode, iop)
char *file, *mode;
FILE *iop;
{
	fclose(iop);
	return (fopen(file, mode));
}
#endif

FILE *fopen(file, mode)
char *file;
char *mode;
{
	FILE *ioptr;
	int oflag, fd;

	ioptr = _findiop();
	if (ioptr == NULL || file == NULL || strlen(file) == 0)
		return (NULL);
	/* xinu doesn't now about appending, so ignore it */
	if (*mode == 'w')
		oflag = O_WRONLY | O_TRUNC | O_CREAT;
	else if (*mode == 'r')
		oflag = O_RDONLY;
	else if (*mode == 'a')
		oflag = O_WRONLY | O_CREAT;
	else
		return (NULL);

	if ((fd = open(file, oflag, 0666)) < 0)
		return (NULL);
	ioptr->_file = fd;
	ioptr->_cnt = 0;
	if (ioptr->_base == NULL) {
		ioptr->_base = (char *) malloc(32);
	}
	ioptr->_ptr = ioptr->_base;
	ioptr->_bufsiz = 0;
	return (ioptr);
}
