
/* $Header: /.gwen/u5/smb/src/bootp/bootps/src/RCS/hash.c,v 1.4 90/07/12 10:50:13 smb Exp $ */

/* 
 * hash.c - generic hash function
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun 14 10:29:58 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	hash.c,v $
 * Revision 1.4  90/07/12  10:50:13  smb
 * Commented and declared register variables.
 * 
 * Revision 1.3  90/07/03  12:43:37  smb
 * Restructured directories to move header files to ../include
 * 
 * Revision 1.2  90/06/14  16:46:51  smb
 * Fixed a few small bugs in the hash functions such as not starting
 * retval at 0.
 * 
 * Revision 1.1  90/06/14  10:46:28  smb
 * Initial revision
 * 
 */

#include <hash.h>

unsigned hash(string, length)
     register char *string;
     int length;
{
    int i;
    register unsigned retval = 0;
    
    for (i=0; i<length; i++) {
	retval += string[i];
    }
    return(retval % SZHASHTABLE);
}
