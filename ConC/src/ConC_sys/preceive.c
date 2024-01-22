/* preceive.c - preceive */

#include <kernel.h>
#include <ports.h>
#include <errno.h>

/*------------------------------------------------------------------------
 *  preceive  --  receive a message from a port, blocking if port empty
 *------------------------------------------------------------------------
 */
char	*preceive(portid)
int portid;
{
	long	ps;
	struct	pt	*ptptr;
	int	seq;
	int	msg;
	struct	ptnode	*nxtnode;

	disable(ps);
	if ( isbadport(portid) || !ptmark ||
	     (ptptr= &ports[portid])->ptstate != PTALLOC ) {
		errno = CEBADPT;
		restore(ps);
		return(NULL);
	}

	/* wait for message and verify that the port is still allocated */

	seq = ptptr->ptseq;
	if (swait(ptptr->ptrsem) == SYSERR || ptptr->ptstate != PTALLOC
	    || ptptr->ptseq != seq) {
		errno = CEBADPT;
		restore(ps);
		return(NULL);
	}

	/* dequeue first message that is waiting in the port */

	nxtnode = ptptr->pthead;
	msg = nxtnode->ptmsg;
	if (ptptr->pthead == ptptr->pttail)	/* delete last item	*/
		ptptr->pthead = ptptr->pttail = (struct ptnode *)NULL;
	else
		ptptr->pthead = nxtnode->ptnext;
	nxtnode->ptnext = ptfree;		/* return to free list	*/
	ptfree = nxtnode;
	ssignal(ptptr->ptssem);
	restore(ps);

	return((char *)msg);
}
