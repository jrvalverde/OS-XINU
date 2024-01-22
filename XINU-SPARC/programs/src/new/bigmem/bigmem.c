/* 
 * bigmem.c - grab a lot of memory and keep it for a while,
 *            just for vm debugging and testing the dumpcore routines
 * 
 * Author:	Shawn Ostermann
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Tue May 22 22:27:34 1990
 */


#include <sys/syscall.h>
#include <sys/xinusys.h>
#include <sys/xinuconf.h>


static int secs;
static int cycle;
static int quiet;
static int priority;


#define PGSIZ_CHARS 4096
#define PGSIZ_WORDS 1024

struct page {
	unsigned 	tag;
	unsigned	junk[PGSIZ_WORDS-1];
};


/*------------------------------------------------------------------------
 *  bigmem
 *------------------------------------------------------------------------
 */
main(argc, argv)
     int	argc;
     char	*argv[];
{
    int n_pages;
    char *pbuf;
    int i;

    n_pages = -1;
    secs = 600;		/* default sleep time in seconds */
    quiet = FALSE;
    cycle = 1;
    priority = 0;

    for (i=1; i < argc; ++i) {
	if (strcmp(argv[i],"-s") == 0) {
	    if (((i+1) < argc) && (isdigit(*argv[i+1])))
		secs = atoi(argv[i+1]);
	    else
		Usage(argv[0]);
	    ++i;
	} else if (strcmp(argv[i],"-c") == 0) {
	    if (((i+1) < argc) && (isdigit(*argv[i+1])))
		cycle = atoi(argv[i+1]);
	    else
		Usage(argv[0]);
	    ++i;
	} else if (strcmp(argv[i],"-p") == 0) {
	    if (((i+1) < argc) && (isdigit(*argv[i+1])))
		priority = atoi(argv[i+1]);
	    else
		Usage(argv[0]);
	    ++i;
	} else if (strcmp(argv[i],"-q") == 0) {
	    quiet = TRUE;
	} else if (isdigit(*argv[i])) {
	    if (n_pages < 0)
		sscanf(argv[i],"%i",&n_pages);
	    else
		Usage(argv[0]);
	} else
	    Usage(argv[0]);
    }

    if (n_pages < 0)
	Usage(argv[0]);

    if (priority != 0) {
	if (xchprio(xgetpid(),priority) == SYSERR) {
	    printf("Couldn't change priority to %d\n", priority);
	    return(SYSERR);
	}
    }
    if (!quiet)
       printf("Running at priority %d\n", xgetprio(xgetpid()));

    /* get heap pages */
    if ((pbuf = (char *) xgetheap(PGSIZ_CHARS * n_pages)) == (char *) SYSERR) {
	printf("Couldn't allocate %d pages of heap\n", n_pages);
	return(SYSERR);
    }
    
    if (!quiet)
	printf("Holding %d pages of heap for %d seconds\n", n_pages, secs);

    while (cycle-- > 0) {
	if (!quiet)
	    printf("\nIteration %d\n\n", cycle+1);

	doit(secs,n_pages,pbuf);
    }
}


/*-------------------------------------------------------------------------
 * doit - 
 *-------------------------------------------------------------------------
 */
doit(secs,n_pages,page)
     int secs;
     int n_pages;
     struct page page[];
{
    int i, j;
    unsigned val;
    int errors;

    Log("Writing to each location\n");
    for (val = 0,i=0; i < n_pages; ++i, ++val) {
	if (!quiet)
	    printf("Writing to page %d (%08x)\n", i, &page[i]);
	page[i].tag = val;
    }

    Log("Sleeping\n");
    xsleep(secs);
    
    Log("Good Morning\n");

    Log("Checking each location\n");
    
    errors = 0;
    for (val = 0,i=0; i < n_pages; ++i, ++val) {
	if (!quiet)
	    printf("Checking page %d (%08x): ", i, &page[i]);
	if (page[i].tag != val) {
	    ++errors;
	    printf("ERROR, saw %d, wanted %d\n", page[i].tag, val);
	} else
	    Log("OK\n");
    }

    if (errors == 0)
	printf("No Errors\n");
    else
	printf("Final error Count: %d\n", errors);
}


Log(m)
{
    if (!quiet)
	printf(m);
}


Usage(prog)
     char *prog;
{
    printf("usage: %s [-s N] [-q] [-p N] [-c N] n_pages\n", prog);
    printf("       hold heap pages for a length of time\n");
    printf("       -s N   hold for N seconds\n");
    printf("       -c N   cycle: repeat N times\n");
    printf("       -p N   run at priority N\n");
    printf("       -q     quiet: only print errors\n");
    exit(-1);
}


static isdigit(ch)
     int ch;
{
    return(ch >= '0' && ch <= '9');
}
