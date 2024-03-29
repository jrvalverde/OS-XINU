/****************************************
 *					*
 *    getproto.c			*
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
static char sccsid[] = "@(#)getproto.c	5.3 (Berkeley) 5/19/86";
#endif LIBC_SCCS and not lint

#include <netdb.h>

extern int _proto_stayopen;

struct protoent *
getprotobynumber(proto)
	register int proto;
{
	register struct protoent *p;

    swait(sem_sys_net);
	SYS_setprotoent(_proto_stayopen);
	while (p = SYS_getprotoent())
		if (p->p_proto == proto)
			break;
	if (!_proto_stayopen)
		SYS_endprotoent();
    ssignal(sem_sys_net);
	return (p);
}
