
/* $Header: /usr/src/local/etc/bootpd/src/RCS/insert.c,v 1.5 1991/01/07 20:10:42 trinkle Exp $ */

/* 
 * insert.c - routine to insert an entry into a hash table
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun 14 12:05:43 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log: insert.c,v $
 * Revision 1.5  1991/01/07  20:10:42  trinkle
 * Latest mods from smb
 *
 * Revision 1.4  90/08/17  15:09:34  trinkle
 * Replaced strncmp with bcmp.
 * 
 * Revision 1.3  90/07/12  10:50:23  smb
 * Commented and declared register variables.
 * 
 * Revision 1.2  90/07/03  12:43:49  smb
 * Restructured directories to move header files to ../include
 * 
 * Revision 1.1  90/06/14  16:48:16  smb
 * Initial revision
 * 
 */

#include <strings.h>
#include <hash.h>

/* this function is polymorphic! */
int insert(entry, table, key, keylen)
     VOID *entry;
     struct list *table[];
     register char *key;
     register int keylen;
{
    unsigned hashvalue;
    int retval;
    register struct list *bucket;

    retval = -1;
    hashvalue = hash(key, keylen);
    bucket = table[hashvalue];
    if (!bucket) {
	table[hashvalue] = (struct list *) xmalloc(sizeof(struct list));
	bucket = table[hashvalue];
	bzero(bucket, sizeof(struct list));
	bucket->key = key;
	bucket->keylen = keylen;
	bucket->data = entry;
	retval = 0;
    }
    else {
	while (bucket) {
	    if ((keylen == bucket->keylen) &&
		(bcmp(key, bucket->key, keylen) == 0)) {
		break;
	    }
	    if (bucket->next) {
		bucket = bucket->next;
	    }
	    else {
		bucket->next = (struct list *) xmalloc(sizeof(struct list));
		bucket = bucket->next;
		bzero(bucket, sizeof(struct list));
		bucket->key = key;
		bucket->keylen = keylen;
		bucket->data = entry;
		retval = 0;
		break;
	    }
	}
    }
    return(retval);
}
