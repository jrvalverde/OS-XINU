/****************************************
 *					*
 *    getlogin.c			*
 *					*
 *    Static return areas malloc'ed	*
 *    NOTE:  This area is not freed,	*
 *    but as someone has said --	*
 *    "Memory is cheap!"		*
 *					*
 ****************************************/
#include <kernel.h>
#undef NULL
char *malloc();

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)getlogin.c	5.3 (Berkeley) 5/9/86";
#endif LIBC_SCCS and not lint

#include <utmp.h>

static	char UTMP[]	= "/etc/utmp";

char *
getlogin()
{
	register int me, uf;
	register char *cp;
    struct utmp *pubuf = (struct utmp *)malloc(sizeof(struct utmp));

	if (!(me = ttyslot()))
		return(0);
	if ((uf = open(UTMP, 0)) < 0)
		return (0);
	lseek (uf, (long)(me*sizeof(*pubuf)), 0);
	if (read(uf, (char *)pubuf, sizeof (*pubuf)) != sizeof (*pubuf)) {
		close(uf);
		return (0);
	}
	close(uf);
	if (pubuf->ut_name[0] == '\0')
		return (0);
	pubuf->ut_name[sizeof (pubuf->ut_name)] = ' ';
	for (cp = pubuf->ut_name; *cp++ != ' '; )
		;
	*--cp = '\0';
	return (pubuf->ut_name);
}
