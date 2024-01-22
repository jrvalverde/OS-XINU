/****************************************
 *					*
 *    getpwnamuid.c			*
 *    Modified for mutual exclusion	*
 *					*
 *    Static return areas malloc'ed	*
 *    (line and passwd)			*
 *    NOTE:  This area is not freed,	*
 *    but as someone has said --	*
 *    "Memory is cheap!"		*
 *					*
 ****************************************/
#include <kernel.h>
#undef NULL
char *malloc();
struct passwd *SYS_getpwent();

/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)getpwnamuid.c	5.2 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

#include <stdio.h>
#include <pwd.h>
#include <ndbm.h>

#include <sys/file.h>

/*
 * The following are shared with getpwent.c
 */
extern	char *_pw_file;
DBM	*_pw_db;
int	_pw_stayopen;

static struct passwd *
fetchpw(key)
	datum key;
{
        register char *cp, *tp;
    char *line = malloc(sizeof(char) * (BUFSIZ+1));
    struct passwd *ppasswd = (struct passwd *)malloc(sizeof(struct passwd));

    swait(sem_sys_gen);
        if (key.dptr == 0)
                {ssignal(sem_sys_gen); return ((struct passwd *)NULL);}
	key = dbm_fetch(_pw_db, key);
	if (key.dptr == 0)
                {ssignal(sem_sys_gen); return ((struct passwd *)NULL);}
        cp = key.dptr;
	tp = line;

#define	EXPAND(e)	ppasswd->pw_/**/e = tp; while (*tp++ = *cp++);
	EXPAND(name);
	EXPAND(passwd);
	bcopy(cp, (char *)&ppasswd->pw_uid, sizeof (int));
	cp += sizeof (int);
	bcopy(cp, (char *)&ppasswd->pw_gid, sizeof (int));
	cp += sizeof (int);
	bcopy(cp, (char *)&ppasswd->pw_quota, sizeof (int));
	cp += sizeof (int);
	EXPAND(comment);
	EXPAND(gecos);
	EXPAND(dir);
	EXPAND(shell);
    ssignal(sem_sys_gen);
    return (ppasswd);
}

struct passwd *
getpwnam(nam)
	char *nam;
{
        datum key;
	register struct passwd *pw;

    swait(sem_sys_gen);
        if (_pw_db == (DBM *)0 &&
	    (_pw_db = dbm_open(_pw_file, O_RDONLY)) == (DBM *)0) {
	oldcode:
		SYS_setpwent();
		while ((pw = SYS_getpwent()) && strcmp(nam, pw->pw_name))
			;
		if (!_pw_stayopen)
			SYS_endpwent();
		{ssignal(sem_sys_gen); return (pw);}
	}
	if (flock(dbm_dirfno(_pw_db), LOCK_SH) < 0) {
		dbm_close(_pw_db);
		_pw_db = (DBM *)0;
		goto oldcode;
	}
        key.dptr = nam;
        key.dsize = strlen(nam);
	pw = fetchpw(key);
	(void) flock(dbm_dirfno(_pw_db), LOCK_UN);
	if (!_pw_stayopen) {
		dbm_close(_pw_db);
		_pw_db = (DBM *)0;
	}
    ssignal(sem_sys_gen);
        return (pw);
}

struct passwd *
getpwuid(uid)
	int uid;
{
        datum key;
	register struct passwd *pw;

    swait(sem_sys_gen);
        if (_pw_db == (DBM *)0 &&
	    (_pw_db = dbm_open(_pw_file, O_RDONLY)) == (DBM *)0) {
	oldcode:
		SYS_setpwent();
		while ((pw = SYS_getpwent()) && pw->pw_uid != uid)
			;
		if (!_pw_stayopen)
			SYS_endpwent();
		{ssignal(sem_sys_gen); return (pw);}
	}
	if (flock(dbm_dirfno(_pw_db), LOCK_SH) < 0) {
		dbm_close(_pw_db);
		_pw_db = (DBM *)0;
		goto oldcode;
	}
        key.dptr = (char *) &uid;
        key.dsize = sizeof uid;
	pw = fetchpw(key);
	(void) flock(dbm_dirfno(_pw_db), LOCK_UN);
	if (!_pw_stayopen) {
		dbm_close(_pw_db);
		_pw_db = (DBM *)0;
	}
    ssignal(sem_sys_gen);
        return (pw);
}
