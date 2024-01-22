/* x_pmeg.c - x_pmeg */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

/*------------------------------------------------------------------------
 *  x_pmeg  -  print info about the pmegs in the system
 *------------------------------------------------------------------------
 */
COMMAND	x_pmeg(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	int	pid;

	switch (nargs) {
	case 1:
		return(pmegdump(stdout,-1));
		break;
	case 2:
		return(pmegdump(stdout,atoi(args[1])));
		break;
	default:
		fprintf(stderr, "usage: %s [pid]\n", args[0]);
		return(SYSERR);
	}
}
