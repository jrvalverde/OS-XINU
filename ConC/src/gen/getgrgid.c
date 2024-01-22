/****************************************
 *					*
 *    getgrgid.c			*
 *    Modified for mutual exclusion	*
 *					*
 ****************************************/
#include <kernel.h>
#undef NULL

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)getgrgid.c	5.2 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

#include <grp.h>

struct group *
getgrgid(gid)
register gid;
{
	register struct group *p;
	struct group *SYS_getgrent();

    swait(sem_sys_gen);
	SYS_setgrent();
	while( (p = SYS_getgrent()) && p->gr_gid != gid );
	SYS_endgrent();
    ssignal(sem_sys_gen);
	return(p);
}
