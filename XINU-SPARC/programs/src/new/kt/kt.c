
/* 
 * kt.c - Print the contents of the thread/process table
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

#define KTDEVICE	"/dev/ktab/ktab"
#define BUFFSIZE	1024

main(argc, argv)
int argc;
char *argv[];
{
	int ktdev;
	char buff[BUFFSIZE];
	struct ktspec_ai *ktp;
	
	if ((ktdev = open(KTDEVICE, O_RDONLY, 0666)) == -1) {
		fprintf(stderr, "Could not open %s\n", KTDEVICE);
		exit(1);
	}
	printf("%-16.16s %-20.20s %10.10s %10.10s %8.8s %8.8s\n", "KERNEL TABLE",
	       "DESCR", "ADDR", "ENTRY SIZE", "ROWS", "COLS");
	while (read(ktdev, buff, BUFFSIZE) > 0) {
		ktp = (struct ktspec_ai *) buff;
		printf("%-16.16s %-20.20s %10.10x %10.10d %8.8d %8.8d\n",
		       ktp->name, ktp->descr, ktp->baseaddr,
		       ktp->u_entrysize, ktp->maxrows, ktp->maxcols);
	}
	close(ktdev);
	    
}

