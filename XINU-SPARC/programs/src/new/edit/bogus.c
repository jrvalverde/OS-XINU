
/* 
 * bogus.c - unix system calls that xinu does not support
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Wed Jun 27 13:06:24 1990
 *
 * Copyright (c) 1990 Jim Griffioen
 */

#include <stdio.h>
#include <varargs.h>

#ifndef UNIX	/* compiles as xinu process */
extern char libwarning;
#else		/* compile as unix process, not xinu */
char libwarning = 0;
xcontrol(){return(1);}
#endif

/* bogus calls - hopefully not needed */
#ifndef UNIX
stty()
{
	if (libwarning)
	    printf("calling stty - not implemented at all\n");
	return(-1);
}


gtty(fd, buf)
int fd;
char *buf;
{
	if ((fd < 0) || (fd > 2)) {
		printf("called gtty on non-console device\n");
		return(-1);
	}
	if (libwarning)
	    printf("calling gtty - not implemented correctly\n");
/*
	buf->sg_ispeed = buf->sg_ospeed = 13;
	buf->sg_erase = '\h';
	buf->sg_kill = 21;
	buf->sg_flags = 216;
*/
	return(-1);
}

sigsetmask()
{
	if (libwarning)
	    printf("calling sigsetmask - not implemented at all\n");
	return(0);
}

sigblock()
{
	if (libwarning)
	    printf("calling sigblock - not implemented at all\n");
	return(0);
}

#ifndef NORMAL_CLIB

isatty(fd)
int fd;
{
	if (libwarning)
	    fprintf(stderr, "isatty not implemented correctly\n");
	
	if (readdtable(fd) == xdvlookup("CONSOLE"))
	    return(1);
	else
	    return(0);
}

char *vsprintf(str, fmt, ap)
char *str, *fmt;
va_list ap;
{
	int     bogus_sprntf();
        char    *s;

        s = str;
        _doprnt(fmt, ap, bogus_sprntf, &s);
        *s++ = '\0';

        return(str);
}

static	bogus_sprntf(cpp, c)
        char    **cpp;
        char    c;
{
        return(*(*cpp)++ = c);
}

char regexpression[256];

char *re_comp(sb)
char *sb;
{
	if (libwarning)
	    fprintf(stderr, "re_comp not correctly implemented\n");
	
	if ((sb == (char *)0) || (sb == NULL))
	    return;
	strcpy(regexpression, sb);
	return(NULL);
}

re_exec(sb)
char *sb;
{
	int status;
	
	if (libwarning)
	    fprintf(stderr, "re_exec not correctly implemented\n");

	status = strindex(sb, regexpression);
	if ( status != (-1) )
	    return(1);
	else
	    return(0);
}

strindex(str, substr)
char *str, *substr;
{
	int len;
	char *sb;

	len = strlen(substr);
	sb = str;
	while (*sb) {
		if (strncmp(sb, substr, len) == 0)
		    return(sb-str);
		sb++;
	}
	return(-1);
}

#endif

#endif

dbg(fmt, s1, s2, s3, s4 ,s5, s6)
char *fmt;
int s1, s2, s3, s4 ,s5, s6;
{
	if (libwarning) {
		printf(fmt, s1, s2, s3, s4 ,s5, s6);
/*		sleep(1);*/
	}
}



