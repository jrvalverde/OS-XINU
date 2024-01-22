/* getasid.c - getasid() */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

/*------------------------------------------------------------------------
 * getasid  --  get the address space id of currently executing process
 *------------------------------------------------------------------------
 */
SYSCALL getasid()
{
	return(proctab[currpid].asid);
}

