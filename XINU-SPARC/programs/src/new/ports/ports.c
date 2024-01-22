
/* 
 * ports.c - Print the contents of the ports table
 * 
 * Author:	Vic Norman
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Sat Sep 15 12:48:05 1990
 *
 * Copyright (c) 1990 Vic Norman
 */

#include <stdio.h>
#include <fcntl.h>
#include <ktab.h>
#include <ktabobj.h>

#define PORTDEVICE	"/dev/ktab/porttab"
#define BUFFSIZE	1024

main(argc, argv)
int argc;
char *argv[];
{
	int portdev;
	char buff[BUFFSIZE];
	struct pt_ai *portp;
	int ptid;
	char *getstate();
	
	if ((portdev = open(PORTDEVICE, O_RDONLY, 0666)) == -1) {
		fprintf(stderr, "Could not open %s\n", PORTDEVICE);
		exit(1);
	}
	printf("%4.4s %5.5s %4.4s %4.4s %8.8s %10.10s\n", "PORT", "STATE",
	       "SSEM", "RSEM", "MAXCOUNT", "SEQUENCE #");
	ptid = 0;
	while (read(portdev, buff, BUFFSIZE) > 0) {
		portp = (struct pt_ai *) buff;
		printf("%4.4d %5.5s %4.4d %4.4d %8.8d %10.10d\n", ptid,
		       getstate(portp->ptstate), portp->ptssem, portp->ptrsem,
		       portp->ptmaxcnt, portp->ptseq);
		ptid++;		       
	}
	close(portdev);
	    
}

char *getstate(state)
     char state;
{
	if (state == PTFREE)
	    return("FREE");
	else if (state == PTLIMBO)
	    return("LIMBO");
	else
	    return("ALLOC");
}
