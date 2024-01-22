/****************************************
 *					*
 *    getnetbyname.c			*
 *    Modified for mutual exclusion	*
 *					*
 ****************************************/
#include <kernel.h>
#undef NULL

/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)getnetbyname.c	5.3 (Berkeley) 5/19/86";
#endif LIBC_SCCS and not lint

#include <netdb.h>

extern int _net_stayopen;

struct netent *
getnetbyname(name)
	register char *name;
{
	register struct netent *p;
	register char **cp;

    swait(sem_sys_net);
	SYS_setnetent(_net_stayopen);
	while (p = SYS_getnetent()) {
		if (strcmp(p->n_name, name) == 0)
			break;
		for (cp = p->n_aliases; *cp != 0; cp++)
			if (strcmp(*cp, name) == 0)
				goto found;
	}
found:
	if (!_net_stayopen)
		SYS_endnetent();
    ssignal(sem_sys_net);
	return (p);
}
