
/* $Header: /.gwen/u5/smb/src/bootp/bootps/src/RCS/xrealloc.c,v 1.2 90/07/12 10:51:32 smb Exp $ */

/* 
 * xrealloc.c - realloc with a test for success
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Wed Jun 20 16:23:57 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	xrealloc.c,v $
 * Revision 1.2  90/07/12  10:51:32  smb
 * Commented and declared register variables.
 * 
 * Revision 1.1  90/06/20  16:31:46  smb
 * Initial revision
 * 
 */

#include <syslog.h>

char *xrealloc(ptr, size)
     register char *ptr;
     int size;
{
    register char *retval;

    retval = (char *) realloc(ptr, size);
    if (retval) {
	return(retval);
    }
    syslog(LOG_ERR, "out of memory!");
    exit(1);
}
