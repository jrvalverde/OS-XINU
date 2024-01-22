/****************************************/
/*					*/
/*	Modified from original source:	*/
/*					*/
/*	stdio_getc, swait, ssignal added*/
/*					*/
/****************************************/

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)fgetc.c	5.2 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

#include <stdio.h>

fgetc(fp)
FILE *fp;
{
	int ch;

	stdio_swait(fp);

	ch = stdio_getc(fp);

	stdio_ssignal(fp);

	return(ch);
}
