/****************************************
 *					*
 *    fstab.c				*
 *    Modified for mutual exclusion	*
 *					*
 *    Static return areas malloc'ed	*
 *    (fs and line).			*
 *    NOTE:  This area is not freed,	*
 *    but as someone has said --	*
 *    "Memory is cheap!"		*
 *					*
 *    static global fs_file renamed	*
 *    to SYS_fs_file and made global	*
 *    so SYS_ routines can access it.	*
 *					*
 ****************************************/
#include <kernel.h>
#undef NULL
char *malloc();

/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)fstab.c	5.2 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

#include <fstab.h>
#include <stdio.h>
#include <ctype.h>

FILE *SYS_fs_file = 0;

static char *
fsskip(p)
	register char *p;
{

	while (*p && *p != ':')
		++p;
	if (*p)
		*p++ = 0;
	return (p);
}

static char *
fsdigit(backp, string, end)
	int *backp;
	char *string, end;
{
	register int value = 0;
	register char *cp;

	for (cp = string; *cp && isdigit(*cp); cp++) {
		value *= 10;
		value += *cp - '0';
	}
	if (*cp == '\0')
		return ((char *)0);
	*backp = value;
	while (*cp && *cp != end)
		cp++;
	if (*cp == '\0')
		return ((char *)0);
	return (cp+1);
}

static
fstabscan(fs)
	struct fstab *fs;
{
	register char *cp;
    char *line = malloc(sizeof(char) * (BUFSIZ+1));

	cp = fgets(line, 256, SYS_fs_file);
	if (cp == NULL)
		return (EOF);
	fs->fs_spec = cp;
	cp = fsskip(cp);
	fs->fs_file = cp;
	cp = fsskip(cp);
	fs->fs_type = cp;
	cp = fsskip(cp);
	cp = fsdigit(&fs->fs_freq, cp, ':');
	if (cp == 0)
		return (3);
	cp = fsdigit(&fs->fs_passno, cp, '\n');
	if (cp == 0)
		return (4);
	return (5);
}
	
setfsent()
{

    swait(sem_sys_gen);
	if (SYS_fs_file)
		SYS_endfsent();
	if ((SYS_fs_file = fopen(FSTAB, "r")) == (FILE *)NULL) {
		SYS_fs_file = 0;
		{ssignal(sem_sys_gen); return (0);}
	}
    ssignal(sem_sys_gen);
	return (1);
}

endfsent()
{

    swait(sem_sys_gen);
	if (SYS_fs_file) {
		fclose(SYS_fs_file);
		SYS_fs_file = 0;
	}
    ssignal(sem_sys_gen);
	return (1);
}

struct fstab *SYS_getfsent();
struct fstab *
getfsent()
{
	int nfields;
    struct fstab *pfs = (struct fstab *)malloc(sizeof(struct fstab));;

    swait(sem_sys_gen);
	if ((SYS_fs_file == 0) && (SYS_setfsent() == 0))
		{ssignal(sem_sys_gen); return ((struct fstab *)0);}
    nfields = fstabscan(pfs);
	if (nfields == EOF || nfields != 5)
		{ssignal(sem_sys_gen); return ((struct fstab *)0);}
    ssignal(sem_sys_gen);
    return (pfs);
}

struct fstab *
getfsspec(name)
	char *name;
{
	register struct fstab *fsp;

    swait(sem_sys_gen);
	if (SYS_setfsent() == 0)	/* start from the beginning */
		{ssignal(sem_sys_gen); return ((struct fstab *)0);}
	while((fsp = SYS_getfsent()) != (struct fstab *)0)
		if (strcmp(fsp->fs_spec, name) == 0)
			{ssignal(sem_sys_gen); return (fsp);}
    ssignal(sem_sys_gen);
	return ((struct fstab *)0);
}

struct fstab *
getfsfile(name)
	char *name;
{
	register struct fstab *fsp;

    swait(sem_sys_gen);
	if (SYS_setfsent() == 0)	/* start from the beginning */
		{ssignal(sem_sys_gen); return ((struct fstab *)0);}
	while ((fsp = SYS_getfsent()) != (struct fstab *)0)
		if (strcmp(fsp->fs_file, name) == 0)
			{ssignal(sem_sys_gen); return (fsp);}
    ssignal(sem_sys_gen);
	return ((struct fstab *)0);
}

struct fstab *
getfstype(type)
	char *type;
{
	register struct fstab *fs;

    swait(sem_sys_gen);
	if (SYS_setfsent() == 0)
		{ssignal(sem_sys_gen); return ((struct fstab *)0);}
	while ((fs = SYS_getfsent()) != (struct fstab *)0)
		if (strcmp(fs->fs_type, type) == 0)
			{ssignal(sem_sys_gen); return (fs);}
    ssignal(sem_sys_gen);
	return ((struct fstab *)0);
}
