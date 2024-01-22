
/* 
 * frames.c - Print the contents of the frame table
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

#define FRAMEDEVICE	"/dev/ktab/frametab"
#define BUFFSIZE	1024
/* from frame.h */
#define FTNLIST		0		/* no list - locked		*/
#define FTALIST		1		/* active list value		*/
#define FTMLIST		2		/* modified list value		*/
#define FTRLIST		3		/* reclaim list value		*/
#define ftlist(x)	((x)->bits.st.list)
#define ftlocked(x)	(ftlist(x)==FTNLIST)
#define ftonalist(x)	(ftlist(x)==FTALIST)
#define ftonmlist(x)	(ftlist(x)==FTMLIST)
/* from vmem.sun3.h */
#define PGSIZ			0x2000	/* size of a single page	*/
/* from vmem.h */
#define	tovaddr(x)	(((unsigned) x) * PGSIZ)
					/* return vm address of page x	*/
/* from conf.h */
#define	NPROC	    30			/* number of user processes	*/
/* from proc.h */
#define NADDR NPROC
#define isasid(x)	(x>=NPROC && x<NPROC+NADDR)

main(argc, argv)
int argc;
char *argv[];
{
	int framedev;
	char buff[BUFFSIZE];
	struct fte_ai *pfte;
	int frame;
	
	if ((framedev = open(FRAMEDEVICE, O_RDONLY, 0666)) == -1) {
		fprintf(stderr, "Could not open %s\n", FRAMEDEVICE);
		exit(1);
	}

	frame = 0;
	while (read(framedev, buff, BUFFSIZE) > 0) {
		pfte = (struct fte_ai *) buff;
		if (pfte->bits.st.used) {
			if ((frame % 20) == 0) {
				printf("frame # idtype  id   virt addr  used locked ");
				printf("shared wanted pgout tstamp list\n");
				printf("------- ------ ---- ----------- ---- ------ ");
				printf("------ ------ ----- ------ ----\n");
			}
			/*      frame#  idtype id  virtaddr used */
			printf(" %5d     %c   %4d  %09x    %1d  ",
				frame, (isasid(pfte->id)?'A':'P'),
				pfte->id, tovaddr(pfte->pageno), 1);
			/*      locked   shared   wanted    pgout*/
			printf("   %1d      %1d     %1d      %1d   ",
				ftlocked(pfte),
				pfte->bits.st.shared,
				pfte->bits.st.wanted,
				pfte->bits.st.pgout);
			/*	tstamp list			*/
			printf("%6d   %c\n",
				pfte->tstamp,
				ftlocked(pfte)?'N':
				(ftonalist(pfte)?'A':
				 (ftonmlist(pfte)?'M':'R')));
		}
#ifdef LOTSOUTPUT			
		else {			/* frame not in use - free frame */
			/*      frame#  idtype id  virtaddr used */
			printf(" %5d     %c   %4d  %09x    %1d  ",
				frame, 'N', (-1), 0, FALSE);
			/*      locked   shared   wanted    pgout*/
			printf("   %1d      %1d     %1d      %1d   ",
				FALSE, FALSE, FALSE, FALSE);
			/*	tstamp list			*/
			printf("%6d   %c\n",
				0, 'R');
		}
#endif
	frame++;
	}

	close(framedev);
}
