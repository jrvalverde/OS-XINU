/* fseek.c - fseek() */

#include <stdio.h>

fseek(ioptr, offset, ptrname)
register FILE *ioptr;
long	offset;
int	ptrname;
{
	int status;
	
	status = lseek(fileno(ioptr), offset, ptrname);
	if (status == (-1))
	    return(-1);
	else
	    return(0);
}
