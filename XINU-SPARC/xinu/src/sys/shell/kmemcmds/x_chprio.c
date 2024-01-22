/* x_chprio.c - x_chprio */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  x_chprio  -  (command chprio) change the priority of a process
 *------------------------------------------------------------------------
 */
BUILTIN	x_chprio(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	int ret;
	int pid;
	int prio;
	int i;
	
	if (nargs < 3) {
		fprintf(stderr, "use: %s pid [pid*] prio\n", args[0]);
		return(SYSERR);
	}

	prio = atoi(args[nargs-1]);
	--nargs;

	for (i=1; i < nargs; ++i) {
		pid = atoi(args[i]);

		if ((ret = chprio(pid,prio)) > 0) {
			fprintf(stdout, "previous priority of process %d: %d\n",
				pid, ret);
		} else {
			fprintf(stderr, "bad return code from chprio(%d,%d): %d\n",
				pid,prio,ret);
		}
	}
	return(OK);
}
