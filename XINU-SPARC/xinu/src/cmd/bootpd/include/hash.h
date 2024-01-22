
/* $Header: /usr/src/local/etc/bootpd/include/RCS/hash.h,v 1.5 1991/01/07 21:43:30 trinkle Exp $ */

/* 
 * hash.h - definitions for the hash function/tables
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun 14 10:21:16 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log: hash.h,v $
 * Revision 1.5  1991/01/07  21:43:30  trinkle
 * Latest mods from smb
 *
 * Revision 1.1  1991/01/07  20:10:42  trinkle
 * Latest mods from smb
 *
 * Revision 1.4  90/07/12  10:23:37  smb
 * Corrected a comment.
 * 
 * Revision 1.3  90/07/12  09:58:42  smb
 * Commented.
 * 
 * Revision 1.2  90/07/03  12:51:06  smb
 * Restructured directories to move all header files here.
 * 
 * Revision 1.1  90/06/14  16:47:40  smb
 * Initial revision
 * 
 */

#define SZHASHTABLE	67		/* should be prime	*/

#ifndef LIST_H
#include <list.h>
#endif

/* extern declarations for the various hashes (see inithash.c for uses) */
extern struct list *name_hash[];
extern struct list *haddr_hash[];
extern struct list *generic_hash[];
extern struct list *ip_hash[];
extern struct list *text_hash[];

extern VOID *lookup();
extern unsigned hash();
