/* tcpkick.c - tcpkick */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>

/*------------------------------------------------------------------------
 *  tcpkick -  make sure we send a packet soon
 *------------------------------------------------------------------------
 */
int tcpkick(ptcb)
     struct	tcb	*ptcb;
{
    int	tcbnum = ptcb - &tcbtab[0];	/* for MKEVENT() */
    int	tv;

    tv = MKEVENT(SEND, tcbnum);
#ifdef SJCDEBUG
    rec_tcp_seg(0, 0, "TCPKICK called");
#endif
    if (ptcb->tcb_flags & TCBF_DELACK && !tmleft(tcps_oport, tv))
	tmset(tcps_oport, TCPQLEN, tv, TCP_ACKDELAY);
    else if (pcount(tcps_oport) < TCPQLEN)
	psend(tcps_oport, tv);	/* send now */
    
    return OK;
}
