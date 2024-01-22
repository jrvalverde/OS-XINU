/****************************************
 *					*
 *    sethostent.c			*
 *    Modified for mutual exclusion	*
 *					*
 ****************************************/
#include <kernel.h>
#undef NULL

/*
 * Copyright (c) 1985 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)sethostent.c	6.3 (Berkeley) 4/10/86";
#endif LIBC_SCCS and not lint

#include <sys/types.h>
#include <arpa/nameser.h>
#include <netinet/in.h>
#include <resolv.h>

sethostent(stayopen)
{
    swait(sem_sys_net);
	if (stayopen)
		_res.options |= RES_STAYOPEN | RES_USEVC;
    ssignal(sem_sys_net);
}

endhostent()
{
    swait(sem_sys_net);
	_res.options &= ~(RES_STAYOPEN | RES_USEVC);
	_res_close();
    ssignal(sem_sys_net);
}

sethostfile(name)
char *name;
{
#ifdef lint
name = name;
#endif
}
