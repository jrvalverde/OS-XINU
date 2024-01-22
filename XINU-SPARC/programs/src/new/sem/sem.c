
/* 
 * sem.c - Print the contents of the semaphore table
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

#define SEMDEVICE	"/dev/ktab/semtab"
#define BUFFSIZE	1024

main(argc, argv)
int argc;
char *argv[];
{
	int semdev;
	char buff[BUFFSIZE];
	struct sentry_ai *semp;
	char *ststr();
	int sid;
	
	if ((semdev = open(SEMDEVICE, O_RDONLY, 0666)) == -1) {
		fprintf(stderr, "Could not open %s\n", SEMDEVICE);
		exit(1);
	}
	printf("%3.3s %5.5s %5.5s %4.4s %4.4s\n", "SID", "STATE", "COUNT",
	       "HEAD", "TAIL");
	sid = 0;
	while (read(semdev, buff, BUFFSIZE) > 0) {
		semp = (struct sentry_ai *) buff;
		printf("%3.3d %5.5s %5.5d %4.4d %4.4d\n", sid,
		       ststr(semp->sstate), semp->semcnt, semp->sqhead,
		       semp->sqtail);
		sid++;		       
	}
	close(semdev);
	    
}

char *ststr(state)
     char state;
{
	if (state == SFREE)
	    return("FREE");
	else
	    return("USED");
}
