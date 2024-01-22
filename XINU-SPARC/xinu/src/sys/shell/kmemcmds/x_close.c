/* x_close.c - x_close */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  x_close  -  (command close) close a device given its id
 *------------------------------------------------------------------------
 */
COMMAND	x_close(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	int i;
	
	if (nargs < 2) {
		fprintf(stderr, "use: close device-number [device-number]*\n");
		return(SYSERR);
	}
	for (i=1 ; i<nargs ; i++)
	    close(atoi(args[i]));
	return(OK);
}
