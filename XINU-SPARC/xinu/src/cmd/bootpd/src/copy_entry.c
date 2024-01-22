
/* $Header: /usr/src/local/etc/bootpd/src/RCS/copy_entry.c,v 1.4 90/07/27 10:32:45 trinkle Exp $ */

/* 
 * copy_entry.c - copy the initialized entries from entry2 to entry1
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun 14 11:46:12 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	copy_entry.c,v $
 * Revision 1.4  90/07/27  10:32:45  trinkle
 * Xinu additions from smb.
 * 
 * Revision 1.3  90/07/12  10:49:30  smb
 * Commented and declared register variables.
 * 
 * Revision 1.2  90/07/03  12:43:17  smb
 * Restructured directories to move header files to ../include
 * 
 * Revision 1.1  90/06/14  16:45:08  smb
 * Initial revision
 * 
 */

#include <bootp_conf.h>

/*
 * for every value in entry2 that is not empty, copy it to entry1 -
 * some of these will require true copies, others just pointers
 * also, we may have to free the list associated with the old value
 * in entry1
 */
void copy_entry(entry1, entry2)
     register struct bootp_entry *entry1, *entry2;
{
    int k;
    
    if (entry2->hlen != 0) {
	entry1->hlen = entry2->hlen;
	for (k=0; k<entry2->hlen; k++) {
	    entry1->haddr[k] = entry2->haddr[k];
	}
    }
    if (entry2->ipaddr != 0) {
	entry1->ipaddr = entry2->ipaddr;
    }
    if (entry2->netmask != 0) {
	entry1->netmask = entry2->netmask;
    }
    if (entry2->gateway) {
	freelist(entry1->gateway);
	entry1->gateway = entry2->gateway;
    }
    if (entry2->lpr_servers) {
	freelist(entry1->lpr_servers);
	entry1->lpr_servers = entry2->lpr_servers;
    }
    if (entry2->rlp_servers) {
	freelist(entry1->rlp_servers);
	entry1->rlp_servers = entry2->rlp_servers;
    }
    if (entry2->domain_servers) {
	freelist(entry1->domain_servers);
	entry1->domain_servers = entry2->domain_servers;
    }
    if (entry2->time_servers) {
	freelist(entry1->time_servers);
	entry1->time_servers = entry2->time_servers;
    }
    if (entry2->time_offset != 0) {
	entry1->time_offset = entry2->time_offset;
    }
    if (entry2->homedir) {
	entry1->homedir = entry2->homedir;
    }
    if (entry2->bootfile) {
	entry1->bootfile = entry2->bootfile;
    }
    if (entry2->generic) {
	entry1->generic = entry2->generic;
    }
    if (entry2->machine) {
	entry1->machine = entry2->machine;
    }

    /* begin Xinu additions */
    if (entry2->cipaddr != 0) {
	entry1->cipaddr = entry2->cipaddr;
    }
    if (entry2->cnetmask != 0) {
	entry1->cnetmask = entry2->cnetmask;
    }
    if (entry2->cimplets != 0) {
	entry1->cimplets = entry2->cimplets;
    }
}
