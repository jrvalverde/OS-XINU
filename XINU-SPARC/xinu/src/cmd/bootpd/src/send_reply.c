
/* $Header: /usr/src/local/etc/bootpd/src/RCS/send_reply.c,v 1.8 1991/01/07 20:10:42 trinkle Exp $ */

/* 
 * send_reply.c - send the bootp reply to the client
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Jun 22 15:04:30 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log: send_reply.c,v $
 * Revision 1.8  1991/01/07  20:10:42  trinkle
 * Latest mods from smb
 *
 * Revision 1.7  1990/12/05  14:47:55  trinkle
 * Updates from smb, mainly for byte ordering corrects and Ultrix
 * modifications.
 *
 * Revision 1.6  90/07/12  11:15:31  smb
 * Removed addr from a register because cc complained.
 * 
 * Revision 1.5  90/07/12  10:51:19  smb
 * Commented and declared register variables.
 * 
 * Revision 1.4  90/07/03  12:54:14  smb
 * Corrected a bad include of <sys/sockio.h> and replaced it with
 * <sys/ioctl.h> which, on a sun, includes the other file.
 * 
 * Revision 1.3  90/07/03  12:45:17  smb
 * Restructured directories to move header files to ../include
 * 
 * Revision 1.2  90/06/26  16:42:20  smb
 * Added code (untested) to add an ARP cache entry for the client.
 * 
 * Revision 1.1  90/06/26  14:48:22  smb
 * Initial revision
 * 
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <syslog.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <errno.h>
#ifdef sun
#include <net/if_arp.h>
#endif

#include <sizes.h>
#include <bootp.h>
#include <port_descr.h>

extern int errno;

int send_reply(buf, addr, haddr)
     register char *buf;
     unsigned long addr;
     register unsigned char *haddr;
{
    struct sockaddr_in saddr, *siaddr;
    char ip[SZIPTEXT];
    register struct arpreq arpreq;

    bzero(&saddr, sizeof(struct sockaddr_in));
    saddr.sin_addr.s_addr = addr;
    saddr.sin_port = client_port_num;
    saddr.sin_family = AF_INET;

#ifndef DEBUG
    /* need to add an arp entry */
    bzero(&arpreq, sizeof(struct arpreq));

    siaddr = (struct sockaddr_in *) &(arpreq.arp_pa);
    siaddr->sin_addr.s_addr = addr;
    arpreq.arp_pa.sa_family = AF_INET;

    bcopy(haddr, arpreq.arp_ha.sa_data, SZENADDR);
    arpreq.arp_ha.sa_family = AF_UNSPEC;

    arpreq.arp_flags = ATF_INUSE | ATF_COM;

    if (ioctl(server_port_sd, SIOCSARP, &arpreq) < 0) {
	/* if this fails, check for errno == EHOSTUNREACH which means
	 * that it is not on a connected Ethernet (may be Cypress).
	 */
	if (errno != ENETUNREACH) {
	    syslog(LOG_ERR, "ioctl(SIOCSARP): %m");
	    return(-1);
	}
    }
    
#endif

    if (sendto(server_port_sd, buf, sizeof(struct bootp_msg), 0, &saddr,
	       sizeof(struct sockaddr_in)) < 0) {
	syslog(LOG_ERR, "sendto: %m");
	return(-1);
    }

    sprint_ip(ip, &addr);
    syslog(LOG_INFO, "sent reply to %s", ip);
    return(0);
}
