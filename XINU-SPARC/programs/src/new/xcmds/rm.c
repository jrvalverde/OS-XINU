/* rm.c - rm */

#include <stdio.h>

/*------------------------------------------------------------------------
 *  rm  -  (xinu dynamic rm command) remove a file given its name
 *------------------------------------------------------------------------
 */
main(nargs, args)
int	nargs;
char	*args[];
{
	int i;
	
	if (nargs < 2) {
		fprintf(stderr, "usage: rm file\n");
		exit(1);
	}
	for (i=1; i<nargs; i++)
	    if ( unlink(args[i]) == (-1) ) {
		    fprintf(stderr, "Cannot remove %s\n", args[i]);
		    exit(1);
	    }
}
