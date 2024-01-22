/*------------------------------------------------------------------------
 *	panic  --  panic and abort XINU
 *------------------------------------------------------------------------
 */

#include <kernel.h>    
#include <proc.h>

static unsigned long	esp, ebp;

panic (msg)
char *msg;
{
	STATWORD ps;    
	disable(ps);
	kprintf("currpid %d (%s)\n", currpid, proctab[currpid].pname);
	kprintf("Panic: %s\n", msg);
/*
	asm("movl	%esp,esp");
	asm("movl	%ebp,ebp");
	stacktrace(esp, ebp);
	stop("(panic)\n");
*/
	while (1)
		monitor();
	restore(ps);
}

