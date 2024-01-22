/****************************************/
/*					*/
/*	User's version of putc		*/
/*					*/
/****************************************/

#include <stdio.h>
#undef putc

putc(x,p)
int x;
FILE *p;
{
	int ch;

	stdio_swait(p);

	ch = stdio_putc(x,p);

	stdio_ssignal(p);

	return(ch);
}
