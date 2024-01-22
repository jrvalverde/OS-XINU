/* pdelete.c - pdelete */

#include <kernel.h>
#include <ports.h>
#include <errno.h>

/*------------------------------------------------------------------------
 *  pdelete  --  delete a port, freeing waiting processes and messages
 *------------------------------------------------------------------------
 */
SYSCALL	pdelete(portid, dispose)
	int	portid;
	int	(*dispose)();
{
	long	ps;
	struct	pt *ptptr;

	disable(ps);
	if ( isbadport(portid) || !ptmark ||
	     (ptptr= &ports[portid])->ptstate != PTALLOC ) {
		errno = CEBADPT;
		restore(ps);
		return(SYSERR);
	}
	_ptclear(ptptr, PTFREE, dispose);
	restore(ps);

	return(OK);
}
