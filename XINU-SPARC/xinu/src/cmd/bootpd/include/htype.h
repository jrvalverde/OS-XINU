
/* $Header: /.gwen/u5/smb/src/bootp/bootps/include/RCS/htype.h,v 1.2 90/07/12 09:58:44 smb Exp $ */

/* 
 * htype.h - hardware address types header file
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun 14 10:34:56 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	htype.h,v $
 * Revision 1.2  90/07/12  09:58:44  smb
 * Commented.
 * 
 * Revision 1.1  90/06/14  10:47:06  smb
 * Initial revision
 * 
 */

/* declarations for defining what the hardware types are */

extern char *htypes[];

#define NONE		0		/* no hardware address		*/
#define ETHER		1		/* 10 Mbps ethernet		*/
#define ETHERNET	2		/* 10 Mbps ethernet (alias)	*/

#define NUMHTYPES	3
