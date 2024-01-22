/* x_X.c - x_x */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

#define X_PATHS "/bin/X","/usr/local/bin/X"
#define X_PRIO 30

static char *paths[] = {X_PATHS,0};

/* start the X server */
COMMAND	x_X(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	int pid;
	int i;

	for (i=0; paths[i] != NULL; ++i) {
		pid = xinu_execv(paths[i], X_PRIO, stdin, stdout, stderr,
				 nargs, args);
		if (pid != SYSERR) {
			resume(pid);
			return(OK);
		}
	}
	fprintf(stderr,"Cannot find X server binary\n");
	return(SYSERR);
}
	


