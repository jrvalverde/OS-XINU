/****************************************/
/*					*/
/*	Modified from original source:	*/
/*					*/
/*	swait, ssignal added		*/
/*					*/
/****************************************/

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)puts.c	5.2 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

#include	<stdio.h>

puts(s)
register char *s;
{
    int ret_val;
	register c;

    stdio_swait(stdout);

	while (c = *s++)
		stdio_putchar(c);
    ret_val = (stdio_putchar('\n'));

    stdio_ssignal(stdout);

    return(ret_val);
}
