
/* $Header: /.gwen/u5/smb/src/bootp/bootps/src/RCS/print_en.c,v 1.3 90/07/12 10:50:46 smb Exp $ */

/* 
 * print_en.c - print Ethernet address to stdout, a string, or a file
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Wed Jun 20 12:08:19 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	print_en.c,v $
 * Revision 1.3  90/07/12  10:50:46  smb
 * Commented and declared register variables.
 * 
 * Revision 1.2  90/06/26  14:46:25  smb
 * Fixed it so that the octets are unsigned.
 * 
 * Revision 1.1  90/06/20  16:31:04  smb
 * Initial revision
 * 
 */

/*
 * currently, there is only sprint_en() but there could easily be a
 * fprint_en() and a print_en()
 */

static char *format = "%02x:%02x:%02x:%02x:%02x:%02x";

void sprint_en(str, en)
     register char *str;
     register unsigned char *en;
{
    sprintf(str, format, en[0], en[1], en[2], en[3], en[4], en[5]);
    return;
}
