/* ungetc.c - ungetc() */

#include <stdio.h>

extern char libwarning;

/* Our current implementation of streams does not do buffering	*/
/* So this is kinda bogus but seems to work			*/

ungetc(c, fp)
char c;
FILE *fp;
{
	if (libwarning) {
		fprintf(stderr, "ungetc not properly implemented\n");
	}
	
	fp->_cnt = 1;
	fp->_ptr = fp->_base;
	*(fp->_ptr) = (unsigned char) c;
	return(c);
}


