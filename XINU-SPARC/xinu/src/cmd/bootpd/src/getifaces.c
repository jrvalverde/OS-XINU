
/* $Header: /usr/src/local/etc/bootpd/src/RCS/getifaces.c,v 1.5 1991/01/07 20:10:42 trinkle Exp $ */

/* 
 * getifaces.c - build a list of our IP interfaces
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun 21 15:53:01 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log: getifaces.c,v $
 * Revision 1.5  1991/01/07  20:10:42  trinkle
 * Latest mods from smb
 *
 * Revision 1.4  1990/12/05  14:45:43  trinkle
 * Updates from smb, mainly for byte ordering corrects and Ultrix
 * modifications.
 *
 * Revision 1.3  90/07/12  10:50:04  smb
 * Commented and declared register variables.
 * 
 * Revision 1.2  90/07/03  12:43:31  smb
 * Restructured directories to move header files to ../include
 * 
 * Revision 1.1  90/06/21  16:55:40  smb
 * Initial revision
 * 
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <syslog.h>
#include <null.h>

#include <ifaces.h>

#define BUFSIZE	1024

struct list *myIPaddrs = NULL;

int getifaces()
{
    int s, nif;
    register int i;
    struct ifconf ifc;
    register struct ifreq *ifr;
    char buf[BUFSIZE];
    struct sockaddr_in *addr;
    unsigned long ipaddr, *tlong;
    register struct list *temp;

    if ((s=socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	syslog(LOG_ERR, "socket: %m");
	close(s);
	return(-1);
    }
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if (ioctl(s, SIOCGIFCONF, &ifc) < 0) {
	syslog(LOG_ERR, "ioctl: %m");
	close(s);
	return(-1);
    }
    close(s);

    nif = ifc.ifc_len / sizeof(struct ifreq);
    for (i=0, ifr=ifc.ifc_req; i<nif; i++, ifr++) {
	addr = (struct sockaddr_in *) &(ifr->ifr_addr);
	if ((addr->sin_family == AF_INET) &&
	    ((ipaddr = addr->sin_addr.s_addr) != 0)) {
	    temp = (struct list *) xmalloc(sizeof(struct list));
	    bzero(temp, sizeof(struct list));
	    temp->next = myIPaddrs;
	    myIPaddrs = temp;
	    tlong = (unsigned long *) xmalloc(sizeof(long));
	    *tlong = ipaddr;
	    temp->data = (VOID *) tlong;
	}
    }
    return(0);
}
