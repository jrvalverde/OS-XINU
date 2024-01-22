
/* $Header: /usr/src/local/etc/bootpd/src/RCS/findtag.c,v 1.4 90/07/27 10:32:52 trinkle Exp $ */

/* 
 * findtag.c - determine which tag is selected
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun 14 10:37:40 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	findtag.c,v $
 * Revision 1.4  90/07/27  10:32:52  trinkle
 * Xinu additions from smb.
 * 
 * Revision 1.3  90/07/12  10:49:51  smb
 * Commented and declared register variables.
 * 
 * Revision 1.2  90/07/03  12:43:23  smb
 * Restructured directories to move header files to ../include
 * 
 * Revision 1.1  90/06/14  10:45:39  smb
 * Initial revision
 * 
 */

#include <tags.h>

/* character values for tags */
char *tags[] = {
    "bf",		/* Bootfile		*/
    "gn",		/* Generic image name	*/
    "gw",		/* Gateway		*/
    "ha",		/* Hardware address	*/
    "hd",		/* Home directory	*/
    "ht",		/* Hardware type	*/
    "ip",		/* IP address		*/
    "ls",		/* LPR server		*/
    "mt",		/* Machine type		*/
    "nm",		/* Subnet mask		*/
    "ns",		/* DNS server		*/
    "rs",		/* RLP server		*/
    "tp",		/* Template name	*/
    "to",		/* Time offset		*/
    "ts",		/* Time server		*/

/* begin Xinu additions */
    "cnm",		/* Cypress subnet mask	*/
    "cip",		/* Cypress IP address	*/
    "cim",		/* Cypress implet list	*/
    };

int findtag(value)
     register char *value;
{
    register int i;
    int retval;

    retval = -1;
    if (value) {
	for (i=0; i<NUMTAGS; i++) {
	    if (strcmp(value, tags[i]) == 0) {
		retval  = i;
		break;
	    }
	}
    }
    return(retval);
}
