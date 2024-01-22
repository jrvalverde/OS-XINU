
/* 
 * asp.c - Print the contents of the thread/process table
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

#define ASPDEVICE	"/dev/ktab/addrsptab"
#define BUFFSIZE	1024

main(argc, argv)
int argc;
char *argv[];
{
	int aspdev;
	char buff[BUFFSIZE];
	struct aentry_ai *aspp;
	int asid;
	char torf();
	
	if ((aspdev = open(ASPDEVICE, O_RDONLY, 0666)) == -1) {
		fprintf(stderr, "Could not open %s\n", ASPDEVICE);
		exit(1);
	}
	/* for this hack, we assume asid starts at 0.  We should ask the
	   object where the pointer is, using a control call. */
	asid = 30;
	printf("%4.4s %5.5s %10.10s %11.11s %9.9s %8.8s\n", "ASID",
	       "VALID", "TSTAMP", "NUM THREADS", "THREAD ID", "INFORMED");
	while (read(aspdev, buff, BUFFSIZE) > 0) {
		aspp = (struct aentry_ai *) buff;
		printf("%4.4d %5.5c %10.10x %11.11d %9.9d %8.8c\n",
		       asid, torf(aspp->valid) ? 'T' : 'F', aspp->tstamp,
		       aspp->numthd, aspp->thd_id, torf(aspp->psinformed));
		asid++;
	}
	close(aspdev);
	    
}

char torf(arg)
     Boolean arg;
{
	return(arg ? 'T' : 'F');
}
