
/* $Header: /.gwen/u5/smb/src/bootp/bootps/include/RCS/args.h,v 1.3 90/07/12 09:58:23 smb Exp $ */

/* 
 * args.h - definitions for stuff that comes from argument vector
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Jun 15 15:00:22 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	args.h,v $
 * Revision 1.3  90/07/12  09:58:23  smb
 * Commented.
 * 
 * Revision 1.2  90/06/26  14:44:35  smb
 * Removed standalone parameter and -s flag
 * 
 * Revision 1.1  90/06/19  14:27:19  smb
 * Initial revision
 * 
 */

/*
 * define the default configuration file name - this will have the bootp
 * directory name prepended to it
 */
#define DEF_CONFIG	"bootpd.conf"

/* place to store the config file name */
extern char *config;
