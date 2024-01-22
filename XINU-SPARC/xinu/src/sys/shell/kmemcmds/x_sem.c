/* x_sem.c - x_sem */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  x_sem - print the semaphore table
 *------------------------------------------------------------------------
 */
COMMAND
x_sem(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	int sem;

	if (nargs > 2) {
		fprintf(stderr, "use: sem [semaphore_num]\n");
		return(SYSERR);
	} else if (nargs == 2)
	    sem = atoi(args[1]);
	else
	    sem = -1;

	prsem(sem);

	return(OK);
}
