/* x_fmdump.c - x_fmdump */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

/*------------------------------------------------------------------------
 *  x_fmdump  -  print frames that a process owns
 *------------------------------------------------------------------------
 */
COMMAND	x_fmdump(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	int	pid;

	switch (nargs) {

	case 2:
/*		sscanf(args[1], "%d", &pid);*/
		pid = atoi(args[1]);
		return(fmdump(pid));
		break;
	default:
		fprintf(stderr, "use: %s pid\n", args[0]);
		return(SYSERR);
	}
	return(OK);
}
