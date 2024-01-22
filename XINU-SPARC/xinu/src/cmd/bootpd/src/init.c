
/* $Header: /usr/src/local/etc/bootpd/src/RCS/init.c,v 1.4 1990/12/05 14:46:03 trinkle Exp $ */

/* 
 * init.c - server initialization routine
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Jun 15 15:03:57 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log: init.c,v $
 * Revision 1.4  1990/12/05  14:46:03  trinkle
 * Updates from smb, mainly for byte ordering corrects and Ultrix
 * modifications.
 *
 * Revision 1.3  90/07/12  10:50:17  smb
 * Commented and declared register variables.
 * 
 * Revision 1.2  90/06/26  14:45:33  smb
 * Removed standalone parameter.
 * 
 * Revision 1.1  90/06/19  14:28:46  smb
 * Initial revision
 * 
 */

#include <syslog.h>
#include <stdio.h>
#include <strings.h>

#define PID_FILE	"bootpd.pid"

extern char Version[];

int init(config)
     char *config;
{
    unsigned long mypid;
    register char *pidfile;
    FILE *fpid;
    
#ifdef mips
    openlog("bootpd", LOG_PID);
#else
    openlog("bootpd", LOG_PID | LOG_CONS, LOG_DAEMON);
#endif

    syslog(LOG_INFO, Version);

    fclose(stdin);
    fclose(stdout);
    fclose(stderr);

#ifndef NODETACH
    detach();
    if (fork())
	exit(0);
#endif

    mypid = getpid();
    pidfile = (char *) xmalloc(strlen(BOOTPD_DIR) + strlen(PID_FILE) + 2);
    strcpy(pidfile, BOOTPD_DIR);
    strcat(pidfile, "/");
    strcat(pidfile, PID_FILE);
    fpid = fopen(pidfile, "w");
    fprintf(fpid, "%u\n", mypid);
    fclose(fpid);
    free(pidfile);

    if (opensockets() < 0) {
	return(-1);
    }
    
    /* need to setup my handling of signals */
    sigsetup();
    
    inithash();
    if (readfile(config) < 0) {
	return(-1);
    }

    if (getifaces() < 0) {
	return(-1);
    }
}
