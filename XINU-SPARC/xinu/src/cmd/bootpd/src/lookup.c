
/* $Header: /usr/src/local/etc/bootpd/src/RCS/lookup.c,v 1.5 1991/08/13 20:21:19 trinkle Exp $ */

/* 
 * lookup.c - lookup an entry in a hash table
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun 14 12:39:04 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log: lookup.c,v $
 * Revision 1.5  1991/08/13  20:21:19  trinkle
 * Fixed another strncmp() problem (should be bcmp for ether addresses)
 *
 * Revision 1.4  1991/01/07  20:10:42  trinkle
 * Latest mods from smb
 *
 * Revision 1.3  90/07/12  10:50:27  smb
 * Commented and declared register variables.
 * 
 * Revision 1.2  90/07/03  12:43:53  smb
 * Restructured directories to move header files to ../include
 * 
 * Revision 1.1  90/06/14  16:49:23  smb
 * Initial revision
 * 
 */

#include <strings.h>
#include <hash.h>
#include <null.h>

/* this function is polymorphic! */
VOID *lookup(table, key, keylen)
     struct list *table[];
     register char *key;
     register int keylen;
{
    VOID *retval;
    unsigned hashvalue;
    register struct list *bucket;

    retval = NULL;
    hashvalue = hash(key, keylen);
    bucket = table[hashvalue];
    while (bucket) {
	if ((keylen == bucket->keylen) &&
	    (bcmp(key, bucket->key, keylen) == 0)) {
	    retval = bucket->data;
	    break;
	}
	bucket = bucket->next;
    }
    return(retval);
}
