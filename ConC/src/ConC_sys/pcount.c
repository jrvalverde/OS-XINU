/* pcount.c - pcount */

#include <kernel.h>
#include <ports.h>
#include <errno.h>

/*------------------------------------------------------------------------
 *  pcount  --  return the count of current messages in a port
 *------------------------------------------------------------------------
 */
SYSCALL	pcount(portid)
int	portid;
{
	int	scnt;
	int	count;
	long	ps;
	struct	pt	*ptptr;

	disable(ps);

	errno = 0;	/* to distinquish between error and valid -1 pcount */

	if ( isbadport(portid) || !ptmark ||
		(ptptr= &ports[portid])->ptstate != PTALLOC ) {
			errno = CEBADPT;
			restore(ps);
			return(SYSERR);
	}
	count = scount(ptptr->ptrsem);
	if ( (scnt=scount(ptptr->ptssem)) < 0 )
		count -= scnt;			/* add number waiting	*/
	restore(ps);

	return(count);
}
