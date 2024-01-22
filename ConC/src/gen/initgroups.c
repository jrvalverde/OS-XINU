/****************************************
 *					*
 *    initgroups.c			*
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
static char sccsid[] = "@(#)initgroups.c	5.3 (Berkeley) 4/27/86";
#endif LIBC_SCCS and not lint

/*
 * initgroups
 */
#include <stdio.h>
#undef NULL
#include <sys/param.h>
#include <grp.h>

struct group *SYS_getgrent();

initgroups(uname, agroup)
	char *uname;
	int agroup;
{
	int groups[NGROUPS], ngroups = 0;
	register struct group *grp;
	register int i;

    swait(sem_sys_gen);
	if (agroup >= 0)
		groups[ngroups++] = agroup;
	SYS_setgrent();
	while (grp = SYS_getgrent()) {
		if (grp->gr_gid == agroup)
			continue;
		for (i = 0; grp->gr_mem[i]; i++)
			if (!strcmp(grp->gr_mem[i], uname)) {
				if (ngroups == NGROUPS) {
fprintf(stderr, "initgroups: %s is in too many groups\n", uname);
					goto toomany;
				}
				groups[ngroups++] = grp->gr_gid;
			}
	}
toomany:
	SYS_endgrent();
	if (setgroups(ngroups, groups) < 0) {
		perror("setgroups");
		{ssignal(sem_sys_gen); return (-1);}
	}
    ssignal(sem_sys_gen);
	return (0);
}
