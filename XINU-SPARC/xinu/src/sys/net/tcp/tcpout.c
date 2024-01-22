/* tcpout.c - tcpout */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>

/*#define DEBUG_TCPOUT*/
/*#define DEBUG*/
#define SJCDEBUG

/*------------------------------------------------------------------------
 *  tcpout - handle events affecting TCP output processing
 *------------------------------------------------------------------------
 */
PROCESS tcpout()
{
    struct	tcb	*ptcb;
    int		i;
    
    tcps_oport = pcreate(NETBUFS);
    signal(Net.sema);		/* synchronize on startup	*/
    
    while (TRUE) {
	i = preceive(tcps_oport);
#ifdef DEBUG_TCPOUT
	if (tcpout_checkm(tcps_oport,i) != OK)
	    continue;
#endif DEBUG_TCPOUT
#ifdef DEBUG
	kprintf("tcpout: tcb:%d event:%d\n",  TCB(i), EVENT(i));
#endif
#ifdef SJCDEBUG
	rec_tcp_seg(0, 1, "TCPOUT: tcb:%d event:%d", TCB(i), EVENT(i));
#endif		
	ptcb = &tcbtab[TCB(i)];
	if (ptcb->tcb_state <= TCPS_CLOSED) {
#ifdef DEBUG
	    kprintf("tcpout:   skipping, == CLOSED\n");
#endif
	    continue;		/* a rogue; ignore it	*/
	}
	wait(ptcb->tcb_mutex);
	if (ptcb->tcb_state <= TCPS_CLOSED)
	    continue;		/* TCB deallocated	*/
	if (EVENT(i) == DELETE)		/* same for all states	*/
	    tcbdealloc(ptcb);
	else {
	    tcposwitch[ptcb->tcb_ostate](TCB(i), EVENT(i));
	}
	
	if (ptcb->tcb_state != TCPS_FREE)
	    signal(ptcb->tcb_mutex);
    }
}


#ifdef DEBUG_TCPOUT
#include <proc.h>

tcpout_checkm(port,i)
{
    if (port != tcps_oport)
	return(OK);
    
    if ((TCB(i) < 0) || (TCB(i) >= Ntcp)) {
	kprintf("PROCESS tcpout: bad msg:%d, tcb:%d evnt:%d\n",
		i, TCB(i), EVENT(i));
	panic("bad tcpout message");
	return(SYSERR);
    }
    return(OK);
}
#endif DEBUG_TCPOUT
