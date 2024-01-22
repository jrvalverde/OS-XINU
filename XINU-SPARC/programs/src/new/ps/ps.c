
/* 
 * ps.c - Print the contents of the thread/process table
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Sat Sep 15 12:48:05 1990
 *
 * Copyright (c) 1990 Jim Griffioen
 */

#include <stdio.h>
#include <fcntl.h>
#include <ktab.h>
#include <ktabobj.h>

#define PSDEVICE	"/dev/ktab/threadtab"
#define BUFFSIZE	1024
#define MAXENTRIES	(BUFFSIZE/sizeof(struct pentry_ai))

#define	TRUE	1
#define FALSE	0

main(argc, argv)
int argc;
char *argv[];
{
	int ii, jj;
	char gflag, lflag, nflag, rflag, xflag;
	int psdev;
	char buff[BUFFSIZE];
	struct pentry_ai *pe;
	int xinuasid;
	char statestr[128];

	gflag = lflag = nflag = rflag = xflag = FALSE;
	for (ii=1; ii<argc; ii++) {
		if (strncmp(argv[ii], "-g",
			    (jj=strlen("-g"))) == 0) {
			gflag = TRUE;
		}
		else if (strncmp(argv[ii], "-l", 
				 (jj=strlen("-l"))) == 0) {
			lflag = TRUE;
		}
		else if (strncmp(argv[ii], "-n",
				 (jj=strlen("-n"))) == 0) {
			nflag = TRUE;
		}
		else if (strncmp(argv[ii], "-r",
				 (jj=strlen("-r"))) == 0) {
			rflag = TRUE;
		}
		else if (strncmp(argv[ii], "-x",
				 (jj=strlen("-x"))) == 0) {
			xflag = TRUE;
		}
		else if (strcmp(argv[ii], "-h") == 0) {
			help(argv[0]);
		}
		else if (strcmp(argv[ii], "-help") == 0) {
			help(argv[0]);
		}
		else {
			fprintf(stderr, "%s: unknown option %s\n",
				argv[0], argv[ii]);
			help(argv[0]);
			exit(1);
		}
	}

	if ((psdev = open(PSDEVICE, O_RDONLY, 0666)) == -1) {
		fprintf(stderr, "Could not open %s\n", PSDEVICE);
		exit(1);
	}
	printf(" %3s %10s", "PID", "NAME");
	if (lflag) {
		printf("%8s ", "STATE");
		printf("%5s ", "ASID");
		printf("%5s ", "PRIO");
		printf("%5s ", "SEM");
		printf("%8s ", "STDIN");
		printf("%8s ", "STDOUT");
		printf("%6s ", "NXTKIN");
	}
	printf("\n");
	do {
		 ii = read(psdev, buff, BUFFSIZE);
		 if (ii >= 1) {
			 pe = (struct pentry_ai *) buff;
			 if (pe->pid == 0) /* null process - we hope */
			     xinuasid = pe->asid;
			 if ((pe->pstate != PSFREE) &&
			     ((gflag) || (pe->asid != xinuasid)) &&
			     ((!rflag) || ((rflag) &&
					   ((pe->pstate == PSCURR) ||
					    (pe->pstate == PSREADY)))) ) {
				 printf(" %3d %10s",
					pe->pid, pe->pname);
				 if (lflag) {
					 getstate(statestr, pe->pstate);
					 printf("%8s ", statestr);
					 printf("%5d ", pe->asid);
					 printf("%5d ", pe->pprio);
					 printf("%5d ", pe->psem);
					 printf("%8s ", pe->stdindev);
					 printf("%8s ", pe->stdoutdev);
					 printf("%6d ", pe->pnxtkin);
				 }
				 printf("\n");
			 }
		 }
	} while (ii >= 1);
	close(psdev);
	    
}

getstate(str, num)
char *str;
int num;
{
	char *sb;
	
	if (num == PSCURR)
	    sb = "CURR";
	else if (num == PSFREE)
	    sb = "FREE";
	else if (num == PSREADY)
	    sb = "READY";
	else if (num == PSRECV)
	    sb = "RECV";
	else if (num == PSSLEEP)
	    sb = "SLEEP";
	else if (num == PSSUSP)
	    sb = "SUSP";
	else if (num == PSWAIT)
	    sb = "WAIT";
	else if (num == PSTRECV)
	    sb = "TRECV";
	strcpy(str, sb);
}



help(cmd)
char *cmd;
{
	printf("usage: %s [options]\n", cmd);
	printf("Options\n");
	printf("-------\n");
	printf("\t-g\tPrint all processes - not just interesting ones\n");
	printf("\t-l\tPrint long listing\n");
	printf("\t-n\tDo not interpret numeric fields\n");
	printf("\t-r\tPrint only running processes\n");
	printf("\t-x\tinclude processes with no controlling terminal\n");
	printf("\t-h\tThis help message\n");
	printf("\t-help\tThis help message\n");
	exit(0);
}

