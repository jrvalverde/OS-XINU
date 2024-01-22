/* /*_swait* /_findiop.c - /*_swait* /_findiop:				    */
/*					Get a new iob pointer and swait on  */
/*					its semaphore.  This swait will	    */
/*					ensure correct execution without    */
/*					reguard for race conditions.	    */

/*
 * Copyright (c) 1983, 1985 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)findiop.c	5.6 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

#include <kernel.h>
#undef NULL
#include <stdio.h>
#include <errno.h>

extern int errno;

#define active(iop)	((iop)->_flag & (_IOREAD|_IOWRT|_IORW))

#define NSTATIC	20	/* stdin + stdout + stderr + the usual */

FILE _iob[NSTATIC] = {
	{ 0, NULL, NULL, 0, _IOREAD,		0 },	/* stdin  */
	{ 0, NULL, NULL, 0, _IOWRT,		1 },	/* stdout */
	{ 0, NULL, NULL, 0, _IOWRT|_IONBF,	2 },	/* stderr */
};

extern	char	*calloc();

static	char sbuf[NSTATIC];
char	*_smallbuf = sbuf;
static	FILE	**iobglue;
static	FILE	**endglue;

/*
 * Find a free FILE for fopen et al.
 * We have a fixed static array of entries, and in addition
 * may allocate additional entries dynamically, up to the kernel
 * limit on the number of open files.
 * At first just check for a free slot in the fixed static array.
 * If none are available, then we allocate a structure to glue together
 * the old and new FILE entries, which are then no longer contiguous.
 */
FILE *
/*_swait*/
_findiop()
{
	register FILE **iov, *iop;
	register FILE *fp;
	long ps;

	disable(ps);

	if (iobglue == 0) {
		for (iop = _iob; iop < _iob + NSTATIC; iop++)
			if (!active(iop))
				{stdio_swait(iop); enable(ps); return (iop);}

		if (_f_morefiles() == 0) {
			errno = ENOMEM;
			enable(ps); return ((FILE *)NULL);
		}
	}

	iov = iobglue;
	while (*iov != (FILE *)NULL && active(*iov))
		if (++iov >= endglue) {
			errno = EMFILE;
			enable(ps); return ((FILE *)NULL);
		}

	if (*iov == (FILE *)NULL)
		*iov = (FILE *)calloc(1, sizeof **iov);

    stdio_swait(*iov);
    enable(ps);
	return (*iov);
}

_f_morefiles()
{
	register FILE **iov;
	register FILE *fp;
	register char *cp;
	int nfiles;

	nfiles = getdtablesize();

	iobglue = (FILE **)calloc(nfiles, sizeof *iobglue);
	if (iobglue == (FILE **)NULL)
		return (0);

	endglue = iobglue + nfiles;

	for (fp = _iob, iov = iobglue; fp < &_iob[NSTATIC]; /* void */)
		*iov++ = fp++;

	_smallbuf = calloc(nfiles, sizeof(*_smallbuf));
	return (1);
}

f_prealloc()
{
	register FILE **iov;
	register FILE *fp;

	if (iobglue == (FILE **)NULL && _f_morefiles() == 0)
		return;

	for (iov = iobglue; iov < endglue; iov++)
		if (*iov == (FILE *)NULL)
			*iov = (FILE *)calloc(1, sizeof **iov);
}

_fwalk(function)
	register int (*function)();
{
	register FILE **iov;
	register FILE *fp;

	if (iobglue == (FILE **)NULL) {
		for (fp = _iob; fp < &_iob[NSTATIC]; fp++)
			if (active(fp))
				(*function)(fp);
	} else {
		for (iov = iobglue; iov < endglue; iov++)
			if (*iov && active(*iov))
				(*function)(*iov);
	}
}

_cleanup()
{
	extern int stdio_fclose();

	_fwalk(stdio_fclose);
}
