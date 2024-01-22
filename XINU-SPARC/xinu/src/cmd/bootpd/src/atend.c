
/* $Header: /usr/src/local/etc/bootpd/src/RCS/atend.c,v 1.3 1991/01/07 20:10:42 trinkle Exp $ */

/* 
 * atend.c - add a value to the end of a list
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun 14 12:22:11 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log: atend.c,v $
 * Revision 1.3  1991/01/07  20:10:42  trinkle
 * Latest mods from smb
 *
 * Revision 1.2  90/07/03  12:42:54  smb
 * Restructured directories to move header files to ../include
 * 
 * Revision 1.1  90/06/14  16:43:57  smb
 * Initial revision
 * 
 */

#include <list.h>

void atend(data, list)
     VOID *data;
     struct list **list;
{
    register struct list *walker;

    if (*list) {
	walker = *list;
	while (walker->next) {
	    walker = walker->next;
	}
	walker->next = (struct list *) xmalloc(sizeof(struct list));
	walker = walker->next;
    }
    else {
	*list = (struct list *) xmalloc(sizeof(struct list));
	walker = *list;
    }
    bzero(walker, sizeof(struct list));
    walker->data = data;
    return;
}
