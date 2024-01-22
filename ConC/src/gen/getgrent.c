/****************************************
 *					*
 *    getgrent.c			*
 *    Modified for mutual exclusion	*
 *					*
 *    Static return areas malloc'ed	*
 *    (line, group, gr_mem)		*
 *    NOTE:  This area is not freed,	*
 *    but as someone has said --	*
 *    "Memory is cheap!"		*
 *					*
 *    Static globlas GROUP and grf	*
 *    changed into regular globals	*
 *    SYS_GROUP and SYS_grf to make	*
 *    accessible to SYS_ routines.	*
 *					*
 ****************************************/
#include <kernel.h>
#undef NULL
char *malloc();

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)getgrent.c	5.2 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

#include <stdio.h>
#include <grp.h>

#define	MAXGRP	200

static char SYS_GROUP[] = "/etc/group";
static FILE *SYS_grf = (FILE *)NULL;

setgrent()
{
    swait(sem_sys_gen);
	if( !SYS_grf )
		SYS_grf = fopen( SYS_GROUP, "r" );
	else
		rewind( SYS_grf );
    ssignal(sem_sys_gen);
}

endgrent()
{
    swait(sem_sys_gen);
	if( SYS_grf ){
		fclose( SYS_grf );
		SYS_grf = (FILE *)NULL;
	}
    ssignal(sem_sys_gen);
}

static char *
grskip(p,c)
register char *p;
register c;
{
	while( *p && *p != c ) ++p;
	if( *p ) *p++ = 0;
	return( p );
}

struct group *
getgrent()
{
	register char *p, **q;
    char *line = malloc(sizeof(char) * (BUFSIZ+1));
    struct group *pgroup = (struct group *)malloc(sizeof(struct group));
    char **gr_mem = (char **)malloc(sizeof(char) * MAXGRP);

    swait(sem_sys_gen);
	if( !SYS_grf && !(SYS_grf = fopen( SYS_GROUP, "r" )) )
		{ssignal(sem_sys_gen); return((struct group *)NULL);}
	if( !(p = fgets( line, BUFSIZ, SYS_grf )) )
		{ssignal(sem_sys_gen); return((struct group *)NULL);}
    pgroup->gr_name = p;
    pgroup->gr_passwd = p = grskip(p,':');
    pgroup->gr_gid = atoi( p = grskip(p,':') );
    pgroup->gr_mem = gr_mem;
	p = grskip(p,':');
	grskip(p,'\n');
	q = gr_mem;
	while( *p ){
		if (q < &gr_mem[MAXGRP-1])
			*q++ = p;
		p = grskip(p,',');
	}
	*q = NULL;
    ssignal(sem_sys_gen);
    return( pgroup );
}
