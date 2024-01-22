
/* $Header: /usr/src/local/etc/bootpd/include/RCS/tags.h,v 1.3 90/07/27 10:35:12 trinkle Exp $ */

/* 
 * tags.h - definitions of field tags for bootp configuration file
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun 14 10:25:35 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	tags.h,v $
 * Revision 1.3  90/07/27  10:35:12  trinkle
 * Xinu additions from smb.
 * 
 * Revision 1.2  90/07/12  09:59:00  smb
 * Commented.
 * 
 * Revision 1.1  90/06/14  10:47:31  smb
 * Initial revision
 * 
 */


extern char *tags[];

#define BOOTFILE	0		/* Tag: bf */
#define GENERIC		1		/* Tag: gn */
#define GATEWAY		2		/* Tag: gw */
#define HADDR		3		/* Tag: ha */
#define HOMEDIR		4		/* Tag: hd */
#define HTYPE		5		/* Tag: ht */
#define IPADDR		6		/* Tag: ip */
#define LPR_SERVER	7		/* Tag: ls */
#define MACHINE		8		/* Tag: mt */
#define NETMASK		9		/* Tag: nm */
#define DOMAIN_SERVER	10		/* Tag: ns */
#define RLP_SERVER	11		/* Tag: rs */
#define TEMPLATE	12		/* Tag: tp */
#define TIME_OFFSET	13		/* Tag: to */
#define TIME_SERVER	14		/* Tag: ts */

/* begin xinu additions */
#define CNETMASK	15		/* Tag: cnm */
#define CIPADDR		16		/* Tag: cip */
#define CIMPLETS	17		/* Tag: cim */

#define NUMTAGS		18
