/****************************************/
/*					*/
/*	Modified from original source:	*/
/*					*/
/*	swait, ssignal added		*/
/*					*/
/****************************************/

/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)setbuf.c	5.2 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

#include	<stdio.h>

setbuf(iop, buf)
register FILE *iop;
char *buf;
{
    stdio_swait(iop);

	if (iop->_base != NULL && iop->_flag&_IOMYBUF)
		free(iop->_base);
	iop->_flag &= ~(_IOMYBUF|_IONBF|_IOLBF);
	if ((iop->_base = buf) == NULL) {
		iop->_flag |= _IONBF;
		iop->_bufsiz = (int)NULL;
	} else {
		iop->_ptr = iop->_base;
		iop->_bufsiz = BUFSIZ;
	}
	iop->_cnt = 0;

    stdio_ssignal(iop);
}
