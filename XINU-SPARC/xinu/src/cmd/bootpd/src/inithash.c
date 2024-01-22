
/* $Header: /.gwen/u5/smb/src/bootp/bootps/src/RCS/inithash.c,v 1.2 90/07/03 12:43:41 smb Exp $ */

/* 
 * inithash.c - initialize the hash tables to be empty
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun 14 12:50:35 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	inithash.c,v $
 * Revision 1.2  90/07/03  12:43:41  smb
 * Restructured directories to move header files to ../include
 * 
 * Revision 1.1  90/06/14  16:48:02  smb
 * Initial revision
 * 
 */

#include <hash.h>
#include <null.h>

/*
 * the following hash tables are used to store various name to address
 * bindings as specified here:
 *
 * 	name_hash - binds the name of a template to the template record
 * 	haddr_hash - binds a hardware address to a template record
 * 	generic_hash - binds a generic name to the generic name/file
 * 		name record
 * 	ip_hash - binds an IP address to an IP address (used for
 * 		server lists)
 * 	text_hash - binds a piece of text to a piece of text (used for
 * 		home directories, bootfiles, machine types, etc.)
 */
struct list *name_hash[SZHASHTABLE];
struct list *haddr_hash[SZHASHTABLE];
struct list *generic_hash[SZHASHTABLE];
struct list *ip_hash[SZHASHTABLE];
struct list *text_hash[SZHASHTABLE];

void inithash()
{
    register int i;

    for (i=0; i<SZHASHTABLE; i++) {
	name_hash[i] = NULL;
    }
    for (i=0; i<SZHASHTABLE; i++) {
	haddr_hash[i] = NULL;
    }
    for (i=0; i<SZHASHTABLE; i++) {
	generic_hash[i] = NULL;
    }
    for (i=0; i<SZHASHTABLE; i++) {
	ip_hash[i] = NULL;
    }
    for (i=0; i<SZHASHTABLE; i++) {
	text_hash[i] = NULL;
    }
    return;
}
