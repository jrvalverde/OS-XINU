/* x_args.c - x_args */

#include <conf.h>
#include <kernel.h>
#include <vmem.h>

/*------------------------------------------------------------------------
 *  x_args  -  test argument passing
 *------------------------------------------------------------------------
 */
COMMAND	x_args(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	int i;
	
	kprintf("stdin:  %d\n", stdin);
	kprintf("stdout: %d\n", stdout);
	kprintf("stderr: %d\n", stderr);
	kprintf("nargs:  %d\n", nargs);

#ifdef DUMP_STACK
	{
		int *pint;
		for (pint = &stdin, i=0;
		     pint <= (int *) vmaddrsp.stkstrt; ++i,++pint) {
			kprintf("%2d: 0x%x: 0x%x\n",
				i,pint,*pint);
		}
	}
#endif

	for (i=0; i < nargs; ++i) {
		kprintf("   arg %2d: %s\n", i, args[i]);
	}
}
