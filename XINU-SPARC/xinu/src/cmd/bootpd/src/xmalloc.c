
/* $Header: /.gwen/u5/smb/src/bootp/bootps/src/RCS/xmalloc.c,v 1.2 90/07/12 10:51:29 smb Exp $ */

/* 
 * xmalloc.c - malloc with a check of returned value
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun 14 10:32:30 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	xmalloc.c,v $
 * Revision 1.2  90/07/12  10:51:29  smb
 * Commented and declared register variables.
 * 
 * Revision 1.1  90/06/14  10:47:51  smb
 * Initial revision
 * 
 */

#include <syslog.h>

char *xmalloc(size)
     int size;
{
    register char *retval;

    retval = (char *) malloc(size);
    if (retval) {
	return(retval);
    }
    syslog(LOG_ERR, "out of memory!");
    exit(1);
}
