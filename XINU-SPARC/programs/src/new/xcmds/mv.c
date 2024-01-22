/* mv.c - mv */

#include <stdio.h>

/*------------------------------------------------------------------------
 *  mv  -  (xinu dynamic mv command) move (rename) a file
 *------------------------------------------------------------------------
 */
main(nargs, args)
int	nargs;
char	*args[];
{
	if (nargs != 3) {
		fprintf(stderr, "usage: mv file tofile\n");
		exit(1);
	}
	if (rename(args[1], args[2]) == (-1) ) {
		fprintf(stderr, "Cannot move %s\n", args[1]);
		exit(1);
	}
}
