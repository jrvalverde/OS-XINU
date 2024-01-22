/****************************************
 *					*
 *    getpwent.c			*
 *    Modified for mutual exclusion	*
 *					*
 *    Static return areas malloc'ed	*
 *    (line and passwd)			*
 *    NOTE:  This area is not freed,	*
 *    but as someone has said --	*
 *    "Memory is cheap!"		*
 *					*
 *    Static globals pwf and EMPTY	*
 *    changed to regular globals	*
 *    SYS_pwf SYS_EMPTY so as to	*
 *    make them accessible from SYS_	*
 *    routines.				*
 *					*
 ****************************************/
#include <kernel.h>
#undef NULL
char *malloc();

/*
 * Copyright (c) 1984 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)getpwent.c	5.2 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

#include <stdio.h>
#include <pwd.h>
#include <ndbm.h>

char SYS_EMPTY[] = "";
FILE *SYS_pwf = (FILE *)NULL;

/*
 * The following are shared with getpwnamuid.c
 */
char	*_pw_file = "/etc/passwd";
DBM	*_pw_db;
int	_pw_stayopen;

setpwent()
{
    swait(sem_sys_gen);
	if (SYS_pwf == (FILE *)NULL)
		SYS_pwf = fopen(_pw_file, "r");
	else
		rewind(SYS_pwf);
    ssignal(sem_sys_gen);
}

endpwent()
{
    swait(sem_sys_gen);
	if (SYS_pwf != (FILE *)NULL) {
		fclose(SYS_pwf);
		SYS_pwf = (FILE *)NULL;
	}
	if (_pw_db != (DBM *)0) {
		dbm_close(_pw_db);
		_pw_db = (DBM *)0;
		_pw_stayopen = 0;
	}
    ssignal(sem_sys_gen);
}

static char *
pwskip(p)
register char *p;
{
	while (*p && *p != ':' && *p != '\n')
		++p;
	if (*p)
		*p++ = 0;
	return(p);
}

struct passwd *
getpwent()
{
	register char *p;
    char *line = malloc(sizeof(char) * (BUFSIZ+1));
    struct passwd *ppasswd = (struct passwd *)malloc(sizeof(struct passwd));

    swait(sem_sys_gen);
	if (SYS_pwf == (FILE *)NULL) {
		if ((SYS_pwf = fopen( _pw_file, "r" )) == (FILE *)NULL)
			{ssignal(sem_sys_gen); return(0);}
	}
	p = fgets(line, BUFSIZ, SYS_pwf);
	if (p == NULL)
		{ssignal(sem_sys_gen); return(0);}
    ppasswd->pw_name = p;
	p = pwskip(p);
    ppasswd->pw_passwd = p;
	p = pwskip(p);
    ppasswd->pw_uid = atoi(p);
	p = pwskip(p);
    ppasswd->pw_gid = atoi(p);
    ppasswd->pw_quota = 0;
    ppasswd->pw_comment = SYS_EMPTY;
	p = pwskip(p);
    ppasswd->pw_gecos = p;
	p = pwskip(p);
    ppasswd->pw_dir = p;
	p = pwskip(p);
    ppasswd->pw_shell = p;
	while (*p && *p != '\n')
		p++;
	*p = '\0';
    ssignal(sem_sys_gen);
    return(ppasswd);
}

setpwfile(file)
	char *file;
{
    swait(sem_sys_gen);
	_pw_file = file;
    ssignal(sem_sys_gen);
}
