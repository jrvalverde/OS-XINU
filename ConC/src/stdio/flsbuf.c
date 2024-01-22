/****************************************/
/*					*/
/*	Modified from original source:	*/
/*					*/
/*	stdio_* added (assumes		*/
/*	stdio_swait called)		*/
/*					*/
/****************************************/

/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)flsbuf.c	5.2 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/stat.h>

char	*malloc();

_flsbuf(c, iop)
unsigned char c;
register FILE *iop;
{
	register char *base;
	register n, rn;
	char c1;
	int size;
	struct stat stbuf;

	if (iop->_flag & _IORW) {
		iop->_flag |= _IOWRT;
		iop->_flag &= ~(_IOEOF|_IOREAD);
	}

	if ((iop->_flag&_IOWRT)==0)
		return(EOF);
tryagain:
	if (iop->_flag&_IOLBF) {
		base = iop->_base;
		*iop->_ptr++ = c;
		if (iop->_ptr >= base+iop->_bufsiz || c == '\n') {
			n = write(stdio_fileno(iop),base,rn = iop->_ptr - base);
			iop->_ptr = base;
			iop->_cnt = 0;
		} else
			rn = n = 0;
	} else if (iop->_flag&_IONBF) {
		c1 = c;
		rn = 1;
		n = write(stdio_fileno(iop), &c1, rn);
		iop->_cnt = 0;
	} else {
		if ((base=iop->_base)==NULL) {
			if (fstat(stdio_fileno(iop), &stbuf) < 0 ||
			    stbuf.st_blksize <= (long)NULL)
				size = BUFSIZ;
			else
				size = stbuf.st_blksize;
			if ((iop->_base=base=malloc(size)) == NULL) {
				iop->_flag |= _IONBF;
				goto tryagain;
			}
			iop->_flag |= _IOMYBUF;
			iop->_bufsiz = size;
			if (iop==stdout && isatty(stdio_fileno(stdout))) {
				iop->_flag |= _IOLBF;
				iop->_ptr = base;
				goto tryagain;
			}
			rn = n = 0;
		} else if ((rn = n = iop->_ptr - base) > 0) {
			iop->_ptr = base;
			n = write(stdio_fileno(iop), base, n);
		}
		iop->_cnt = iop->_bufsiz-1;
		*base++ = c;
		iop->_ptr = base;
	}
	if (rn != n) {
		iop->_flag |= _IOERR;
		return(EOF);
	}
	return(c);
}

/****************************************/
/*					*/
/*	Modified from original source:	*/
/*					*/
/*	swait, ssignal added		*/
/*					*/
/****************************************/

fflush(iop)
register FILE *iop;
{
	register char *base;
	register n;

    stdio_swait(iop);

	if ((iop->_flag&(_IONBF|_IOWRT))==_IOWRT
	 && (base=iop->_base)!=NULL && (n=iop->_ptr-base)>0) {
		iop->_ptr = base;
		iop->_cnt = (iop->_flag&(_IOLBF|_IONBF)) ? 0 : iop->_bufsiz;
		if (write(stdio_fileno(iop), base, n)!=n) {
			iop->_flag |= _IOERR;
			stdio_ssignal(iop);
			return(EOF);
		}
	}

    stdio_ssignal(iop);

	return(0);
}

fclose(iop)
	register FILE *iop;
{
	register int r;

    stdio_swait(iop);

	r = EOF;
	if (iop->_flag&(_IOREAD|_IOWRT|_IORW) && (iop->_flag&_IOSTRG)==0) {
		r = stdio_fflush(iop);
		if (close(stdio_fileno(iop)) < 0)
			r = EOF;
		if (iop->_flag&_IOMYBUF)
			free(iop->_base);
	}
	iop->_cnt = 0;
	iop->_base = (char *)NULL;
	iop->_ptr = (char *)NULL;
	iop->_bufsiz = 0;
	iop->_flag = 0;
	iop->_file = 0;

    stdio_ssignal(iop);

	return(r);
}

no_flush_fclose(iop)
	register FILE *iop;
{
	register int r;

    stdio_swait(iop);

	r = EOF;
	if (iop->_flag&(_IOREAD|_IOWRT|_IORW) && (iop->_flag&_IOSTRG)==0) {
		if (close(stdio_fileno(iop)) < 0)
			r = EOF;
		if (iop->_flag&_IOMYBUF)
			free(iop->_base);
	}
	iop->_cnt = 0;
	iop->_base = (char *)NULL;
	iop->_ptr = (char *)NULL;
	iop->_bufsiz = 0;
	iop->_flag = 0;
	iop->_file = 0;

    stdio_ssignal(iop);

	return(r);
}
