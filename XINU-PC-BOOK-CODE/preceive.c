/* preceive.c - preceive */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <ports.h>

/*------------------------------------------------------------------------
 *  preceive  --  receive a message from a port, blocking if port empty
 *------------------------------------------------------------------------
 */

SYSCALL preceive(portid)
int portid;
{
	int	ps;
	struct	pt	*ptptr;
	int	seq;
	int	msg;
	struct	ptnode	*nxtnode;

	disable(ps);
	if ( isbadport(portid) ||
#ifdef MEMMARK
		unmarked(ptmark) ||
#endif
		(ptptr=&ports[portid])->ptstate != PTALLOC ) {
		restore(ps);
		return(SYSERR);
	}

	/* wait for a msg. and verify that the port is still allocated */

	seq = ptptr->ptseq;
	if ( wait(ptptr->ptrsem) == SYSERR
	    || ptptr->ptstate  != PTALLOC
	    || ptptr->ptseq != seq ) {
		restore(ps);
		return(SYSERR);
	}

	/* dequeue the first message that is waiting in the port */

	nxtnode = ptptr->pthead;
	msg = nxtnode->ptmsg;
	if (ptptr->pthead == ptptr->pttail)	/* delete the last item */
		ptptr->pthead = ptptr->pttail = (struct ptnode *)NULL;
	else
		ptptr->pthead = nxtnode->ptnext;
	nxtnode->ptnext = ptfree;		/* return to free list */
	ptfree = nxtnode;
	ptptr->ptct--;
	signal(ptptr->ptrsem);
	restore(ps);
	return(msg);
}
