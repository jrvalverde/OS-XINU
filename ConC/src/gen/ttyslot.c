/****************************************
 *					*
 *    ttyslot.c				*
 *    Modified for mutual exclusion	*
 *					*
 ****************************************/
#include <kernel.h>
#undef NULL
extern struct ttyent *SYS_getttyent();

/*
 * Copyright (c) 1984 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)ttyslot.c	5.2 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

/*
 * Return the number of the slot in the utmp file
 * corresponding to the current user: try for file 0, 1, 2.
 * Definition is the line number in the /etc/ttys file.
 */
#include <ttyent.h>

char	*ttyname();
char	*rindex();

#define	NULL	0

ttyslot()
{
	register struct ttyent *ty;
	register char *tp, *p;
	register s;

    swait(sem_sys_gen);
	if ((tp = ttyname(0)) == NULL &&
	    (tp = ttyname(1)) == NULL &&
	    (tp = ttyname(2)) == NULL)
		{ssignal(sem_sys_gen); return(0);}
	if ((p = rindex(tp, '/')) == NULL)
		p = tp;
	else
		p++;
	SYS_setttyent();
	s = 0;
	while ((ty = SYS_getttyent()) != (struct ttyent *)NULL) {
		s++;
		if (strcmp(ty->ty_name, p) == 0) {
			SYS_endttyent();
			{ssignal(sem_sys_gen); return (s);}
		}
	}
	SYS_endttyent();
    ssignal(sem_sys_gen);	
	return (0);
}
