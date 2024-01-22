/****************************************
 *					*
 *    getgrnam.c			*
 *    Modified for mutual exclusion	*
 *					*
 ****************************************/
#include <kernel.h>
#undef NULL

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)getgrnam.c	5.2 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

#include <grp.h>

struct group *
getgrnam(name)
register char *name;
{
	register struct group *p;
	struct group *SYS_getgrent();

    swait(sem_sys_gen);
	SYS_setgrent();
	while( (p = SYS_getgrent()) && strcmp(p->gr_name,name) );
	SYS_endgrent();
    ssignal(sem_sys_gen);
	return(p);
}
