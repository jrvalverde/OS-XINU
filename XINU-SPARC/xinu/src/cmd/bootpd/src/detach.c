
/* $Header: /.gwen/u5/smb/src/bootp/bootps/src/RCS/detach.c,v 1.2 90/07/12 10:49:44 smb Exp $ */

/* 
 * detach.c - detach the process from the controlling terminal
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Jun 15 15:06:39 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	detach.c,v $
 * Revision 1.2  90/07/12  10:49:44  smb
 * Commented and declared register variables.
 * 
 * Revision 1.1  90/06/19  14:27:50  smb
 * Initial revision
 * 
 */

#include <sys/file.h>
#include <sys/ioctl.h>

void detach()
{
    register int temp;

    /* detach from the terminal */
    temp = open("/dev/tty", O_RDWR);
    ioctl(temp, TIOCNOTTY, 0);
    close(temp);
}
