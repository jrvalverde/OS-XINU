/* psend.c - psend */

#include <kernel.h>
#include <ports.h>
#include <errno.h>

/*------------------------------------------------------------------------
 *  psend  --  send a message to a port by enqueuing it
 *------------------------------------------------------------------------
 */
SYSCALL	psend(portid, msg)
int	portid;
int	msg;
{
	long	ps;
	struct	pt	*ptptr;
	int	seq;
	struct	ptnode	*freenode;

	disable(ps);
	if ( isbadport(portid) || !ptmark ||
	     (ptptr= &ports[portid])->ptstate != PTALLOC ) {
		errno = CEBADPT;
		restore(ps);
		return(SYSERR);
	}

	/* wait for space and verify port is still allocated */

	seq = ptptr->ptseq;
	if (swait(ptptr->ptssem) == SYSERR
	    || ptptr->ptstate != PTALLOC
	    || ptptr->ptseq != seq) {
		errno = CEBADPT;
		restore(ps);
		return(SYSERR);
	}
	if (ptfree == (struct ptnode *)NULL)
		panic("Ports  -  out of nodes");
	freenode = ptfree;
	ptfree  = freenode->ptnext;
	freenode->ptnext = (struct ptnode *)NULL;
	freenode->ptmsg  = msg;
	if (ptptr->pttail == (struct ptnode *)NULL)	/* empty queue */
		ptptr->pttail = ptptr->pthead = freenode;
	else {
		(ptptr->pttail)->ptnext = freenode;
		ptptr->pttail = freenode;
	}
	ssignal(ptptr->ptrsem);
	restore(ps);

	return(OK);
}
