
/* $Header: /.gwen/u5/smb/src/bootp/bootps/src/RCS/print_ip.c,v 1.2 90/07/12 10:50:49 smb Exp $ */

/* 
 * print_ip.c - print an IP address to stdout, a string, or a file
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Wed Jun 20 11:38:37 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	print_ip.c,v $
 * Revision 1.2  90/07/12  10:50:49  smb
 * Commented and declared register variables.
 * 
 * Revision 1.1  90/06/20  16:31:32  smb
 * Initial revision
 * 
 */

/*
 * currently there is only an sprint_ip() but there could easily be an
 * fprint_ip() and a print_ip()
 */

static char *format = "%u.%u.%u.%u";

void sprint_ip(str, ipaddr)
     register char *str;
     register unsigned char ipaddr[4];
{
    sprintf(str, format, ipaddr[0], ipaddr[1], ipaddr[2], ipaddr[3]);
    return;
}
