/****************************************/
/*					*/
/*	Modified from original source:	*/
/*					*/
/*	swait, ssignal added		*/
/*					*/
/****************************************/

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)clrerr.c	5.2 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

#include <stdio.h>
#undef	clearerr

clearerr(iop)
	register FILE *iop;
{
    stdio_swait(iop);

	iop->_flag &= ~(_IOERR|_IOEOF);

    stdio_ssignal(iop);
}
