
/* $Header: /usr/src/local/etc/bootpd/src/RCS/my_best_ipaddr.c,v 1.4 1990/12/05 14:46:41 trinkle Exp $ */

/* 
 * my_best_ipaddr.c - figure out which of my IP addresses is best for client
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun 21 16:28:32 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log: my_best_ipaddr.c,v $
 * Revision 1.4  1990/12/05  14:46:41  trinkle
 * Updates from smb, mainly for byte ordering corrects and Ultrix
 * modifications.
 *
 * Revision 1.3  90/07/12  10:50:39  smb
 * Commented and declared register variables.
 * 
 * Revision 1.2  90/07/03  12:44:01  smb
 * Restructured directories to move header files to ../include
 * 
 * Revision 1.1  90/06/21  16:56:26  smb
 * Initial revision
 * 
 */

#include <sizes.h>
#include <ifaces.h>

unsigned long my_best_ipaddr(caddr)
     unsigned long caddr;
{
    register char *caddrp, *maddrp;
    register struct list *lptr;
    int maxmatch = 0, match, i;
    unsigned long retval = 0;

#ifdef mips
    unsigned long foo = caddr;
    caddrp = (char *) &foo;
#else
    caddrp = (char *) &caddr;
#endif
    
    lptr = myIPaddrs;
    while (lptr) {
	maddrp = lptr->data;
	match = 0;
	for (i=0; i<SZIPADDR; i++) {
	    if (caddrp[i] == maddrp[i]) {
		match++;
	    }
	    else {
		break;
	    }
	}
	if (match > maxmatch) {
	    maxmatch = match;
	    retval = *((unsigned long *) lptr->data);
	}
	lptr = lptr->next;
    }
    return(retval);
}


    
