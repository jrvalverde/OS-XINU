/* psend.c - psend */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <ports.h>

/*------------------------------------------------------------------------
 *  psend  --  send a message to a port by enqueuing it
 *------------------------------------------------------------------------
 */

SYSCALL psend(portid, msg)
int portid;
int msg;
{
	int	ps;
	struct	pt	*ptptr;
	int	seq;
	struct	ptnode	*freenode;

	disable(ps);
	if ( isbadport(portid) ||
#ifdef	MEMMARK
		unmarked(ptmark) ||
#endif
		(ptptr=&ports[portid])->ptstate != PTALLOC ) {
		restore(ps);
		return(SYSERR);
	}

	/* wait for space and verify that the port is still allocated */

	seq = ptptr->ptseq;
	ptptr->ptct++;
	if (wait(ptptr->ptssem) == SYSERR
		|| ptptr->ptstate != PTALLOC
		|| ptptr->ptseq != seq ) {
			restore(ps);
			return(SYSERR);
		}
	if ( ptfree == (struct ptnode *)NULL ) {
		kprintf("psend - out of nodes");
		xdone();
	}
	freenode = ptfree;
	ptfree = freenode->ptnext;
	freenode->ptnext = (struct ptnode *)NULL;
	freenode->ptmsg = msg;
	if ( ptptr->pttail == (struct ptnode *)NULL )	/* empty queue */
		ptptr->pttail = ptptr->pthead = freenode;
	else {
		(ptptr->pttail)->ptnext = freenode;
		ptptr->pttail = freenode;
	}
	signal(ptptr->ptrsem);
	restore(ps);
	return(OK);
}
