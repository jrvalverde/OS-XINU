/****************************************
 *					*
 *    tpmnam.c				*
 *    Modified for mutual exclusion.	*
 *    Depending on implementation of	*
 *    ++ operator, duplicates may	*
 *    have been returned without mutex	*
 *					*
 ****************************************/

#include <kernel.h>
#undef NULL

/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)tmpnam.c	4.3 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

char *tmpnam(s)
char *s;
{
	static seed;

    swait(sem_sys_compat);
	sprintf(s, "temp.%d.%d", getpid(), seed++);
    ssignal(sem_sys_compat);
	return(s);
}
