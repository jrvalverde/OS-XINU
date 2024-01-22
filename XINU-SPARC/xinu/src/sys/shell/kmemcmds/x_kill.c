/* x_kill.c - x_kill */

#include <conf.h>
#include <kernel.h>
#include <shell.h>

/*------------------------------------------------------------------------
 *  x_kill  -  (command kill) terminate a process
 *------------------------------------------------------------------------
 */
BUILTIN	x_kill(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	int	pid;
	int	i;

	if (nargs < 2) {
		fprintf(stderr, "use: kill process-id [process-id]*\n");
		return(SYSERR);
	}
	
	for (i=1 ; i<nargs ; i++) {
		if ( (pid=atoi(args[i])) == getpid() )
		    fprintf(stderr, "Shell killed\n");
		kill(pid);
	}
	return(OK);
}
