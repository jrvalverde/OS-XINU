/* preset.c - preset */

#include <kernel.h>
#include <ports.h>
#include <errno.h>

/*------------------------------------------------------------------------
 *  preset  --  reset a port, freeing waiting processes and messages
 *------------------------------------------------------------------------
 */
SYSCALL	preset(portid, dispose)
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
	_ptclear(ptptr, PTALLOC, dispose);
	restore(ps);

	return(OK);
}
