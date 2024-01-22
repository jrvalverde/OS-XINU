/****************************************
 *					*
 *    getnetent.c			*
 *    Modified for mutual exclusion	*
 *					*
 *    Static return areas malloc'ed	*
 *    (net, line)			*
 *    NOTE:  This area is not freed,	*
 *    but as someone has said --	*
 *    "Memory is cheap!"		*
 *					*
 *    SYS_ routines defined.		*
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
static char sccsid[] = "@(#)getnetent.c	5.3 (Berkeley) 5/19/86";
#endif LIBC_SCCS and not lint

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ctype.h>

#define	MAXALIASES	35

static char NETDB[] = "/etc/networks";
static FILE *netf = (FILE *)NULL;
static char *net_aliases[MAXALIASES];
int _net_stayopen;
static char *any();

setnetent(f)
	int f;
{
    swait(sem_sys_net);
	if (netf == (FILE *)NULL)
		netf = fopen(NETDB, "r" );
	else
		rewind(netf);
	_net_stayopen |= f;
    ssignal(sem_sys_net);
}

SYS_setnetent(f)
	int f;
{
	if (netf == (FILE *)NULL)
		netf = fopen(NETDB, "r" );
	else
		rewind(netf);
	_net_stayopen |= f;
}

endnetent()
{
    swait(sem_sys_net);
	if (netf) {
		fclose(netf);
		netf = (FILE *)NULL;
	}
	_net_stayopen = 0;
    ssignal(sem_sys_net);
}

SYS_endnetent()
{
	if (netf) {
		fclose(netf);
		netf = (FILE *)NULL;
	}
	_net_stayopen = 0;
}

struct netent *
getnetent()
{
	char *p;
	register char *cp, **q;
    char *line = malloc(sizeof(char) * (BUFSIZ+1));
    struct netent *pnet = (struct netent *)malloc(sizeof(struct netent));

    swait(sem_sys_net);
	if (netf == (FILE *)NULL && (netf = fopen(NETDB, "r" )) == (FILE *)NULL)
		{ssignal(sem_sys_net); return ((struct netent *)NULL);}
again:
	p = fgets(line, BUFSIZ, netf);
	if (p == NULL)
		{ssignal(sem_sys_net); return ((struct netent *)NULL);}
	if (*p == '#')
		goto again;
	cp = any(p, "#\n");
	if (cp == NULL)
		goto again;
	*cp = '\0';
    pnet->n_name = p;
	cp = any(p, " \t");
	if (cp == NULL)
		goto again;
	*cp++ = '\0';
	while (*cp == ' ' || *cp == '\t')
		cp++;
	p = any(cp, " \t");
	if (p != NULL)
		*p++ = '\0';
    pnet->n_net = inet_network(cp);
    pnet->n_addrtype = AF_INET;
	q = pnet->n_aliases = net_aliases;
	if (p != NULL) 
		cp = p;
	while (cp && *cp) {
		if (*cp == ' ' || *cp == '\t') {
			cp++;
			continue;
		}
		if (q < &net_aliases[MAXALIASES - 1])
			*q++ = cp;
		cp = any(cp, " \t");
		if (cp != NULL)
			*cp++ = '\0';
	}
	*q = NULL;
    ssignal(sem_sys_net);
	return (pnet);
}

struct netent *
SYS_getnetent()
{
	char *p;
	register char *cp, **q;
    char *line = malloc(sizeof(char) * (BUFSIZ+1));
    struct netent *pnet = (struct netent *)malloc(sizeof(struct netent));

	if (netf == (FILE *)NULL && (netf = fopen(NETDB, "r" )) == (FILE *)NULL)
		return ((struct netent *)NULL);
again:
	p = fgets(line, BUFSIZ, netf);
	if (p == NULL)
		return ((struct netent *)NULL);
	if (*p == '#')
		goto again;
	cp = any(p, "#\n");
	if (cp == NULL)
		goto again;
	*cp = '\0';
    pnet->n_name = p;
	cp = any(p, " \t");
	if (cp == NULL)
		goto again;
	*cp++ = '\0';
	while (*cp == ' ' || *cp == '\t')
		cp++;
	p = any(cp, " \t");
	if (p != NULL)
		*p++ = '\0';
    pnet->n_net = inet_network(cp);
    pnet->n_addrtype = AF_INET;
	q = pnet->n_aliases = net_aliases;
	if (p != NULL) 
		cp = p;
	while (cp && *cp) {
		if (*cp == ' ' || *cp == '\t') {
			cp++;
			continue;
		}
		if (q < &net_aliases[MAXALIASES - 1])
			*q++ = cp;
		cp = any(cp, " \t");
		if (cp != NULL)
			*cp++ = '\0';
	}
	*q = NULL;
	return (pnet);
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
