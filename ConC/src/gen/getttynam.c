/****************************************
 *					*
 *    getttynam.c			*
 *    Modified for mutual exclusion	*
 *					*
 ****************************************/
#include <kernel.h>
#undef NULL
struct ttyent *SYS_getttyent();

/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)getttynam.c	5.2 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

#include <ttyent.h>

struct ttyent *
getttynam(tty)
	char *tty;
{
	register struct ttyent *t;

    swait(sem_sys_gen);
	SYS_setttyent();
	while (t = SYS_getttyent()) {
		if (strcmp(tty, t->ty_name) == 0)
			break;
	}
	SYS_endttyent();
    ssignal(sem_sys_gen);
	return (t);
}
