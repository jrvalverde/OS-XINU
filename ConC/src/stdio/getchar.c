/****************************************/
/*					*/
/*	User's version of getchar	*/
/*					*/
/****************************************/

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)getchar.c	5.2 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

/*
 * A subroutine version of the macro getchar.
 */
#include <stdio.h>
#undef getchar

getchar()
{
	int ch;

	stdio_swait(stdin);

	ch = stdio_getc(stdin);

	stdio_ssignal(stdin);

	return(ch);
}
