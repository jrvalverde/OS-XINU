/****************************************/
/*					*/
/*	Modified from original source:	*/
/*					*/
/*	swait, ssignal added		*/
/*					*/
/****************************************/

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)scanf.c	5.2 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

#include	<stdio.h>

scanf(fmt, args)
char *fmt;
{
    int ret_val;

    stdio_swait(stdin);

    ret_val = (_doscan(stdin, fmt, &args));

    stdio_ssignal(stdin);

    return(ret_val);
}

fscanf(iop, fmt, args)
FILE *iop;
char *fmt;
{
    int ret_val;

    stdio_swait(iop);

    ret_val = (_doscan(iop, fmt, &args));

    stdio_ssignal(iop);

    return(ret_val);
}

sscanf(str, fmt, args)
register char *str;
char *fmt;
{
	FILE _strbuf;

	_strbuf._flag = _IOREAD|_IOSTRG;
	_strbuf._ptr = _strbuf._base = str;
	_strbuf._cnt = 0;
	while (*str++)
		_strbuf._cnt++;
	_strbuf._bufsiz = _strbuf._cnt;
	return(_doscan(&_strbuf, fmt, &args));
}
