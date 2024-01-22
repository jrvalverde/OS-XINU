
/* $Header: /usr/src/local/etc/bootpd/src/RCS/process_1084_vendor.c,v 1.5 90/07/27 10:32:58 trinkle Exp $ */

/* 
 * process_1084_vendor.c - process vendor field as per RFC 1084
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Wed Jun 20 16:04:11 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	process_1084_vendor.c,v $
 * Revision 1.5  90/07/27  10:32:58  trinkle
 * Xinu additions from smb.
 * 
 * Revision 1.4  90/07/12  10:50:52  smb
 * Commented and declared register variables.
 * 
 * Revision 1.3  90/07/03  12:44:06  smb
 * Restructured directories to move header files to ../include
 * 
 * Revision 1.2  90/06/26  14:46:39  smb
 * Completed the handling of subfields, and I am now checking for
 * potential overflows of the vendor area.
 * 
 * Revision 1.1  90/06/20  16:32:56  smb
 * Initial revision
 * 
 */

#include <sys/types.h>
#include <netinet/in.h>
#include <strings.h>
#include <null.h>

#include <sizes.h>
#include <bootp.h>
#include <bootp_conf.h>

void process_1084_vendor(vend, entry, filesize)
     struct vendor *vend;
     register struct bootp_entry *entry;
     int filesize;
{
    register struct vendor_subfield *subf;
    register struct list *lptr;
    long tlong;
    unsigned long tulong;
    unsigned short size, tushort;
    int nip;
    register int ind;
    char *dot;

    ind = 0;
    vend->magic = htonl(RFC1084);
    subf = (struct vendor_subfield *) &(vend->data[ind]);
    if (entry->netmask != 0) {
	subf->tag = SUBNETMASK;
	subf->length = SZIPADDR;
	tulong = htonl(entry->netmask);
	bcopy(&tulong, subf->data, SZIPADDR);
	ind += SZIPADDR + 2 * sizeof(char);
	subf = (struct vendor_subfield *) &(vend->data[ind]);
    }

    if (entry->time_offset != 0) {	/* what do we do with UTC? */
	subf->tag = TIME_OFFSET;
	subf->length = sizeof(long);
	tlong = htonl(entry->time_offset);
	bcopy(&tlong, subf->data, sizeof(long));
	ind += sizeof(long) + 2 * sizeof(char);
	subf = (struct vendor_subfield *) &(vend->data[ind]);
    }

    if (entry->gateway) {
	nip = 0;
	subf->tag = GATEWAY;

	lptr = entry->gateway;
	while (lptr) {
	    tulong = htonl(*((unsigned long *) lptr->data));
	    bcopy(&tulong, &(subf->data[nip*SZIPADDR]), SZIPADDR);
	    nip++;
	    lptr = lptr->next;
	}
	subf->length = nip * SZIPADDR;
	ind += subf->length + 2 * sizeof(char);
	subf = (struct vendor_subfield *) &(vend->data[ind]);
    }
    
    if (entry->lpr_servers) {
	nip = 0;
	subf->tag = LPR_SERVER;

	lptr = entry->lpr_servers;
	while (lptr) {
	    tulong = htonl(*((unsigned long *) lptr->data));
	    bcopy(&tulong, &(subf->data[nip*SZIPADDR]), SZIPADDR);
	    nip++;
	    lptr = lptr->next;
	}
	subf->length = nip * SZIPADDR;
	ind += subf->length + 2 * sizeof(char);
	subf = (struct vendor_subfield *) &(vend->data[ind]);
    }
    
    if (entry->rlp_servers) {
	nip = 0;
	subf->tag = RLP_SERVER;

	lptr = entry->rlp_servers;
	while (lptr) {
	    tulong = htonl(*((unsigned long *) lptr->data));
	    bcopy(&tulong, &(subf->data[nip*SZIPADDR]), SZIPADDR);
	    nip++;
	    lptr = lptr->next;
	}
	subf->length = nip * SZIPADDR;
	ind += subf->length + 2 * sizeof(char);
	subf = (struct vendor_subfield *) &(vend->data[ind]);
    }
    
    if (entry->domain_servers) {
	nip = 0;
	subf->tag = DOMAIN_NAMESERVER;

	lptr = entry->domain_servers;
	while (lptr) {
	    tulong = htonl(*((unsigned long *) lptr->data));
	    bcopy(&tulong, &(subf->data[nip*SZIPADDR]), SZIPADDR);
	    nip++;
	    lptr = lptr->next;
	}
	subf->length = nip * SZIPADDR;
	ind += subf->length + 2 * sizeof(char);
	subf = (struct vendor_subfield *) &(vend->data[ind]);
    }
    
    if (entry->time_servers) {
	nip = 0;
	subf->tag = TIME_SERVER;

	lptr = entry->time_servers;
	while (lptr) {
	    tulong = htonl(*((unsigned long *) lptr->data));
	    bcopy(&tulong, &(subf->data[nip*SZIPADDR]), SZIPADDR);
	    nip++;
	    lptr = lptr->next;
	}
	subf->length = nip * SZIPADDR;
	ind += subf->length + 2 * sizeof(char);
	subf = (struct vendor_subfield *) &(vend->data[ind]);
    }

    dot = index(entry->name, '.');
    if (dot != NULL) {
	subf->length = (unsigned char) (dot - entry->name);
    }
    else {
	subf->length = strlen(entry->name);
    }
    
    if (ind + subf->length + 3 * sizeof(char) < SZVENDOR - sizeof(long) - 1) {
	subf->tag = HOSTNAME;
	strncpy(subf->data, entry->name, subf->length);
	subf->length++;
	ind += subf->length + 2 * sizeof(char);
	subf = (struct vendor_subfield *) &(vend->data[ind]);
    }

    if (ind + sizeof(unsigned short) + 2 * sizeof(char) < SZVENDOR -
	sizeof(long) - 1) {
	size = filesize / SZBLOCK;
	if (filesize % SZBLOCK != 0) {
	    size++;
	}
	subf->tag = BOOT_FILE_SIZE;
	subf->length = sizeof(unsigned short);
	tushort = htons(size);
	bcopy(&tushort, subf->data, sizeof(unsigned short));
	ind += sizeof(unsigned short) + 2 * sizeof(char);
	subf = (struct vendor_subfield *) &(vend->data[ind]);
    }
    
    subf->tag = END;
    
    return;
}
