/****************************************
 *					*
 *    gethostent.c			*
 *    Modified for mutual exclusion	*
 *					*
 *    Static return areas malloc'ed	*
 *    (host, line)			*
 *    NOTE:  This area is not freed,	*
 *    but as someone has said --	*
 *    "Memory is cheap!"		*
 *					*
 *    SYS_ routines retained here.	*
 *					*
 ****************************************/
#include <kernel.h>
#undef NULL
char *malloc();

/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)gethostent.c	5.3 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ctype.h>
#include <ndbm.h>

/*
 * Internet version.
 */
#define	MAXALIASES	35
#define	MAXADDRSIZE	14

static FILE *hostf = (FILE *)NULL;
static char hostaddr[MAXADDRSIZE];
static char *host_aliases[MAXALIASES];
static char *host_addrs[] = {
	hostaddr,
	NULL
};

/*
 * The following is shared with gethostnamadr.c
 */
char	*_host_file = "/etc/hosts";
int	_host_stayopen;
DBM	*_host_db;	/* set by gethostbyname(), gethostbyaddr() */

static char *any();

sethostent(f)
	int f;
{
    swait(sem_sys_net);
	if (hostf != (FILE *)NULL)
		rewind(hostf);
	_host_stayopen |= f;
    ssignal(sem_sys_net);
}

SYS_sethostent(f)
	int f;
{
	if (hostf != (FILE *)NULL)
		rewind(hostf);
	_host_stayopen |= f;
}

endhostent()
{
    swait(sem_sys_net);
	if (hostf) {
		fclose(hostf);
		hostf = (FILE *)NULL;
	}
	if (_host_db) {
		dbm_close(_host_db);
		_host_db = (DBM *)NULL;
	}
	_host_stayopen = 0;
    ssignal(sem_sys_net);
}

SYS_endhostent()
{
	if (hostf) {
		fclose(hostf);
		hostf = (FILE *)NULL;
	}
	if (_host_db) {
		dbm_close(_host_db);
		_host_db = (DBM *)NULL;
	}
	_host_stayopen = 0;
}

struct hostent *
gethostent()
{
	char *p;
	register char *cp, **q;
    struct hostent *phost = (struct hostent *)malloc(sizeof(struct hostent));
    char *line = malloc(sizeof(char) * (BUFSIZ+1));

    swait(sem_sys_net);
	if (hostf == (FILE *)NULL && (hostf = fopen(_host_file, "r" )) == (FILE *)NULL)
		{ssignal(sem_sys_net); return ((struct hostent *)NULL);}
again:
	if ((p = fgets(line, BUFSIZ, hostf)) == NULL)
		{ssignal(sem_sys_net); return ((struct hostent *)NULL);}
	if (*p == '#')
		goto again;
	cp = any(p, "#\n");
	if (cp == NULL)
		goto again;
	*cp = '\0';
	cp = any(p, " \t");
	if (cp == NULL)
		goto again;
	*cp++ = '\0';
	/* THIS STUFF IS INTERNET SPECIFIC */
    phost->h_addr_list = host_addrs;
	*((u_long *)phost->h_addr) = inet_addr(p);
    phost->h_length = sizeof (u_long);
    phost->h_addrtype = AF_INET;
	while (*cp == ' ' || *cp == '\t')
		cp++;
    phost->h_name = cp;
	q = phost->h_aliases = host_aliases;
	cp = any(cp, " \t");
	if (cp != NULL) 
		*cp++ = '\0';
	while (cp && *cp) {
		if (*cp == ' ' || *cp == '\t') {
			cp++;
			continue;
		}
		if (q < &host_aliases[MAXALIASES - 1])
			*q++ = cp;
		cp = any(cp, " \t");
		if (cp != NULL)
			*cp++ = '\0';
	}
	*q = NULL;
    ssignal(sem_sys_net);
	return (phost);
}

struct hostent *
SYS_gethostent()
{
	char *p;
	register char *cp, **q;
    struct hostent *phost = (struct hostent *)malloc(sizeof(struct hostent));
    char *line = malloc(sizeof(char) * (BUFSIZ+1));

	if (hostf == (FILE *)NULL && (hostf = fopen(_host_file, "r" )) == (FILE *)NULL)
		return ((struct hostent *)NULL);
again:
	if ((p = fgets(line, BUFSIZ, hostf)) == NULL)
		return ((struct hostent *)NULL);
	if (*p == '#')
		goto again;
	cp = any(p, "#\n");
	if (cp == NULL)
		goto again;
	*cp = '\0';
	cp = any(p, " \t");
	if (cp == NULL)
		goto again;
	*cp++ = '\0';
	/* THIS STUFF IS INTERNET SPECIFIC */
    phost->h_addr_list = host_addrs;
	*((u_long *)phost->h_addr) = inet_addr(p);
    phost->h_length = sizeof (u_long);
    phost->h_addrtype = AF_INET;
	while (*cp == ' ' || *cp == '\t')
		cp++;
    phost->h_name = cp;
	q = phost->h_aliases = host_aliases;
	cp = any(cp, " \t");
	if (cp != NULL) 
		*cp++ = '\0';
	while (cp && *cp) {
		if (*cp == ' ' || *cp == '\t') {
			cp++;
			continue;
		}
		if (q < &host_aliases[MAXALIASES - 1])
			*q++ = cp;
		cp = any(cp, " \t");
		if (cp != NULL)
			*cp++ = '\0';
	}
	*q = NULL;
	return (phost);
}

sethostfile(file)
	char *file;
{
	_host_file = file;
}

static char *
any(cp, match)
	register char *cp;
	char *match;
{
	register char *mp, c;

	while (c = *cp) {
		for (mp = match; *mp; mp++)
			if (*mp == c)
				return (cp);
		cp++;
	}
	return ((char *)0);
}
