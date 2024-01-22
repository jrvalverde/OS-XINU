/****************************************/
/*					*/
/*	User's version of getc		*/
/*					*/
/****************************************/

#include <stdio.h>
#undef getc

getc(p)
FILE *p;
{
	int ch;

	stdio_swait(p);

	ch = stdio_getc(p);

	stdio_ssignal(p);

	return(ch);
}
