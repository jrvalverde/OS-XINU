
/* $Header: /usr/src/local/etc/bootpd/include/RCS/list.h,v 1.3 1991/01/07 21:43:30 trinkle Exp $ */

/* 
 * list.h - definition of list structures and routines
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun 14 12:00:52 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log: list.h,v $
 * Revision 1.3  1991/01/07  21:43:30  trinkle
 * Latest mods from smb
 *
 * Revision 1.1  1991/01/07  20:10:42  trinkle
 * Latest mods from smb
 *
 * Revision 1.2  90/07/12  09:58:49  smb
 * Commented.
 * 
 * Revision 1.1  90/06/14  16:49:01  smb
 * Initial revision
 * 
 */

#ifndef LIST_H
#define LIST_H

#ifdef vax
#define VOID	char
#else
#define VOID	void
#endif

/* generic list structure - not all fields are always used */
struct list {
    char *key;		/* key value, used by hash tables		*/
    int keylen;		/* key length, used by hash tables		*/
    VOID *data;		/* data; one of IP, bootp_entry, generic_pair	*/
    struct list *next;	/* link to next value				*/
};

#endif
