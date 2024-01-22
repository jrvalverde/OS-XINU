
/* $Header: /.gwen/u5/smb/src/bootp/bootps/include/RCS/sizes.h,v 1.4 90/07/12 09:58:57 smb Exp $ */

/* 
 * sizes.h - define various maximum sizes for bootpd
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun 14 11:58:09 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	sizes.h,v $
 * Revision 1.4  90/07/12  09:58:57  smb
 * Commented.
 * 
 * Revision 1.3  90/06/26  14:49:08  smb
 * Added a few new sizes.
 * 
 * Revision 1.2  90/06/20  16:34:44  smb
 * Added sizes of IP and Ethernet addresses when represented as ascii
 * text as well as binary octets.
 * Added size of a maximum- (normal-) sized bootp message.
 * 
 * Revision 1.1  90/06/14  16:51:34  smb
 * Initial revision
 * 
 */

#ifndef SIZES_H
#define SIZES_H

#define SZMAXLINE	132	/* maximum size of a line in conf  */
#define SZMAXTOKEN	32	/* maximum size of a token	   */
#define SZMAXHADDR	16	/* maximum size of a hw addr	   */
#define SZMAXMSG	300	/* maximum size of a bootp message */
#define SZIPTEXT	16	/* size of an IP address in ascii  */
#define SZIPADDR	4	/* size of an IP address in bytes  */
#define SZENTEXT	18	/* size of Ethernet addr in ascii  */
#define SZENADDR	6	/* size of Ethernet addr in bytes  */
#define SZBLOCK		512	/* octets in a block for file size */

#endif
