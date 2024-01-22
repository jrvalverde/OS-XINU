
/* $Header: /usr/src/local/etc/bootpd/src/RCS/enter.c,v 1.8 1991/02/08 20:38:52 trinkle Exp $ */

/* 
 * enter.c - enter the value for a specified tag in the bootp entry
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun 14 11:14:01 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log: enter.c,v $
 * Revision 1.8  1991/02/08  20:38:52  trinkle
 * Added changes from smb to handle both hex and dotted quad input of
 * netmasks and numbers.
 *
 * Revision 1.7  1991/01/07  20:10:42  trinkle
 * Latest mods from smb
 *
 * Revision 1.6  1990/12/05  14:44:51  trinkle
 * Updates from smb, mainly for byte ordering corrects and Ultrix
 * modifications.
 *
 * Revision 1.5  90/08/17  14:41:23  trinkle
 * Added cast for hardware address bytes.
 * 
 * Revision 1.4  90/07/27  10:32:49  trinkle
 * Xinu additions from smb.
 * 
 * Revision 1.3  90/07/12  10:49:47  smb
 * Commented and declared register variables.
 * 
 * Revision 1.2  90/07/03  12:43:20  smb
 * Restructured directories to move header files to ../include
 * 
 * Revision 1.1  90/06/14  16:45:30  smb
 * Initial revision
 * 
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <syslog.h>
#include <bootp_conf.h>
#include <hash.h>
#include <tags.h>
#include <null.h>

/* this routine will assign value to the tag for entry */
int enter(tag, value, entry, ch)
     register int tag;
     char **value;
     register struct bootp_entry *entry;
     char ch;
{
    register char *tvalue;
    int state, tlen, htype, stat, k;
    unsigned long tulong, *ulptr;
    register VOID *lptr;
    char tch;
    
    tvalue = *value;
    state = 0;

    /* check for a few illegal cases at the start */
    if (ch == '|') {
	if ((tag == BOOTFILE) || (tag == GENERIC) || (tag == HADDR) ||
	    (tag == HOMEDIR) || (tag == HTYPE) || (tag == IPADDR) ||
	    (tag == MACHINE) || (tag == NETMASK) || (tag == TEMPLATE) ||
    /* begin Xinu additions */
	    (tag == TIME_OFFSET) || (tag == CIPADDR) || (tag == CNETMASK) ||
	    (tag == CIMPLETS)) {
	    state = ERROR;
	    syslog(LOG_ERR, "enter(), set state to ERROR, bad tag %d", tag);
	    return(state);
	}
    }

    /*
     * now do the first batch of processing on the entry - this
     * involves making sure that the value is in its own hash, if
     * appropriate 
     */
    switch (tag) {
	case BOOTFILE:
	case HOMEDIR:
	case MACHINE: {
	    stat = insert(tvalue, text_hash, tvalue, strlen(tvalue));
	    lptr = lookup(text_hash, tvalue, strlen(tvalue));
	    if (stat < 0) {
		free(tvalue);
	    }
	    break;
	}
	case GENERIC: {
	    lptr = lookup(generic_hash, tvalue, strlen(tvalue));
	    if (!lptr) {
		syslog(LOG_ERR, "unknown generic name %s", tvalue);
	    }
	    free(tvalue);
	    break;
	}
	case GATEWAY:
	case LPR_SERVER:
	case DOMAIN_SERVER:
	case RLP_SERVER:
	case TIME_SERVER: {
	    ulptr = (unsigned long *) xmalloc(sizeof(long));
	    tulong = inet_addr(tvalue);
	    tulong = ntohl(tulong);
	    *ulptr = tulong;
	    free(tvalue);
	    stat = insert(ulptr, ip_hash, ulptr, sizeof(long));
	    if (stat < 0) {
		free(ulptr);
	    }
	    lptr = lookup(ip_hash, &tulong, sizeof(long));
	    break;
	}
	case HADDR:	/* for logic checking	*/
	case HTYPE:
	case IPADDR:
	case NETMASK:
	case TIME_OFFSET:
	/* begin Xinu additions */
	case CIPADDR:
	case CNETMASK:
	case CIMPLETS: {
	    break;
	}
	case TEMPLATE: {
	    lptr = lookup(name_hash, tvalue, strlen(tvalue));
	    if (!lptr) {
		syslog(LOG_ERR, "unknown template name %s", tvalue);
	    }
	    free(tvalue);
	    break;
	}
	default: { /* this is why the logic checking entries are here	*/
	    syslog(LOG_ERR, "logic error - unknown tag value %d", tag);
	    exit(1);
	}
    }

    /* now fill in the value from above in the entry */
    switch (tag) {
	case BOOTFILE: {
	    entry->bootfile = lptr;
	    break;
	}
	case GENERIC: {
	    if (lptr) {
		entry->generic = (struct generic_pair *) lptr;
	    }
	    break;
	}
	case GATEWAY: {
	    atend(lptr, &entry->gateway);
	    break;
	}
	case HADDR: {
	    tlen = strlen(tvalue);
	    if (tlen > 2*SZMAXHADDR) {
		state = ERROR;
		syslog(LOG_ERR,
		       "enter(), set state to ERROR, tvalue too long");
		break;
	    }
	    for (k = 0; k<tlen-2; k+=2) {
		tch = tvalue[k+2];
		tvalue[k+2] = 0;
		sscanf(&tvalue[k], "%x", &tulong);
		entry->haddr[k/2] = (char) tulong;
		tvalue[k+2] = tch;
	    }
	    sscanf(&tvalue[k], "%x", &tulong);
	    entry->haddr[k/2] = (char) tulong;
	    entry->hlen = k/2 + 1;
	    free(tvalue);
	    break;
	}
	case HOMEDIR: {
	    entry->homedir = lptr;
	    break;
	}
	case HTYPE: {
	    htype = get_htype(tvalue);
	    if (htype < 0) {
		syslog(LOG_WARNING, "unknown hardware type %s", tvalue);
	    }
	    entry->htype = htype;
	    free(tvalue);
	    break;
	}
	case IPADDR: {
	    tulong = inet_addr(tvalue);
	    entry->ipaddr = ntohl(tulong);
	    free(tvalue);
	    break;
	}
	case LPR_SERVER: {
	    atend(lptr, &entry->lpr_servers);
	    break;
	}
	case MACHINE: {
	    entry->machine = lptr;
	    break;
	}
	case NETMASK: {
	    if ((entry->netmask = ntohl(inet_addr(tvalue))) < 0) {
		syslog(LOG_ERR, "enter(), bad netmask %s", tvalue);
		entry->netmask = 0;
	    }
	    free(tvalue);
	    break;
	}
	case DOMAIN_SERVER: {
	    atend(lptr, &entry->domain_servers);
	    break;
	}
	case RLP_SERVER: {
	    atend(lptr, &entry->rlp_servers);
	    break;
	}
	case TEMPLATE: {
	    if (lptr) {
		copy_entry(entry, lptr);
	    }
	    break;
	}
	case TIME_OFFSET: {
	    entry->time_offset = atol(tvalue);
	    free(tvalue);
	    break;
	}
	case TIME_SERVER: {
	    atend(lptr, &entry->time_servers);
	    break;
	}
	/* begin XINU additions */
	case CIPADDR: {
	    tulong = inet_addr(tvalue);
	    entry->cipaddr = ntohl(tulong);
	    free(tvalue);
	    break;
	}
	case CNETMASK: {
	    if ((entry->cnetmask = ntohl(inet_addr(tvalue))) < 0) {
		syslog(LOG_ERR, "enter(), bad cnetmask %s", tvalue);
		entry->cnetmask = 0;
	    }
	    free(tvalue);
	    break;
	}
	case CIMPLETS: {
	    if ((strlen(tvalue) >= 2) &&
		((tvalue[1] == 'x') || (tvalue[1] == 'X'))) {
		sscanf(&tvalue[2], "%x", &entry->cimplets);
	    }
	    else {
		sscanf(tvalue, "%d", &entry->cimplets);
	    }
	    free(tvalue);
	    break;
	}
	default: {
	    syslog(LOG_ERR, "logic error");
	    state = ERROR;
	    break;
	}
    }
    if (state != ERROR) {
	state = (ch == ':') ? READY : READ_VALUE;
    }
    return(state);
}
    
