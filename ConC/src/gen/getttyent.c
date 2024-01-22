/****************************************
 *					*
 *    getttyent.c			*
 *    Modified for mutual exclusion	*
 *					*
 *    Static return areas malloc'ed	*
 *    (line and tty)			*
 *    NOTE:  This area is not freed,	*
 *    but as someone has said --	*
 *    "Memory is cheap!"		*
 *					*
 *    Static globals TTYFILE, zapchar,	*
 *    and tf changed to SYS_* to make	*
 *    accessible from SYS_ routines.	*
 *					*
 ****************************************/
#include <kernel.h>
#undef NULL
char *malloc();

/*
 * Copyright (c) 1985 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)getttyent.c	5.4 (Berkeley) 5/19/86";
#endif LIBC_SCCS and not lint

#include <stdio.h>
#include <strings.h>
#include <ttyent.h>

char SYS_TTYFILE[] = "/etc/ttys";
char SYS_zapchar;
FILE *SYS_tf = (FILE *)NULL;
#define LINE 256

setttyent()
{
    swait(sem_sys_gen);
	if (SYS_tf == (FILE *)NULL)
		SYS_tf = fopen(SYS_TTYFILE, "r");
	else
		rewind(SYS_tf);
    ssignal(sem_sys_gen);
}

endttyent()
{
    swait(sem_sys_gen);
	if (SYS_tf != (FILE *)NULL) {
		(void) fclose(SYS_tf);
		SYS_tf = (FILE *)NULL;
	}
    ssignal(sem_sys_gen);
}

#define QUOTED	1

/*
 * Skip over the current field, removing quotes,
 * and return a pointer to the next field.
 */
static char *
skip(p)
	register char *p;
{
	register char *t = p;
	register int c;
	register int q = 0;

	for (; (c = *p) != '\0'; p++) {
		if (c == '"') {
			q ^= QUOTED;	/* obscure, but nice */
			continue;
		}
		if (q == QUOTED && *p == '\\' && *(p+1) == '"')
			p++;
		*t++ = *p;
		if (q == QUOTED)
			continue;
		if (c == '#') {
			SYS_zapchar = c;
			*p = 0;
			break;
		}
		if (c == '\t' || c == ' ' || c == '\n') {
			SYS_zapchar = c;
			*p++ = 0;
			while ((c = *p) == '\t' || c == ' ' || c == '\n')
				p++;
			break;
		}
	}
	*--t = '\0';
	return (p);
}

static char *
value(p)
	register char *p;
{
	if ((p = index(p,'=')) == 0)
		return(NULL);
	p++;			/* get past the = sign */
	return(p);
}

struct ttyent *
getttyent()
{
	register char *p;
	register int c;
    char *line = malloc(sizeof(char) * LINE);
    struct ttyent *ptty = (struct ttyent *)malloc(sizeof(struct ttyent));

    swait(sem_sys_gen);
	if (SYS_tf == (FILE *)NULL) {
		if ((SYS_tf = fopen(SYS_TTYFILE, "r")) == (FILE *)NULL)
			{ssignal(sem_sys_gen); return ((struct ttyent *)NULL);}
	}
	do {
		p = fgets(line, LINE, SYS_tf);
		if (p == NULL)
			{ssignal(sem_sys_gen); return ((struct ttyent *)NULL);}
		while ((c = *p) == '\t' || c == ' ' || c == '\n')
			p++;
	} while (c == '\0' || c == '#');
	SYS_zapchar = 0;
    ptty->ty_name = p;
	p = skip(p);
    ptty->ty_getty = p;
	p = skip(p);
    ptty->ty_type = p;
	p = skip(p);
    ptty->ty_status = 0;
    ptty->ty_window = NULL;
	for (; *p; p = skip(p)) {
#define space(x) ((c = p[x]) == ' ' || c == '\t' || c == '\n')
		if (strncmp(p, "on", 2) == 0 && space(2))
			ptty->ty_status |= TTY_ON;
		else if (strncmp(p, "off", 3) == 0 && space(3))
			ptty->ty_status &= ~TTY_ON;
		else if (strncmp(p, "secure", 6) == 0 && space(6))
			ptty->ty_status |= TTY_SECURE;
		else if (strncmp(p, "window=", 7) == 0)
			ptty->ty_window = value(p);
		else
			break;
	}
	if (SYS_zapchar == '#' || *p == '#')
		while ((c = *++p) == ' ' || c == '\t')
			;
    ptty->ty_comment = p;
	if (*p == 0)
		ptty->ty_comment = 0;
	if (p = index(p, '\n'))
		*p = '\0';
    ssignal(sem_sys_gen);
	return(ptty);
}
