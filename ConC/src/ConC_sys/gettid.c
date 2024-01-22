/* gettid.c - gettid */

#include <kernel.h>
#include <proc.h>

/*------------------------------------------------------------------------
 * gettid  --  get the process id of currently executing process
 *------------------------------------------------------------------------
 */
SYSCALL gettid()
{
	return(currpid);
}
