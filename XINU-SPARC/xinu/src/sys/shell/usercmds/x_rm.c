/* x_rm.c - x_rm */

#include <conf.h>
#include <kernel.h>
#include <file.h>

/*------------------------------------------------------------------------
 *  x_rm  -  (command rm) remove a file given its name
 *------------------------------------------------------------------------
 */
COMMAND	x_rm(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	int i;
	
	if (nargs != 2) {
		fprintf(stderr, "usage: rm file\n");
		return(SYSERR);
	}
	for (i=1 ; i<nargs ; i++) {
		if ( remove(args[i]) == SYSERR ) {
			fprintf(stderr, "Cannot remove %s\n", args[i]);
		}
	}
	return(OK);
}
