/* x_create.c - x_creat */

#include <conf.h>
#include <kernel.h>
#include <a.out.h>

/*------------------------------------------------------------------------
 *  x_creat  -  (command create) create a process given a starting address
 *------------------------------------------------------------------------
 */
COMMAND	x_creat(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	kprintf("x_creat: obsolete\n");
	return(SYSERR);
}
