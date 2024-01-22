/* trecvclr.c - trecvclr */

#include <kernel.h>
#include <proc.h>

/*------------------------------------------------------------------------
 *  trecvclr  --  clear messages, returning waiting message (if any)
 *------------------------------------------------------------------------
 */
SYSCALL	trecvclr()
{
	long	ps;
	int	msg;

	disable(ps);
	if (proctab[currpid].phasmsg) {		/* existing message?	*/
		proctab[currpid].phasmsg = 0;
		msg = proctab[currpid].pmsg;
	} else
		msg = OK;
	restore(ps);

	return(msg);
}
