
/* $Header: /usr/src/local/etc/bootpd/src/RCS/reload.c,v 1.6 1991/01/07 21:47:50 trinkle Exp $ */

/* 
 * reload.c - reload the configuration file
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Tue Jun 19 14:08:17 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log: reload.c,v $
 * Revision 1.6  1991/01/07  21:47:50  trinkle
 * Corrected typo in cast.
 *
 * Revision 1.5  1991/01/07  20:10:42  trinkle
 * Latest mods from smb
 *
 * Revision 1.4  90/07/12  10:51:15  smb
 * Commented and declared register variables.
 * 
 * Revision 1.3  90/07/03  12:45:13  smb
 * Restructured directories to move header files to ../include
 * 
 * Revision 1.2  90/06/26  16:40:20  smb
 * Now complete.  Hopefully this works.
 * 
 * Revision 1.1  90/06/19  14:30:18  smb
 * Initial revision
 * 
 */

#include <bootp_conf.h>
#include <list.h>
#include <hash.h>
#include <args.h>

void reload()
{
    int i;
    register struct list *walker, *walker2, *tmp;
    register struct bootp_entry *entry;
    struct generic_pair *generic;
    
    for (i=0; i<SZHASHTABLE; i++) {
	walker = name_hash[i];
	while (walker) {
	    entry = (struct bootp_entry *) walker->data;
	    if (--entry->rcount <=0) {
		free(entry->name);
		walker2 = entry->aliases;
		while (walker2) {
		    free(walker2->data);
		    walker2 = walker2->next;
		}
		freelist(entry->aliases);
		free(entry);
	    }
	    tmp = walker;
	    walker = walker->next;
	    free(tmp);
	}

	/*
	 * we can simply free the haddr_hash entries using freelist since
	 * all of their data is already gone (every entry MUST have a name)
	 * or will be cleaned up by the name_hash
	 */
	freelist(haddr_hash[i]);

	walker = text_hash[i];
	while (walker) {
	    free(walker->data);
	    tmp = walker;
	    walker = walker->next;
	    free(tmp);
	}

	walker = ip_hash[i];
	while (walker) {
	    free(walker->data);
	    tmp = walker;
	    walker = walker->next;
	    free(tmp);
	}

	walker = generic_hash[i];
	while (walker) {
	    generic = (struct generic_pair *) walker->data;
	    free(generic->generic_name);
	    free(generic->file_name);
	    free(generic);
	    tmp = walker;
	    walker = walker->next;
	    free(tmp);
	}
	
    }

    inithash();
    
    readfile(config);
	
    return;
}
