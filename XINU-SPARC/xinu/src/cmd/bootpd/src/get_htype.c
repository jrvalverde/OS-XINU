
/* $Header: /.gwen/u5/smb/src/bootp/bootps/src/RCS/get_htype.c,v 1.3 90/07/12 10:50:00 smb Exp $ */

/* 
 * get_htype.c - determine which hardware type was specified
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun 14 10:28:41 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	get_htype.c,v $
 * Revision 1.3  90/07/12  10:50:00  smb
 * Commented and declared register variables.
 * 
 * Revision 1.2  90/07/03  12:43:28  smb
 * Restructured directories to move header files to ../include
 * 
 * Revision 1.1  90/06/14  10:46:06  smb
 * Initial revision
 * 
 */

#include <htype.h>

char *htypes[] = {
    "",			/* no type		*/
    "ether",		/* type ether (alias)	*/
    "ethernet"		/* type ethernet (alias)*/
    };

int get_htype(value)
     register char *value;
{
    register int i;
    register retval;

    retval = -1;
    if (value) {
	for (i=0; i<NUMHTYPES; i++) {
	    if (strcmp(value, htypes[i]) == 0) {
		retval = i;
		break;
	    }
	}
    }
    return(retval); 
}
