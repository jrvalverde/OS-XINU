/************************************************
*						*
 *    These routines are used by the modified	*
 *    gen routines.  It is assumed that the	*
 *    callers of these routines have obtained	*
 *    mutual exclusion of whatever semaphore.	*
 *						*
 ************************************************/
char *malloc();

#include <fstab.h>
#include <stdio.h>
#include <ctype.h>
#include <grp.h>
#include <pwd.h>
#include <ndbm.h>
#include <strings.h>
#include <ttyent.h>

/************************************************
 *						*
 *    The routines SYS_setkey and		*
 *    SYS_encrypt should be here, but they	*
 *    are staying in crypt.c because of all	*
 *    the static global data used between	*
 *    the routines.				*
 *						*
 ************************************************/

/************************************************
 *						*
 *    SYS_setfsent, SYS_endfsent, SYS_getfsent	*
 *						*
 ************************************************/

extern FILE *SYS_fs_file;

SYS_setfsent()
{

	if (SYS_fs_file)
		SYS_endfsent();
	if ((SYS_fs_file = fopen(FSTAB, "r")) == (FILE *)NULL) {
		SYS_fs_file = 0;
		return (0);
	}
	return (1);
}

SYS_endfsent()
{

	if (SYS_fs_file) {
		fclose(SYS_fs_file);
		SYS_fs_file = 0;
	}
	return (1);
}

struct fstab *
SYS_getfsent()
{
	int nfields;
    struct fstab *pfs = (struct fstab *)malloc(sizeof(struct fstab));;

	if ((SYS_fs_file == 0) && (SYS_setfsent() == 0))
		return ((struct fstab *)0);
    nfields = fstabscan(pfs);
	if (nfields == EOF || nfields != 5)
		return ((struct fstab *)0);
    return (pfs);
}

/************************************************
 *						*
 *    SYS_setgrent, SYS_endgrent, SYS_getgrent	*
 *						*
 ************************************************/

#define	MAXGRP	200  /* IF THIS GETS CHANGED, MUST ALSO BE DONE IN getgrent.c */

extern char SYS_GROUP[];
extern FILE *SYS_grf;

SYS_setgrent()
{
	if( !SYS_grf )
		SYS_grf = fopen( SYS_GROUP, "r" );
	else
		rewind( SYS_grf );
}

SYS_endgrent()
{
	if( SYS_grf ){
		fclose( SYS_grf );
		SYS_grf = (FILE *)NULL;
	}
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
SYS_getgrent()
{
	register char *p, **q;
    char *line = malloc(sizeof(char) * (BUFSIZ+1));
    struct group *pgroup = (struct group *)malloc(sizeof(struct group));
    char **gr_mem = (char **)malloc(sizeof(char) * MAXGRP);

	if( !SYS_grf && !(SYS_grf = fopen( SYS_GROUP, "r" )) )
		return((struct group *)NULL);
	if( !(p = fgets( line, BUFSIZ, SYS_grf )) )
		return((struct group *)NULL);
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
    return( pgroup );
}

/************************************************
 *						*
 *    SYS_setpwent, SYS_endpwent, SYS_getpwent	*
 *						*
 ************************************************/

extern char	*SYS_EMPTY;
extern FILE	*SYS_pwf;
extern char	*_pw_file;
extern DBM	*_pw_db;
extern int	_pw_stayopen;

SYS_setpwent()
{
	if (SYS_pwf == (FILE *)NULL)
		SYS_pwf = fopen(_pw_file, "r");
	else
		rewind(SYS_pwf);
}

SYS_endpwent()
{
	if (SYS_pwf != (FILE *)NULL) {
		fclose(SYS_pwf);
		SYS_pwf = (FILE *)NULL;
	}
	if (_pw_db != (DBM *)0) {
		dbm_close(_pw_db);
		_pw_db = (DBM *)0;
		_pw_stayopen = 0;
	}
}

static char *
pwskip(p)
register char *p;
{
	while (*p && *p != ':' && *p != '\n')
		++p;
	if (*p)
		*p++ = 0;
	return(p);
}

struct passwd *
SYS_getpwent()
{
	register char *p;
    char *line = malloc(sizeof(char) * (BUFSIZ+1));
    struct passwd *ppasswd = (struct passwd *)malloc(sizeof(struct passwd));

	if (SYS_pwf == (FILE *)NULL) {
		if ((SYS_pwf = fopen( _pw_file, "r" )) == (FILE *)NULL)
			return(0);
	}
	p = fgets(line, BUFSIZ, SYS_pwf);
	if (p == NULL)
		return(0);
    ppasswd->pw_name = p;
	p = pwskip(p);
    ppasswd->pw_passwd = p;
	p = pwskip(p);
    ppasswd->pw_uid = atoi(p);
	p = pwskip(p);
    ppasswd->pw_gid = atoi(p);
    ppasswd->pw_quota = 0;
    ppasswd->pw_comment = SYS_EMPTY;
	p = pwskip(p);
    ppasswd->pw_gecos = p;
	p = pwskip(p);
    ppasswd->pw_dir = p;
	p = pwskip(p);
    ppasswd->pw_shell = p;
	while (*p && *p != '\n')
		p++;
	*p = '\0';
    return(ppasswd);
}

/************************************************
 *						*
 *  SYS_setttyent, SYS_endttyent, SYS_getttyent	*
 *						*
 ************************************************/


extern char SYS_TTYFILE[];
extern char SYS_zapchar;
extern FILE *SYS_tf;
#define LINE 256

SYS_setttyent()
{
	if (SYS_tf == (FILE *)NULL)
		SYS_tf = fopen(SYS_TTYFILE, "r");
	else
		rewind(SYS_tf);
}

SYS_endttyent()
{
	if (SYS_tf != (FILE *)NULL) {
		(void) fclose(SYS_tf);
		SYS_tf = (FILE *)NULL;
	}
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
SYS_getttyent()
{
	register char *p;
	register int c;
    char *line = malloc(sizeof(char) * LINE);
    struct ttyent *ptty = (struct ttyent *)malloc(sizeof(struct ttyent));

	if (SYS_tf == (FILE *)NULL) {
		if ((SYS_tf = fopen(SYS_TTYFILE, "r")) == (FILE *)NULL)
			return ((struct ttyent *)NULL);
	}
	do {
		p = fgets(line, LINE, SYS_tf);
		if (p == NULL)
			return ((struct ttyent *)NULL);
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
	return(ptty);
}

/************************************************
 *						*
 *    The routines SYS_malloc and		*
 *    SYS_free should be here, but they		*
 *    are staying in malloc.c because of all	*
 *    the static global data used between	*
 *    the routines.				*
 *						*
 ************************************************/

/************************************************
 *						*
 *    The routines SYS_srandom and		*
 *    SYS_random should be here, but they	*
 *    are staying in random.c because of all	*
 *    the static global data used between	*
 *    the routines.				*
 *						*
 ************************************************/

/************************************************
 *						*
 *    The routine SYS_openlog should be here,	*
 *    but it is staying in syslog.c because of	*
 *    all the static global data used between	*
 *    the routines.				*
 *						*
 ************************************************/
