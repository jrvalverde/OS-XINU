/****************************************
 *					*
 *    getprotoent.c			*
 *    Modified for mutual exclusion	*
 *					*
 *    Static return areas malloc'ed	*
 *    (proto, line, proto_aliases)	*
 *    NOTE:  This area is not freed,	*
 *    but as someone has said --	*
 *    "Memory is cheap!"		*
 *					*
 *    SYS_ routined defined.		*
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
static char sccsid[] = "@(#)getprotoent.c	5.3 (Berkeley) 5/19/86";
#endif LIBC_SCCS and not lint

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ctype.h>

#define	MAXALIASES	35

static char PROTODB[] = "/etc/protocols";
static FILE *protof = (FILE *)NULL;
static char *any();
int _proto_stayopen;

setprotoent(f)
	int f;
{
    swait(sem_sys_net);
	if (protof == (FILE *)NULL)
		protof = fopen(PROTODB, "r" );
	else
		rewind(protof);
	_proto_stayopen |= f;
    ssignal(sem_sys_net);
}

SYS_setprotoent(f)
	int f;
{
	if (protof == (FILE *)NULL)
		protof = fopen(PROTODB, "r" );
	else
		rewind(protof);
	_proto_stayopen |= f;
}

endprotoent()
{
    swait(sem_sys_net);
	if (protof) {
		fclose(protof);
		protof = (FILE *)NULL;
	}
	_proto_stayopen = 0;
    ssignal(sem_sys_net);
}

SYS_endprotoent()
{
	if (protof) {
		fclose(protof);
		protof = (FILE *)NULL;
	}
	_proto_stayopen = 0;
}

struct protoent *
getprotoent()
{
	char *p;
	register char *cp, **q;
    char *line = malloc(sizeof(char) * (BUFSIZ+1));
    char **proto_aliases = (char **)malloc(sizeof(char *) * MAXALIASES);
    struct protoent *pproto = (struct protoent *)malloc(sizeof(struct protoent));

    swait(sem_sys_net);
	if (protof == (FILE *)NULL && (protof = fopen(PROTODB, "r" )) == (FILE *)NULL)
		{ssignal(sem_sys_net); return ((struct protoent *)NULL);}
again:
	if ((p = fgets(line, BUFSIZ, protof)) == NULL)
		{ssignal(sem_sys_net); return ((struct protoent *)NULL);}
	if (*p == '#')
		goto again;
	cp = any(p, "#\n");
	if (cp == NULL)
		goto again;
	*cp = '\0';
    pproto->p_name = p;
	cp = any(p, " \t");
	if (cp == NULL)
		goto again;
	*cp++ = '\0';
	while (*cp == ' ' || *cp == '\t')
		cp++;
	p = any(cp, " \t");
	if (p != NULL)
		*p++ = '\0';
    pproto->p_proto = atoi(cp);
	q = pproto->p_aliases = proto_aliases;
	if (p != NULL) {
		cp = p;
		while (cp && *cp) {
			if (*cp == ' ' || *cp == '\t') {
				cp++;
				continue;
			}
			if (q < &proto_aliases[MAXALIASES - 1])
				*q++ = cp;
			cp = any(cp, " \t");
			if (cp != NULL)
				*cp++ = '\0';
		}
	}
	*q = NULL;
    ssignal(sem_sys_net);
	return (pproto);
}

struct protoent *
SYS_getprotoent()
{
	char *p;
	register char *cp, **q;
    char *line = malloc(sizeof(char) * (BUFSIZ+1));
    char **proto_aliases = (char **)malloc(sizeof(char *) * MAXALIASES);
    struct protoent *pproto = (struct protoent *)malloc(sizeof(struct protoent));

	if (protof == (FILE *)NULL && (protof = fopen(PROTODB, "r" )) == (FILE *)NULL)
		return ((struct protoent *)NULL);
again:
	if ((p = fgets(line, BUFSIZ, protof)) == NULL)
		return ((struct protoent *)NULL);
	if (*p == '#')
		goto again;
	cp = any(p, "#\n");
	if (cp == NULL)
		goto again;
	*cp = '\0';
    pproto->p_name = p;
	cp = any(p, " \t");
	if (cp == NULL)
		goto again;
	*cp++ = '\0';
	while (*cp == ' ' || *cp == '\t')
		cp++;
	p = any(cp, " \t");
	if (p != NULL)
		*p++ = '\0';
    pproto->p_proto = atoi(cp);
	q = pproto->p_aliases = proto_aliases;
	if (p != NULL) {
		cp = p;
		while (cp && *cp) {
			if (*cp == ' ' || *cp == '\t') {
				cp++;
				continue;
			}
			if (q < &proto_aliases[MAXALIASES - 1])
				*q++ = cp;
			cp = any(cp, " \t");
			if (cp != NULL)
				*cp++ = '\0';
		}
	}
	*q = NULL;
	return (pproto);
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
