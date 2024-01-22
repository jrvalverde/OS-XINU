
/* 
 * smore.c - A simple more program
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Dec 11, 1989
 *
 * Copyright (c) 1989 Jim Griffioen
 */

#include	<stdio.h>
#include	<sys/file.h>

#define		NUMLINES	23	/* number of lines per screen	*/
#define		MAXRLEN		512	/* Xinu remote file constant	*/
#define		MAXLINELEN	256	/* maximum line length		*/

#define		TRUE		1
#define		FALSE		0

extern char	*jngfgets();

main(argc, argv)
int argc;
char *argv[];
{
	char	*name;
	int	lcount, len;
	FILE	*fptr;
	char	line[MAXLINELEN];
	char	ch;
	char	askagain;

	if (argc != 2) {
		fprintf(stderr, "usage: %s filename\n", argv[0]);
		exit(1);
	}
	name = argv[1];
	if( access( name, R_OK) != 0){
		printf("Can not access file %s\n",name);
		return;
	}

	if((fptr = fopen(name, "r")) == NULL) {
	       printf("Can not open file %s\n",name);
	       return;
   	}

	while (TRUE) {
		lcount = 0;
		while ((lcount < NUMLINES) &&
		       (jngfgets(line, MAXLINELEN, fptr) == line)) {
			printf("%s", line);
			lcount++;
		}
		if (lcount < NUMLINES) {
			printf("\n");	/* end of file */
			fclose(fptr);
			return;
		}
		askagain = TRUE;
		while (askagain) {
			printf("smore -> ");
			askagain = FALSE;
			while ((ch=getchar()) != '\n') {
				if ((ch == 'q') || (ch == 'Q')) {
					/* get the rest of the line */
					while (getchar()!='\n');
					fclose(fptr);
					return;
				}
				if ((ch == 'h') || (ch == 'H')) {
					printf("type 'q' to quit, ");
					printf("type 'h' for this help\n");
					askagain = TRUE;
				}
			}
		}
	}
	    
}


char	rbuf[MAXRLEN];
char	*curpos = rbuf;
int	rbuflen = 0;

/*---------------------------------------------------------------------------
 * jngfgets - get a string from a file - ret strlen or (-1)
 *---------------------------------------------------------------------------
 */
char *jngfgets(sb, n, fptr)
char	*sb;
int	n;
FILE	*fptr;
{
	char	done;
	register char	*tmp;
	register int	len;
	int	fd;

	fd = readdtable(fileno(fptr));
	tmp = sb;
	len = 0;
	done = FALSE;
	while ((!done) && (--n>0)) {
		if (curpos >= (rbuf+rbuflen)) {	/* read another buffer full */
			rbuflen = xread(fd, rbuf, MAXRLEN);
			if (rbuflen <= 0)
			    return(NULL);
			curpos = rbuf;
		}
		while ((*curpos != '\n') && (curpos != (rbuf+rbuflen))) {
			*tmp++ = *curpos++;
			len++;
		}
		if (*curpos == '\n') {
			*tmp++ = '\n';
			len++;
			*tmp = '\0';
			curpos++;	/* leave curpos after '\n' */
			done = TRUE;
		}
	}
	return(sb);
}
