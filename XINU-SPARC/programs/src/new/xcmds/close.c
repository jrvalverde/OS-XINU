/* close.c - close */

#include <stdio.h>

/*------------------------------------------------------------------------
 *  close  -  (xinu dynamic close command) close a device given its id
 *------------------------------------------------------------------------
 */
main(nargs, args)
int	nargs;
char	*args[];
{
	int i, j;
	
	if (nargs < 2) {
		fprintf(stderr, "usage: close device# [device# ...]\n");
		exit(1);
	}
	for (i=1; i<nargs; i++) {
		j=atoi(args[i]);
		if (j >= 0)
		    xclose(j);
	}
}
