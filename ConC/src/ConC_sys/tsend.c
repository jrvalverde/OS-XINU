/* tsend.c - tsend */

#include <kernel.h>
#include <proc.h>
#include <errno.h>

/*------------------------------------------------------------------------
 *  tsend  --  send a message to another process
 *------------------------------------------------------------------------
 */
SYSCALL	tsend(pid, msg)
int	pid;
int	msg;
{
	struct	pentry	*pptr;		/* receiver's proc. table addr.	*/
	long	ps;

	disable(ps);
	if (isbadpid(pid) || ( (pptr= &proctab[pid])->pstate == PRFREE)
	   || pptr->phasmsg != 0) {
		errno = (pptr -> phasmsg != 0) ? CEHASMSG : CEBADTID;
		restore(ps);
		return(SYSERR);
	}
	pptr->pmsg = msg;		/* deposit message		*/
	pptr->phasmsg++;
	if (pptr->pstate == PRRECV)	/* if receiver waits, start it	*/
		ready(pid, RESCHYES);
	restore(ps);

	return(OK);
}
