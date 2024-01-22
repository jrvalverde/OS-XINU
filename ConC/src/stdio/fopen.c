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
static char sccsid[] = "@(#)fopen.c	5.2 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

#include <sys/types.h>
#include <sys/file.h>
#include <stdio.h>
#include <errno.h>

FILE *
fopen(file, mode)
	char *file;
	register char *mode;
{
	register FILE *iop;
	register f, rw, oflags;
	extern FILE */*_swait*/_findiop();

	iop = /*_swait*/_findiop();
	if (iop == (FILE *)NULL)
		return ((FILE *)NULL);

	rw = (mode[1] == '+');

	switch (*mode) {
	case 'a':
		oflags = O_CREAT | (rw ? O_RDWR : O_WRONLY);
		break;
	case 'r':
		oflags = rw ? O_RDWR : O_RDONLY;
		break;
	case 'w':
		oflags = O_TRUNC | O_CREAT | (rw ? O_RDWR : O_WRONLY);
		break;
	default:
		stdio_ssignal(iop); return ((FILE *)NULL);
	}

	f = open(file, oflags, 0666);
	if (f < 0)
		{stdio_ssignal(iop); return ((FILE *)NULL);}

	if (*mode == 'a')
		lseek(f, (off_t)0, L_XTND);

	iop->_cnt = 0;
	iop->_file = f;
	iop->_bufsiz = 0;
	if (rw)
		iop->_flag = _IORW;
	else if (*mode == 'r')
		iop->_flag = _IOREAD;
	else
		iop->_flag = _IOWRT;
	iop->_base = iop->_ptr = NULL;

    stdio_ssignal(iop);

	return (iop);
}
