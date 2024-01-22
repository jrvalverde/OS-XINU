
/* 
 * newfread.c - 
 * 
 * Author:	
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Tue Sep 11 14:48:52 1990
 *
 * Copyright (c) 1990 
 */

#include <stdio.h>

#ifdef MIPS
size_t
#endif
fread(ptr, size, count, ioptr)
char *ptr;
int size;
int count;
FILE *ioptr;
{
	int bytes_read;
	
	if (size <= 0 || count <= 0)
	    return 0;

	if ((bytes_read = read(fileno(ioptr), ptr, count * size)) < 0)
	    return(EOF);
	else
	    return(bytes_read / size);
	
}


