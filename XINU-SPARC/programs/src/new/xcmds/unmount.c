/* unmou.c -  unmou */

#include <stdio.h>
#include <sys/xinusys.h>

/*------------------------------------------------------------------------
 *  unmou  -  (xinu dyanamic unmount command) remove prefix from namespace
 *------------------------------------------------------------------------
 */
main(nargs, args)
int	nargs;
char	*args[];
{
	if (nargs != 2) {
		fprintf(stderr, "use: unmount prefix\n");
		exit(1);
	}
	if (xunmount(args[1]) == SYSERR) {
		fprintf(stderr, "unmount fails.\n");
		exit(1);
	}
}
