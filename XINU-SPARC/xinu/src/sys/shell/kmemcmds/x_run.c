/* x_run.c */

/* 
   this command will be replaced by searching path so that we may run a
   user program by its name (and arguments) */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <vmem.h>
#include <shell.h>

/*------------------------------------------------------------------------
 *  x_run
 *------------------------------------------------------------------------

 */
COMMAND	x_run(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	int pid;
	
	pid = xinu_execv(args[1], 20, stdin, stdout, stderr,
			 nargs-1, &(args[1]));
	if (pid != SYSERR) {
		resume(pid);
		receive();
	}
	else {
		fprintf(stderr, "Error executing program %s\n", args[1]);
		return(SYSERR);
	}
	return(OK);
}
