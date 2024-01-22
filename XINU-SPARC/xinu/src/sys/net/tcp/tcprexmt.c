/* tcprexmt.c - tcprexmt */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>

/*#define DEBUG*/

/*------------------------------------------------------------------------
 *  tcprexmt - handle TCP output events while we are retransmitting
 *------------------------------------------------------------------------
 */
int tcprexmt(tcbnum, event)
{
    struct	tcb	*ptcb = &tcbtab[tcbnum];

    if (event != RETRANSMIT)
	return OK;	/* ignore others while retransmitting	*/
    
    if (++ptcb->tcb_rexmtcount > TCP_MAXRETRIES) {
	tcpabort(ptcb, TCPE_TIMEDOUT);
	return OK;
    }
#ifdef DEBUG
    kprintf("tcprexmt: retransmitting suna:0x%x count:%d time:%d\n",
	    ptcb->tcb_suna,
	    ptcb->tcb_rexmtcount,
	    min(ptcb->tcb_rexmt<<ptcb->tcb_rexmtcount, TCP_MAXRXT));
#endif
    tcpsend(tcbnum, TSF_REXMT);
    tmset(tcps_oport, TCPQLEN, MKEVENT(RETRANSMIT, tcbnum),
	  min(ptcb->tcb_rexmt<<ptcb->tcb_rexmtcount, TCP_MAXRXT));
    
    if (ptcb->tcb_ostate != TCPO_REXMT)
	ptcb->tcb_ssthresh = ptcb->tcb_cwnd;	/* first drop	*/
    
    ptcb->tcb_ssthresh = min(ptcb->tcb_swindow, ptcb->tcb_ssthresh) >> 1;
    if (ptcb->tcb_ssthresh < ptcb->tcb_smss)
	ptcb->tcb_ssthresh = ptcb->tcb_smss;
    ptcb->tcb_cwnd = ptcb->tcb_smss;
    return OK;
}
