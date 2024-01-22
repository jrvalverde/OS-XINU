
/* $Header: /.gwen/u5/smb/src/bootp/bootps/src/RCS/quit.c,v 1.3 90/07/03 12:45:05 smb Exp $ */

/* 
 * quit.c - end processing and shutdown
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Tue Jun 19 14:07:52 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	quit.c,v $
 * Revision 1.3  90/07/03  12:45:05  smb
 * Restructured directories to move header files to ../include
 * 
 * Revision 1.2  90/06/26  14:47:57  smb
 * Changed to reflect the presence of only one socket.
 * 
 * Revision 1.1  90/06/19  14:26:09  smb
 * Initial revision
 * 
 */

#include <syslog.h>
#include <signal.h>
#include <port_descr.h>

void quit()
{
    syslog(LOG_INFO, "going down on signal %d", SIGTERM);
    close(server_port_sd);
    exit(0);
}
