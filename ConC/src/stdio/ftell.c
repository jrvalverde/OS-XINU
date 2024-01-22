/****************************************/
/*					*/
/*	Modified from original source:	*/
/*					*/
/*	swait, ssignal added		*/
/*					*/
/****************************************/

/* @(#)ftell.c	4.2 (Berkeley) 3/9/81 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)ftell.c	5.2 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

/*
 * Return file offset.
 * Coordinates with buffering.
 */

#include	<stdio.h>
long	lseek();


long ftell(iop)
register FILE *iop;
{
	register long tres;
	register adjust;

    stdio_swait(iop);

	if (iop->_cnt < 0)
		iop->_cnt = 0;
	if (iop->_flag&_IOREAD)
		adjust = - iop->_cnt;
	else if (iop->_flag&(_IOWRT|_IORW)) {
		adjust = 0;
		if (iop->_flag&_IOWRT && iop->_base && (iop->_flag&_IONBF)==0)
			adjust = iop->_ptr - iop->_base;
	} else {
		stdio_ssignal(iop);
		return(-1);
	}
	tres = lseek(stdio_fileno(iop), 0L, 1);
	if (tres<0) {
		stdio_ssignal(iop);
		return(tres);
	}
	tres += adjust;

    stdio_ssignal(iop);

	return(tres);
}
