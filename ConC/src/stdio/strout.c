#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)strout.c	5.2 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

#include	<stdio.h>

_strout(count, string, adjust, file, fillch)
register char *string;
register count;
int adjust;
register FILE *file;
{
	while (adjust < 0) {
		if (*string=='-' && fillch=='0') {
			stdio_putc(*string++, file);
			count--;
		}
		stdio_putc(fillch, file);
		adjust++;
	}
	while (--count>=0)
		stdio_putc(*string++, file);
	while (adjust) {
		stdio_putc(fillch, file);
		adjust--;
	}
}
