
/* $Header: /.gwen/u5/smb/src/bootp/bootps/src/RCS/process_args.c,v 1.3 90/07/03 12:44:10 smb Exp $ */

/* 
 * process_args.c - process the <argc, argv> pair
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Jun 15 14:56:31 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	process_args.c,v $
 * Revision 1.3  90/07/03  12:44:10  smb
 * Restructured directories to move header files to ../include
 * 
 * Revision 1.2  90/06/26  14:44:15  smb
 * Removed standalone parameter and -s flag
 * 
 * Revision 1.1  90/06/19  14:25:39  smb
 * Initial revision
 * 
 */

#include <stdio.h>

#include <args.h>

char *config = NULL;

extern void usage();

int process_args(argc, argv)
     int argc;
     char *argv[];
{
    int i;
    char c;
    extern int optind;
    extern char *optarg;
    
    i = 1;
    while (i < argc) {
	if (argv[i][0] == '-') {
	    optind = i;
	    while ((c = getopt(argc, argv, "hc:")) != EOF) {
		switch (c) {
		    case 'c': {
			config = (char *) xmalloc(strlen(optarg) + 1);
			strcpy(config, optarg);
			break;
		    }
		    case 'h':
		    default: {
			usage(stderr, argv[0]);
			return(-1);
			break;
		    }
		}
	    }
	    i = optind;
	}
	if (i < argc) {
	    usage(stderr, argv[0]);
	    i++;
	    return(-1);
	}
    }
    if (config == NULL) {
	config = (char *) malloc(strlen(BOOTPD_DIR) + strlen(DEF_CONFIG) + 2);
	strcpy(config, BOOTPD_DIR);
	strcat(config, "/");
	strcat(config, DEF_CONFIG);
    }
    return(0);
}

void usage(file, name)
     FILE *file;
     char *name;
{
    fprintf(file, "usage: %s [-c <config file>]\n", name);
    return;
}
