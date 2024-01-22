
/* $Header: /.gwen/u5/smb/src/bootp/bootps/src/RCS/getport.c,v 1.3 90/07/12 10:50:10 smb Exp $ */

/* 
 * getport.c - get a port number
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Tue Jun 19 12:45:42 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	getport.c,v $
 * Revision 1.3  90/07/12  10:50:10  smb
 * Commented and declared register variables.
 * 
 * Revision 1.2  90/07/03  12:43:34  smb
 * Restructured directories to move header files to ../include
 * 
 * Revision 1.1  90/06/19  14:24:24  smb
 * Initial revision
 * 
 */

#include <netdb.h>
#include <syslog.h>
#include <null.h>

int getport(name, number)
     register char *name;
     register int number;
{
    register struct servent *service;
    
    if ((service = getservbyname(name, "udp")) == NULL) {
	syslog(LOG_WARNING,
	       "service \"udp/%s\" not located in /etc/services",
		name);
	syslog(LOG_WARNING, "using default value of \"udp/%d\"", number);
	return(number);
    }
    return(service->s_port);
}
