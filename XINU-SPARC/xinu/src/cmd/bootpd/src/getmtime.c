
/* $Header: /.gwen/u5/smb/src/bootp/bootps/src/RCS/getmtime.c,v 1.3 90/07/12 10:50:07 smb Exp $ */

/* 
 * getmtime.c - get the last modification time for a file by name
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Tue Jun 19 14:38:10 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	getmtime.c,v $
 * Revision 1.3  90/07/12  10:50:07  smb
 * Commented and declared register variables.
 * 
 * Revision 1.2  90/06/20  16:30:27  smb
 * Corrected a small bug (undeclared variable).
 * 
 * Revision 1.1  90/06/19  14:42:33  smb
 * Initial revision
 * 
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

long getmtime(name)
     register char *name;
{
    register struct stat buf;
    
    bzero(&buf, sizeof(struct stat));
    if (stat(name, &buf) < 0) {
	syslog(LOG_ERR, "stat: %m");
	return(-1);
    }
    return((long) buf.st_mtime);
}
