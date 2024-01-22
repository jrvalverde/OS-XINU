/* fgetc - fgetc() */

#include <stdio.h>

fgetc(fptr)
FILE *fptr;
{
	return(getc(fptr));
}
