/* tcpmopen.c - tcpmopen */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>
#include <proc.h>

/*------------------------------------------------------------------------
 *  tcpmopen  -  open a fresh TCP pseudo device and return descriptor
 *------------------------------------------------------------------------
 */
int tcpmopen(pdev, fport, lport)
     struct	devsw	*pdev;
     char		*fport;
     int		lport;
{
    struct	tcb	*ptcb;
    int		error;

#ifdef DEBUG
    kprintf("tcpmopen(pdev,%d,%d) called\n", fport, lport);
#endif
    ptcb = (struct tcb *)tcballoc();
    if (ptcb == (struct tcb *)SYSERR)
	return SYSERR;

    ptcb->tcb_error = 0;
    proctab[currpid].ptcpumode = FALSE;	/* urgent mode */
    if (fport == ANYFPORT)
	return tcpserver(ptcb, lport);
    
#ifdef DEBUG
    kprintf("tcpmopen, binding\n");
#endif
    if (tcpbind(ptcb, fport, lport) != OK || tcpsync(ptcb) != OK) {
	ptcb->tcb_state = TCPS_FREE;
	sdelete(ptcb->tcb_mutex);
	return SYSERR;
    }
    
    if (error = tcpcon(ptcb))
	return error;

#ifdef DEBUG
    kprintf("tcpmopen, returning dev %d\n", ptcb->tcb_dvnum);
#endif
    return ptcb->tcb_dvnum;
}
