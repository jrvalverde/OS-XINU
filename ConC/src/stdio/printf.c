/****************************************/
/*					*/
/*	Modified from original source:	*/
/*					*/
/*	swait, ssignal added		*/
/*					*/
/****************************************/

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)printf.c	5.2 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

#include	<stdio.h>

printf(fmt, args)
char *fmt;
{
    int ret_val;

    stdio_swait(stdout);

	_doprnt(fmt, &args, stdout);

	ret_val = (stdio_ferror(stdout)? EOF: 0);

    stdio_ssignal(stdout);

    return(ret_val);
}
