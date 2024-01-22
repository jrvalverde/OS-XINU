/* rfsend.c - rfsend */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*#define DEBUG*/
#define PRINTERRORS


/*------------------------------------------------------------------------
 *  rfsend  --  send message to remote server and await reply
 *------------------------------------------------------------------------
 */
rfsend(fptr, reqlen, rplylen)
struct	fphdr	*fptr;
int	reqlen;
int	rplylen;
{
	STATWORD ps;    
	int	trys;
	int	ret;

	/* Clear server queue, and send packet to it */

	if (Rf.device == RCLOSED) {
		Rf.device = open(UDP, RSERVER, ANYLPORT);
		if (Rf.device == SYSERR ||
		    control(Rf.device, DG_SETMODE, DG_DMODE|DG_TMODE)
		    == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
			kprintf("rfsend: couldn't open UDP, return SYSERR\n");
#endif
			return(SYSERR);
		}
	}
	disable(ps);
	control(Rf.device, DG_CLEAR);
	for (trys=0 ; trys<RMAXTRY ; trys++) {
#ifdef DEBUG
		kprintf("rfsend: sending RFS request:\n");
		kprintf("   pos: %d  count: %d  op: %d name: %s\n",
			net2hl(fptr->f_pos),
			net2hs(fptr->f_count),
			net2hs(fptr->f_op),
			fptr->f_name);
#endif
		if ( write(Rf.device, fptr, reqlen)
		    == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
			kprintf("rfsend: write failed, return SYSERR\n");
#endif
			restore(ps);
			return(SYSERR);
		}
		if ( (ret=read(Rf.device, fptr, rplylen) )
		    !=SYSERR && ret != TIMEOUT) {
#ifdef DEBUG
			kprintf("rfsend: got  RFS reply:\n");
			kprintf("   pos: %d  count: %d  op: %d name: %s\n",
				net2hl(fptr->f_pos),
				net2hs(fptr->f_count),
				net2hs(fptr->f_op),
				fptr->f_name);
			kprintf("rfsend: returning %d\n",ret);
#endif
			restore(ps);
			return(ret);
		}
	}
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rfsend: too many retries, return SYSERR\n");
#endif
	restore(ps);
	return(SYSERR);
}
