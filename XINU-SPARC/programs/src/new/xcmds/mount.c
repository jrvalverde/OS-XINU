/* mount.c - mount */

#define	PADTO	24

#include <sys/xinusys.h>
#include <stdio.h>
/*------------------------------------------------------------------------
 *  mount  -  (xinu dynamic mount command) change or display namespace table
 *------------------------------------------------------------------------
 */
main(nargs, args)
int	nargs;
char	*args[];
{
	int	dev;

	if (nargs != 4) {
		fprintf(stderr,"use: mount [prefix devicename new_prefix]\n");
		exit(1);
	}
	dev = xdvlookup(args[2]);
	if (xmount(args[1], dev, args[3]) == SYSERR) {
		fprintf(stderr, "Mount failed\n");
		exit(1);
	}
}


