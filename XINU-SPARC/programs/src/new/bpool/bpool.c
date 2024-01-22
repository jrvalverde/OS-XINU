
/* 
 * bpool.c - Print the contents of the buffer pool table
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

#define BPOOLDEVICE	"/dev/ktab/bpooltab"
#define BUFFSIZE	1024

main(argc, argv)
int argc;
char *argv[];
{
	int bpooldev;
	char buff[BUFFSIZE];
	struct bpool_ai *bpoolp;
	int bpid;
	
	if ((bpooldev = open(BPOOLDEVICE, O_RDONLY, 0666)) == -1) {
		fprintf(stderr, "Could not open %s\n", BPOOLDEVICE);
		exit(1);
	}
	printf("%4.4s %4.4s %3.3s %5.5s\n", "POOL", "SIZE", "SEM", "COUNT");
	bpid = 0;
	while (read(bpooldev, buff, BUFFSIZE) > 0) {
		bpoolp = (struct bpool_ai *) buff;
		if (bpoolp->bpsize != 0) {
			printf("%4.4d %4.4d %3.3d %5.5d\n", bpid,
			       bpoolp->bpsize, bpoolp->bpsem,
			       xscount(bpoolp->bpsem));
		}
		bpid++;		       
	}
	close(bpooldev);
	    
}
