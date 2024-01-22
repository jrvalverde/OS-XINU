/* fputc.c - fputc() */

#include <stdio.h>

fputc(c, fptr)
int	c;
FILE *fptr;
{
	return(putc(c, fptr));
}
