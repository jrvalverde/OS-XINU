/****************************************/
/*					*/
/*	User's version of putchar	*/
/*					*/
/****************************************/

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)putchar.c	5.2 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

/*
 * A subroutine version of the macro putchar
 */
#include <stdio.h>
#undef putchar

putchar(x)
{
	int ch;

	stdio_swait(stdout);

	ch = stdio_putc(x,stdout);

	stdio_ssignal(stdout);

	return(ch);
}
