
/* $Header: /usr/src/local/etc/bootpd/src/RCS/server.c,v 1.6 1991/01/07 20:10:42 trinkle Exp $ */

/* 
 * server.c - main server loop
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Wed Jun 20 10:29:48 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log: server.c,v $
 * Revision 1.6  1991/01/07  20:10:42  trinkle
 * Latest mods from smb
 *
 * Revision 1.5  90/07/12  10:51:22  smb
 * Commented and declared register variables.
 * 
 * Revision 1.4  90/07/03  12:45:20  smb
 * Restructured directories to move header files to ../include
 * 
 * Revision 1.3  90/06/26  16:40:38  smb
 * Added check of config file modification time.
 * 
 * Revision 1.2  90/06/26  14:48:46  smb
 * Eliminated standalone parameter and completed the processing of
 * messages.
 * 
 * Revision 1.1  90/06/20  16:34:31  smb
 * Initial revision
 * 
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <syslog.h>
#include <errno.h>
#include <sys/param.h>
#include <bool.h>

#include <bootp.h>
#include <sizes.h>
#include <bootp_conf.h>
#include <port_descr.h>

extern int errno;	/* this is for the old BSD 4.3 headers */

void server(config)
     char *config;
{
    register char buf[SZMAXMSG];
    char ip[SZIPTEXT], en[SZENTEXT], name[MAXHOSTNAMELEN];
    register struct bootp_msg *msg;
    struct hostent *host;
    struct sockaddr_in addr;
    int saddr;
    register fd_set readfds;
    
    gethostname(name, MAXHOSTNAMELEN);

    for(;;) {
	FD_ZERO(&readfds);
	FD_SET(server_port_sd, &readfds);
	if (select(32, &readfds, NULL, NULL, NULL) < 0) {
	    if (errno != EINTR) {
		syslog(LOG_ERR, "select: %m");
		continue;
	    }
	}
	
	/* get a bootp message from the server socket */
	saddr = sizeof(struct sockaddr_in);
	if (recvfrom(server_port_sd, buf, SZMAXMSG, 0, &addr, &saddr) < 0) {
	    syslog(LOG_ERR, "recvfrom: %m");
	    continue;
	}

	/* check for a proper message */
	msg = (struct bootp_msg *) buf;
	if ((msg->op != BOOTREQUEST) || (msg->unused != 0)) {
	    syslog(LOG_ERR, "bad message format");
	    continue;
	}

	/* log who it was from (NOTE: IP address is likely not correct yet */
	sprint_en(en, msg->chaddr);
	syslog(LOG_INFO, "message received from %s", en);

	/*
	 * if client specified a server name then we want to check it
	 * against our name(s)
	 */
	if (msg->sname[0] != '\0') {
	    if (host = gethostbyname(msg->sname)) {
		if (strcmp(host->h_name, name) != 0) {
		    continue;
		}
	    }
	}

	/*
	 * if we get to here, it is for us.  it still might not get
	 * answered if we don't have a file for them, no entry in the
	 * database, etc.
	 */
	/*
	 * We also need to check to see if the configuration file has
	 * changed since the last time we read it.
	 */
	if (getmtime(config) > mtime) {
	    reload();
	}
	
	if (process_request(msg) < 0) {
	    continue;
	}
    }
}
