/* tcpserver.c - tcpserver */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>

/*------------------------------------------------------------------------
 *  tcpserver - do a TCP passive open
 *------------------------------------------------------------------------
 */
int tcpserver(ptcb, lport)
     struct	tcb	*ptcb;
     int		lport;
{
#ifdef DEBUG
    kprintf("tcpserver(ptcb,%d) called\n", lport);
#endif
    if (lport == ANYLPORT) {
	ptcb->tcb_state = TCPS_FREE;
	sdelete(ptcb->tcb_mutex);
	return SYSERR;
    }
    ptcb->tcb_type = TCPT_SERVER;
    ptcb->tcb_lport = lport;
    ptcb->tcb_state = TCPS_LISTEN;
    ptcb->tcb_lqsize = tcps_lqsize;
    ptcb->tcb_listenq = pcreate(ptcb->tcb_lqsize);
    ptcb->tcb_smss = 0;
    signal(ptcb->tcb_mutex);
#ifdef DEBUG
    kprintf("tcpserver returning dev %d\n", ptcb->tcb_dvnum);
#endif
    return ptcb->tcb_dvnum;
}
