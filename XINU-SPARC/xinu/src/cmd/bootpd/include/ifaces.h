
/* $Header: /.gwen/u5/smb/src/bootp/bootps/include/RCS/ifaces.h,v 1.3 90/07/12 09:58:46 smb Exp $ */

/* 
 * ifaces.h - structures to hold our interface configuration
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun 21 15:50:06 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	ifaces.h,v $
 * Revision 1.3  90/07/12  09:58:46  smb
 * Commented.
 * 
 * Revision 1.2  90/07/03  12:51:14  smb
 * Restructured directories to move all header files here.
 * 
 * Revision 1.1  90/06/21  16:56:09  smb
 * Initial revision
 * 
 */

#include <list.h>

/* extern declaration for list of my IP addresses */
extern struct list *myIPaddrs;
