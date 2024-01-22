/* 
 * fastgetc.c - do buffered reads using getc
 * 
 * Author:	Victor Norman
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Sep  6 10:20:01 1990
 *
 * Copyright (c) 1990 Victor Norman
 */

#ifdef XINU8

#include "def.h"

#define BUFSIZE	512

static char 	buffer[BUFSIZE];
static int 	nextchar = BUFSIZE;
static int	bufend = 0;

int fastgetc(ffp)
     FILE *ffp;
{
	register int c;
	int n;
	
	if (nextchar >= BUFSIZE) {
		/* read in a new buffer from the file */
		if ((bufend = fread(buffer, 1, BUFSIZE, ffp)) == EOF)
		    return(EOF);
		nextchar = 0;
	}

	if (nextchar == bufend) {
		/* have read all of the file, so clear out the buffer
		   and reset the pointers. */
		nextchar = BUFSIZE;
		bufend = 0;
		return(EOF);
	}
	c = (int) buffer[nextchar++];
	return(c);
}
#endif
