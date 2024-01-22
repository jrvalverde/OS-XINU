
/* $Header: /.gwen/u5/smb/src/bootp/bootps/src/RCS/freelist.c,v 1.3 90/07/12 10:49:57 smb Exp $ */

/* 
 * freelist.c - free a list structure without freeing the data of the list
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun 14 12:27:07 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	freelist.c,v $
 * Revision 1.3  90/07/12  10:49:57  smb
 * Commented and declared register variables.
 * 
 * Revision 1.2  90/07/03  12:43:26  smb
 * Restructured directories to move header files to ../include
 * 
 * Revision 1.1  90/06/14  16:46:14  smb
 * Initial revision
 * 
 */

#include <list.h>

/* NOTE: this does NOT free the data associated with the list elements */
void freelist(list)
     register struct list *list;
{
    register struct list *walker;

    while (list) {
	walker = list;
	list = list->next;
	if (walker->key) {
	    free(walker->key);
	}
	free(walker);
    }
    return;
}
