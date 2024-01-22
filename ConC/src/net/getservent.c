/****************************************
 *					*
 *    getservent.c			*
 *    Modified for mutual exclusion	*
 *					*
 *    Static return areas malloc'ed	*
 *    (line, serv, serv_aliases)	*
 *    NOTE:  This area is not freed,	*
 *    but as someone has said --	*
 *    "Memory is cheap!"		*
 *					*
 *    SYS_ routines defined here.	*
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
static char sccsid[] = "@(#)getservent.c	5.3 (Berkeley) 5/19/86";
#endif LIBC_SCCS and not lint

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ctype.h>

#define	MAXALIASES	35

static char SERVDB[] = "/etc/services";
static FILE *servf = (FILE *)NULL;
static char *any();
int _serv_stayopen;

setservent(f)
	int f;
{
    swait(sem_sys_net);
	if (servf == (FILE *)NULL)
		servf = fopen(SERVDB, "r" );
	else
		rewind(servf);
	_serv_stayopen |= f;
    ssignal(sem_sys_net);
}

SYS_setservent(f)
	int f;
{
	if (servf == (FILE *)NULL)
		servf = fopen(SERVDB, "r" );
	else
		rewind(servf);
	_serv_stayopen |= f;
}

endservent()
{
    swait(sem_sys_net);
	if (servf) {
		fclose(servf);
		servf = (FILE *)NULL;
	}
	_serv_stayopen = 0;
    ssignal(sem_sys_net);
}

SYS_endservent()
{
	if (servf) {
		fclose(servf);
		servf = (FILE *)NULL;
	}
	_serv_stayopen = 0;
}

struct servent *
getservent()
{
	char *p;
	register char *cp, **q;
    char *line = malloc(sizeof(char) * (BUFSIZ+1));
    char **serv_aliases = (char **)malloc(sizeof(char *) * MAXALIASES);
    struct servent *pserv = (struct servent *)malloc(sizeof(struct servent));

    swait(sem_sys_net);
	if (servf == (FILE *)NULL && (servf = fopen(SERVDB, "r" )) == (FILE *)NULL)
		{ssignal(sem_sys_net); return ((struct servent *)NULL);}
again:
	if ((p = fgets(line, BUFSIZ, servf)) == NULL)
		{ssignal(sem_sys_net); return ((struct servent *)NULL);}
	if (*p == '#')
		goto again;
	cp = any(p, "#\n");
	if (cp == NULL)
		goto again;
	*cp = '\0';
    pserv->s_name = p;
	p = any(p, " \t");
	if (p == NULL)
		goto again;
	*p++ = '\0';
	while (*p == ' ' || *p == '\t')
		p++;
	cp = any(p, ",/");
	if (cp == NULL)
		goto again;
	*cp++ = '\0';
    pserv->s_port = htons((u_short)atoi(p));
    pserv->s_proto = cp;
	q = pserv->s_aliases = serv_aliases;
	cp = any(cp, " \t");
	if (cp != NULL)
		*cp++ = '\0';
	while (cp && *cp) {
		if (*cp == ' ' || *cp == '\t') {
			cp++;
			continue;
		}
		if (q < &serv_aliases[MAXALIASES - 1])
			*q++ = cp;
		cp = any(cp, " \t");
		if (cp != NULL)
			*cp++ = '\0';
	}
	*q = NULL;
    ssignal(sem_sys_net);
	return (pserv);
}

struct servent *
SYS_getservent()
{
	char *p;
	register char *cp, **q;
    char *line = malloc(sizeof(char) * (BUFSIZ+1));
    char **serv_aliases = (char **)malloc(sizeof(char *) * MAXALIASES);
    struct servent *pserv = (struct servent *)malloc(sizeof(struct servent));

	if (servf == (FILE *)NULL && (servf = fopen(SERVDB, "r" )) == (FILE *)NULL)
		return ((struct servent *)NULL);
again:
	if ((p = fgets(line, BUFSIZ, servf)) == NULL)
		return ((struct servent *)NULL);
	if (*p == '#')
		goto again;
	cp = any(p, "#\n");
	if (cp == NULL)
		goto again;
	*cp = '\0';
    pserv->s_name = p;
	p = any(p, " \t");
	if (p == NULL)
		goto again;
	*p++ = '\0';
	while (*p == ' ' || *p == '\t')
		p++;
	cp = any(p, ",/");
	if (cp == NULL)
		goto again;
	*cp++ = '\0';
    pserv->s_port = htons((u_short)atoi(p));
    pserv->s_proto = cp;
	q = pserv->s_aliases = serv_aliases;
	cp = any(cp, " \t");
	if (cp != NULL)
		*cp++ = '\0';
	while (cp && *cp) {
		if (*cp == ' ' || *cp == '\t') {
			cp++;
			continue;
		}
		if (q < &serv_aliases[MAXALIASES - 1])
			*q++ = cp;
		cp = any(cp, " \t");
		if (cp != NULL)
			*cp++ = '\0';
	}
	*q = NULL;
	return (pserv);
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
