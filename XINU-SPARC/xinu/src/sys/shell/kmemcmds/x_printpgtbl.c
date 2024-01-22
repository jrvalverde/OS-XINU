/* x_printpgtbl.c - x_printpgtbl */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

/*------------------------------------------------------------------------
 *  x_printpgtbl  -  print page tbl for process pid 
 *------------------------------------------------------------------------
 */
COMMAND	x_printpgtbl(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	int	pid;
	int	area;
	int	start;
	int	len;

	switch (nargs) {

	case 5:
/*		sscanf(args[1], "%d", &pid);*/
/*		sscanf(args[2], "%d", &area);*/
/*		sscanf(args[3], "%d", &start);*/
/*		sscanf(args[4], "%d", &len);*/
		pid = atoi(args[1]);
		area = atoi(args[2]);
		start = atoi(args[3]);
		len = atoi(args[4]);
		return(printpgtbl(pid, area, start, len));
		break;
	default:
		fprintf(stderr, "use: %s pid area(1,2,3,4,5 or 6) start len\n", args[0]);
		fprintf(stderr, "example: %s 22 1 0 10\n", args[0]);
		fprintf(stderr, "start is the relative pg number within the area\n");
		fprintf(stderr, "area's are:\n");
		fprintf(stderr, "1 - user text, data, bss, heap\n");
		fprintf(stderr, "2 - user stack\n");
		fprintf(stderr, "3 - per process kernel stack\n");
		fprintf(stderr, "4 - per process rsa io\n");
		fprintf(stderr, "5 - S/P\n");
		fprintf(stderr, "6 - xinu text data bss\n");
		return(SYSERR);
	}
	return(OK);
}
