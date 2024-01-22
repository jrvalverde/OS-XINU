/* 
 * connectsun.c - open an odt like connection to the backend sun
 * 
 * Author:	Shawn Oostermann/Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun  9 22:02:03 1988
 *
 * Copyright (c) 1988 Shawn Oostermann/Jim Griffioen
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sgtty.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <pwd.h>

#include "../../h/utils.h"
#include "../../h/bed.h"
#include "../../h/bmutils.h"
#include "../../h/netutils.h"

/* getlogin does not work when odt, or download is used within a 	*/
/* 'script' session - so we must use getpwuid like whoami does		*/
extern char *getlogin();		/* get user id			*/

struct passwd *getpwuid();		/* get password entry 		*/

#define	STDOUT	1
#define	STDIN	0
#ifndef TRUE
#define TRUE	1
#define FALSE	0
typedef int	Bool;
#endif

#define BUFSIZE 100

int	flags;
int	timeout;			/* time keyboard can be idle	*/
int	pid;				/* process id of this process	*/
int	machnum;			/* number of requested machine	*/
char	*lockfile;			/* name of lock file		*/
extern	int verbose;			/* verbose option		*/



/*
 *---------------------------------------------------------------------------
 * connectsun() - return a file descriptor to talk on to backend with
 *---------------------------------------------------------------------------
 */
connectsun(class, femachine, bemachine, unspecifiedclass)
char class[];		/* class of machine desired	*/
char femachine[];	/* front end machine to use	*/
char bemachine[];	/* back end machine to use	*/
char unspecifiedclass;	/* user specified class?	*/
{
	int i;
	int sock;
	struct bm bm;
	int fdTcp;
	int len;
	char *sb;
	int remoteport;
	struct genericreq *odtreq;	/* odt request structure	*/
	struct passwd *pp;		/* password entry		*/

	if (obtainlock(class,femachine,bemachine,unspecifiedclass) != SYSOK) {
		if (strlen(bemachine) > 0)
		    fprintf(stderr, "machine %s is unavailable\n", bemachine);
		else if (strlen(class) > 0)
		    fprintf(stderr, "no machines in class %s available\n",
			    class);
		exit(1);
	}
	
	printf("using front end '%s':    Using backend '%s'\n",
	       femachine, bemachine);
	
	
	
	bm.ver = CURVER;
	sprintf(bm.cmd, "%d", REQ_ODT_CONNECT);
	odtreq = (struct genericreq *) bm.detail;
/* getlogin does not work when odt, or download is used within a 	*/
/* 'script' session - so we must use getpwuid like whoami does		*/
/*	sb = getlogin();*/
	pp = getpwuid(geteuid());
	if (pp == 0) {
		printf("Intruder alert\n");
		exit(1);
	}
	strcpy(odtreq->uid, pp->pw_name /*sb*/);
	if (gethostname(odtreq->hostid, MAXMACHINENAME) != 0) {
		strcpy(odtreq->hostid, "dunno");
		fprintf(stderr, "error getting local host name\n");
	}
	strcpy(odtreq->bename, bemachine);
	
	sock = ConnectUdp (femachine, KNOWN_UDP_PORT);
	
	if (bmsend (sock, &bm, lbm, 0) <= 0)
	    SysError ("send");
	
	len = bmrecv (sock, &bm, lbm, 0);
	if (len<=0) 
	    Error("recv(): response code %d\n",len);
	
	if (atoi(bm.cmd) != RESP_OK) {
		Error("recv(): bad response (%s) from daemon: '%s'\n",bm.cmd,bm.detail);
	}
	
	/* get the TCP port to use */
	remoteport = atoi(bm.detail);
	if (verbose) printf("using TCP port %d\n", remoteport);
	
	
	fdTcp = ConnectTcp(femachine, remoteport);
	if (verbose) printf("ready to read and write characters\n");
	
	return(fdTcp);
	
}

