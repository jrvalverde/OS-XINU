
/* $Header: /usr/src/local/etc/bootpd/src/RCS/process_xinu_vendor.c,v 1.3 90/07/27 10:33:03 trinkle Exp $ */

/* 
 * process_xinu_vendor.c - process vendor field as per local "xinu" specs
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Wed Jun 20 16:05:08 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	process_xinu_vendor.c,v $
 * Revision 1.3  90/07/27  10:33:03  trinkle
 * Xinu additions from smb.
 * 
 * Revision 1.2  90/07/03  12:44:49  smb
 * Restructured directories to move header files to ../include
 * 
 * Revision 1.1  90/06/20  16:33:49  smb
 * Initial revision
 * 
 */

#include <sys/types.h>
#include <netinet/in.h>

#include <bootp.h>
#include <bootp_conf.h>

void process_xinu_vendor(vend, entry, filesize)
     struct vendor *vend;
     struct bootp_entry *entry;
     int filesize;
{
    register struct vendor_subfield *subf;
    long tlong;
    unsigned long tulong;
    register int ind;

    ind = 0;
    vend->magic = htonl(XINU);
    subf = (struct vendor_subfield *) &(vend->data[ind]);

    if (entry->netmask != 0) {
	subf->tag = X_ETHER_SUBNETMASK;
	subf->length = SZIPADDR;
	tulong = htonl(entry->netmask);
	bcopy(&tulong, subf->data, SZIPADDR);
	ind += SZIPADDR + 2 * sizeof(char);
	subf = (struct vendor_subfield *) &(vend->data[ind]);
    }

    if (entry->gateway) {
	subf->tag = X_ETHER_GATEWAY;
	tulong = htonl(*((unsigned long *) entry->gateway->data));
	bcopy(&tulong, subf->data, SZIPADDR);
	subf->length = SZIPADDR;
	ind += subf->length + 2 * sizeof(char);
	subf = (struct vendor_subfield *) &(vend->data[ind]);
    }
    
    if (entry->cnetmask != 0) {
	subf->tag = X_CYPRESS_SUBNETMASK;
	subf->length = SZIPADDR;
	tulong = htonl(entry->cnetmask);
	bcopy(&tulong, subf->data, SZIPADDR);
	ind += SZIPADDR + 2 * sizeof(char);
	subf = (struct vendor_subfield *) &(vend->data[ind]);
    }

    if (entry->cipaddr != 0) {
	subf->tag = X_CYPRESS_ADDRESS;
	subf->length = SZIPADDR;
	tulong = htonl(entry->cipaddr);
	bcopy(&tulong, subf->data, SZIPADDR);
	ind += SZIPADDR + 2 * sizeof(char);
	subf = (struct vendor_subfield *) &(vend->data[ind]);
    }

    if (entry->cimplets != 0) {
	subf->tag = X_CYPRESS_IMPLETS;
	subf->length = sizeof(long);
	tulong = htonl(entry->cimplets);
	bcopy(&tulong, subf->data, sizeof(long));
	ind += sizeof(long) + 2 * sizeof(char);
	subf = (struct vendor_subfield *) &(vend->data[ind]);
    }

    if (entry->time_offset != 0) {	/* what do we do with UTC? */
	subf->tag = X_TIME_OFFSET;
	subf->length = sizeof(long);
	tlong = htonl(entry->time_offset);
	bcopy(&tlong, subf->data, sizeof(long));
	ind += sizeof(long) + 2 * sizeof(char);
	subf = (struct vendor_subfield *) &(vend->data[ind]);
    }

    if (entry->time_servers) {
	subf->tag = X_TIME_SERVER;
	tulong = htonl(*((unsigned long *) entry->time_servers->data));
	bcopy(&tulong, subf->data, SZIPADDR);
	subf->length = SZIPADDR;
	ind += subf->length + 2 * sizeof(char);
	subf = (struct vendor_subfield *) &(vend->data[ind]);
    }
    
    subf->tag = END;

    return;
}
