
/* 
 * x_pgstats.c - Print virtual memory paging statistics
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Wed Feb  8 17:16:17 1989
 *
 * Copyright (c) 1989 Jim Griffioen
 */


#include <conf.h>
#include <kernel.h>
#include <proc.h>

/*------------------------------------------------------------------------
 *  x_pgstats  -  print virtual memory paging stats
 *------------------------------------------------------------------------
 */
COMMAND	x_pgstats(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
    int i;
    char showtimes, sorttimes, toss;

    showtimes = FALSE;
    sorttimes = FALSE;
    toss = FALSE;
	
    if (nargs > 4) {
	fprintf(stderr, "use: %s [h][t][s][d]\n", args[0]);
	fprintf(stderr, " h - help, t - times, s - sort,");
	fprintf(stderr, " d - drop high and low times\n");
	return(SYSERR);
    }

    for (i=1; i<nargs; i++) {
	if (*args[i] == 't')
	    showtimes = TRUE;
	else if (*args[i] == 's')
	    sorttimes = TRUE;
	else if (*args[i] == 'd')
	    toss = TRUE;
	else {
	    fprintf(stderr, "use: %s [h][t][s][d]\n", args[0]);
	    fprintf(stderr, " h - help, t - times, s - sort,");
	    fprintf(stderr, " d - drop high and low times\n");
	    return(SYSERR);
	}
    }
	
    return(pgstats(stdin, stdout, stderr, showtimes, sorttimes, toss));
}
