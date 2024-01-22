/****************************************
 *					*
 *    rand.c				*
 *    Modified for mutual exclusion	*
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
static char sccsid[] = "@(#)rand.c	5.2 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

static	long	randx = 1;

srand(x)
unsigned x;
{
    swait(sem_sys_compat);
	randx = x;
    ssignal(sem_sys_compat);
}

rand()
{
    unsigned ret_val;

    swait(sem_sys_compat);
	ret_val = ((randx = randx * 1103515245 + 12345) & 0x7fffffff);
    ssignal(sem_sys_compat);

    return(ret_val);
}
