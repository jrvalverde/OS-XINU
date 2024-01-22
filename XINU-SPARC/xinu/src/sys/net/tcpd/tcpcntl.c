/* tcpcntl.c - tcpcntl */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>
#ifdef XSELECT
#include <proc.h>
#include <mark.h>
#include <ports.h>
#endif

/*#define DEBUG*/
/*#define DEBUG_NBIO*/

/*------------------------------------------------------------------------
 *  tcpcntl - control function for TCP pseudo-devices
 *------------------------------------------------------------------------
 */
int tcpcntl(pdev, func, arg, arg2)
     struct	devsw	*pdev;
     int		func;
     char		*arg, *arg2;
{
    struct	tcb	*ptcb = (struct tcb *)pdev->dvioblk;
    int		rv;
#ifdef XSELECT
    int holderpid;
    int freelen;
#endif
    
#ifdef DEBUG
    kprintf("tcpcntl(dev:%d,0x%x,0x%x,0x%x) called\n",
	    pdev->dvnum,func,arg,arg2);
#endif
    
    if (ptcb == (struct tcb *)NULL || ptcb->tcb_state == TCPS_FREE)
	return SYSERR;
    
    wait(ptcb->tcb_mutex);
    if (ptcb->tcb_state == TCPS_FREE) /* verify no state change */
	return SYSERR;
    
    switch (func) {
      case TCPC_ACCEPT:
	if (ptcb->tcb_type != TCPT_SERVER) {
	    rv = SYSERR;
	    break;
	}
	signal(ptcb->tcb_mutex);
#ifdef DEBUG
	kprintf("tcpcntl: calling preceive...\n");
#endif
	return preceive(ptcb->tcb_listenq);
      case TCPC_LISTENQ:
	rv = tcplq(ptcb, arg);
	break;
      case TCPC_STATUS:
	rv = tcpstat(ptcb, arg);
	break;
      case TCPC_SOPT:
      case TCPC_COPT:
	rv = tcpuopt(ptcb, func, arg);
	break;
      case IONREAD:
	*((int *) arg) = ptcb->tcb_rbcount;
	rv = OK;
	break;
	
      case IONWRITE:
	*((int *) arg) = ptcb->tcb_sbsize - ptcb->tcb_sbcount;
	rv = OK;
	break;
	
#ifdef XSELECT
      case SETNBIO:
	ptcb->tcb_nbio     = TRUE;
#ifdef DEBUG_NBIO
	kprintf("tcpcntl: setting nbio mode for TCP device %d to %s\n",
		pdev->dvnum, ptcb->tcb_nbio?"TRUE":"FALSE");
#endif DEBUG_NBIO
	rv = OK;
	break;
	
      case UNSETNBIO:
	ptcb->tcb_nbio     = FALSE;
#ifdef DEBUG_NBIO
	kprintf("tcpcntl: setting nbio mode for TCP device %d to %s\n",
		pdev->dvnum, ptcb->tcb_nbio?"TRUE":"FALSE");
#endif DEBUG_NBIO
	rv = OK;
	break;
	
      case NOTIFYW:
	holderpid = ptcb->tcb_writepid;
	
#ifdef DEBUG
	kprintf("tcpcntl: setting writepid to %d, holder = %d\n",
		(int) arg, holderpid);
#endif	    
	if (((int) arg != holderpid) && (!isbadpid(holderpid))) {
	    rv = SYSERR;
	    break;
	}
	else
	    holderpid = ptcb->tcb_writepid = (int) arg;
	
	/*
	 * Code to check if output can be done is swiped from
	 * tcpwakeup() on the recommendation of dls. -- sjc
	 */
	if ( ptcb->tcb_type == TCPT_CONNECTION ) {
	    if ( ptcb->tcb_state > TCPS_ESTABLISHED )
		rv = IOREADY;
	    else if ( ptcb->tcb_state == TCPS_ESTABLISHED ) {
		freelen = ptcb->tcb_sbsize - ptcb->tcb_sbcount;
		if ((ptcb->tcb_flags & TCBF_SDONE) || (freelen > 0))
		    rv = IOREADY;
		else 
		    rv = BLOCKERR;
	    } else {
		ptcb->tcb_writepid = BADPID;
		rv = SYSERR;
	    }
	}
	else {
	    ptcb->tcb_writepid = BADPID;
	    rv = SYSERR;
	}
	break;
	
      case UNNOTIFYW:
	holderpid = ptcb->tcb_writepid; 
	if (getpid() != holderpid) 
	    rv = SYSERR;
	else  {
	    ptcb->tcb_writepid = BADPID;
	    rv = OK;
	}
	break;
	
      case NOTIFYR:
	holderpid = ptcb->tcb_readpid;
	
#ifdef DEBUG
	kprintf("tcpcntl: setting readpid to %d, holder = %d.\n",
		(int) arg, holderpid);
#endif	    
	if (((int) arg != ptcb->tcb_readpid) && (!isbadpid(holderpid))) {
	    rv = SYSERR;
	    break;
	}
	else 
	    holderpid = ptcb->tcb_readpid = (int) arg;
	
	/*
	 * Code to check for data is swiped from tcpwakeup(). -- sjc
	 */
	
	if (ptcb->tcb_type == TCPT_SERVER) {
	    /*
	     * a server has IOREADY if there is someone waiting in the
	     * listen queue.  The ptrsem of the listen queue port tells
	     * us how many messages are waiting there.
	     */
#ifdef DEBUG
	    kprintf("tcpcntl: NOTIFYR, %d outstanding connections.\n",
		    scount(ports[ptcb->tcb_listenq].ptrsem));
#endif
	    if (scount(ports[ptcb->tcb_listenq].ptrsem) > 0) 
		rv = IOREADY;
	    else
		rv = BLOCKERR;
	} else if ( ptcb->tcb_type == TCPT_CONNECTION ) {
	    if ( ptcb->tcb_state > TCPS_ESTABLISHED )
		rv = IOREADY;
	    else if ( ptcb->tcb_state == TCPS_ESTABLISHED ) {
		if (((ptcb->tcb_flags & TCBF_RDONE) ||
		     ptcb->tcb_rbcount > 0 ||
		     ptcb->tcb_rudq >= 0)) 
		    rv = IOREADY;
		else 
		    rv = BLOCKERR;
	    } else {
		rv = SYSERR;
		ptcb->tcb_readpid = BADPID;
	    }
	} else {
	    ptcb->tcb_readpid = BADPID;
	    rv = SYSERR;
	}
	break;
	
      case UNNOTIFYR:
	holderpid = ptcb->tcb_readpid; 
	if (getpid() != holderpid)
	    rv = SYSERR;
	else {
	    rv = OK;
	    ptcb->tcb_readpid = BADPID;
	}
	break;
#endif
      case TCPC_SENDURG:
	/*
	 * tcpwr acquires and releases tcb_mutex
	 * itself.
	 */
	signal(ptcb->tcb_mutex);
	return tcpwr(pdev, arg, arg2, TWF_URGENT);
      default:
	rv = SYSERR;
    }
    signal(ptcb->tcb_mutex);
#ifdef DEBUG
    kprintf("tcpcntl(dev:%d,0x%x,0x%x,0x%x) returns %d\n",
	    pdev->dvnum,func,arg,arg2,rv);
#endif
    return rv;
}
