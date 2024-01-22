
/* $Header: /.gwen/u5/smb/src/bootp/bootps/src/RCS/sigsetup.c,v 1.2 90/07/12 10:51:26 smb Exp $ */

/* 
 * sigsetup.c - setup handling of system signals
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Tue Jun 19 13:42:04 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	sigsetup.c,v $
 * Revision 1.2  90/07/12  10:51:26  smb
 * Commented and declared register variables.
 * 
 * Revision 1.1  90/06/19  14:26:22  smb
 * Initial revision
 * 
 */

#include <signal.h>
#include <syslog.h>

#ifdef sun
extern void reload(), quit();
#else
extern int reload(), quit();
#endif

void sigsetup()
{
    register int mask;

    mask = (sigmask(SIGINT) | sigmask(SIGPIPE) | sigmask(SIGALRM) |
	    sigmask(SIGXCPU) | sigmask(SIGXFSZ) | sigmask(SIGVTALRM) |
	    sigmask(SIGPROF) | sigmask(SIGUSR1) | sigmask(SIGUSR2));
#ifdef sun
    mask |= sigmask(SIGLOST);
#endif
#ifndef DEBUG
    mask |= sigmask(SIGTSTP);
#endif

    sigblock(mask);

    if ((int) signal(SIGHUP, reload) < 0) {
	syslog(LOG_ERR, "signal SIGHUP: %m");
	exit(1);
    }
    
    if ((int) signal(SIGTERM, quit) < 0) {
	syslog(LOG_ERR, "signal SIGTERM: %m");
	exit(1);
    }
    
    return;
}
