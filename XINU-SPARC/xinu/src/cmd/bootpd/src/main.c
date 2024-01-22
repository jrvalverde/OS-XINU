
/* $Header: /.gwen/u5/smb/src/bootp/bootps/src/RCS/main.c,v 1.5 90/07/03 12:43:55 smb Exp $ */

/* 
 * main.c - bootp server main entry routine
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Tue Jun 12 13:04:55 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	main.c,v $
 * Revision 1.5  90/07/03  12:43:55  smb
 * Restructured directories to move header files to ../include
 * 
 * Revision 1.4  90/06/26  14:40:52  smb
 * Removed standalone parameter and decided that this will NOT be called
 * by inetd.
 * 
 * Revision 1.3  90/06/20  16:32:31  smb
 * Expanded calls to support routines slightly.
 * 
 * Revision 1.2  90/06/19  14:29:30  smb
 * Re-structured to call the new service routines.
 * 
 * Revision 1.1  90/06/14  16:50:00  smb
 * Initial revision
 * 
 */

#include <args.h>

main(argc, argv)
     int argc;
     char *argv[];
{
    if (process_args(argc, argv) < 0) {
	exit(1);
    }
    if (init(config) < 0) {
	exit(1);
    }

    server(config);
    
    exit(0);
}
