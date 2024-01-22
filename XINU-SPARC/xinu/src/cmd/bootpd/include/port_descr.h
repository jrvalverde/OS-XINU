
/* $Header: /.gwen/u5/smb/src/bootp/bootps/include/RCS/port_descr.h,v 1.3 90/07/12 09:58:54 smb Exp $ */

/* 
 * port_descr.h - definitions for port/socket descriptors
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Tue Jun 19 12:43:30 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	port_descr.h,v $
 * Revision 1.3  90/07/12  09:58:54  smb
 * Commented.
 * 
 * Revision 1.2  90/06/26  14:46:17  smb
 * Removed standalone parameter and realized that I only need to open 
 * the server socket, not the client.
 * 
 * Revision 1.1  90/06/19  14:25:13  smb
 * Initial revision
 * 
 */

/* external declarations of interesting values for ports */
extern int server_port_sd;
extern int client_port_num;
