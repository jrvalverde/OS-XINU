/* treceive.c - treceive */

#include <kernel.h>
#include <proc.h>

/*------------------------------------------------------------------------
 *  treceive  -  wait for a message and return it
 *------------------------------------------------------------------------
 */
SYSCALL	treceive()
{
	struct	pentry	*pptr;
	int	msg;
	long	ps;

	disable(ps);
	pptr = &proctab[currpid];
	if (pptr->phasmsg == 0) {	/* if no message, wait for one	*/
		pptr->pstate = PRRECV;
		resched();
	}
	msg = pptr->pmsg;		/* retrieve message		*/
	pptr->phasmsg = 0;
	restore(ps);

	return(msg);
}
