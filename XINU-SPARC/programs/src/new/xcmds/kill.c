/* kill.c - kill */

#include <stdio.h>

/*------------------------------------------------------------------------
 *  kill  -  (xinu dynamic kill command) terminate a process
 *------------------------------------------------------------------------
 */
main(nargs, args)
int	nargs;
char	*args[];
{
	int	pid;

	if (nargs != 2) {
		fprintf(stderr, "use: kill process-id\n");
		exit(1);
	}
	pid = atoi(args[1]);
	xkill(pid);
}
