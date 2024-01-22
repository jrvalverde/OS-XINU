/* poolinit.c - poolinit */

#include <kernel.h>
#include <bufpool.h>

struct	bpool	bptab[NBPOOLS];
int	nbpools;

/*------------------------------------------------------------------------
 *  poolinit  --  initialize the buffer pool routines
 *------------------------------------------------------------------------
 */
poolinit()
{
	long	ps;

	if ( bpmark )
		return(SYSERR);

	disable(ps);
	nbpools = 0;
	bpmark = TRUE;
	restore(ps);

	return(OK);
}
