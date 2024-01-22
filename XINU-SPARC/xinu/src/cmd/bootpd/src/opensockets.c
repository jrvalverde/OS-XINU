
/* $Header: /usr/src/local/etc/bootpd/src/RCS/opensockets.c,v 1.4 1990/12/05 14:47:00 trinkle Exp $ */

/* 
 * opensockets.c - open the bootp protocol server socket
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Jun 15 15:10:55 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log: opensockets.c,v $
 * Revision 1.4  1990/12/05  14:47:00  trinkle
 * Updates from smb, mainly for byte ordering corrects and Ultrix
 * modifications.
 *
 * Revision 1.3  90/07/03  12:44:04  smb
 * Restructured directories to move header files to ../include
 * 
 * Revision 1.2  90/06/26  14:45:51  smb
 * Removed standalone parameter and realized that I only need to open 
 * the server socket, not the client.
 * 
 * Revision 1.1  90/06/19  14:29:58  smb
 * Initial revision
 * 
 */

#include <syslog.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <bootp.h>
#include <null.h>
#include <port_descr.h>

int server_port_sd;
int client_port_num;

int opensockets()
{

    register short port;
    register struct sockaddr_in addr;
    register int sd;

    port = getport(BOOTPS, NBOOTPS);

    server_port_sd = socket(PF_INET, SOCK_DGRAM, 0);
    if (server_port_sd < 0) {
	syslog(LOG_ERR, "socket: %m");
	return(-1);
    }

    bzero(&addr, sizeof(struct sockaddr));
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(server_port_sd, &addr, sizeof(struct sockaddr)) < 0) {
	syslog(LOG_ERR, "bind: %m");
	return(-1);
    }

    client_port_num = getport(BOOTPC, NBOOTPC);

    return 0;
}
