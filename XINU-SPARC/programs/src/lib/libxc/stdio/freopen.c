/* freopen.c - freopen() */

#include <stdio.h>
#include <fcntl.h>

FILE *freopen(file, mode, ioptr)
char *file;
char *mode;
FILE *ioptr;
{
	int fd, oflags;

	fclose(ioptr);
	oflags = 0;
	if (*mode == 'w')
		oflags = O_TRUNC | O_CREAT| O_WRONLY;
	if (*mode == 'a')
		oflags = O_CREAT | O_WRONLY;
	if (*mode == 'r')
		oflags = O_RDONLY;
	fd = open(file, oflags, 0666);
	if (fd < 0)
		return (NULL);
	ioptr->_cnt = 0;
	ioptr->_file = fd;
	return (ioptr);
}
