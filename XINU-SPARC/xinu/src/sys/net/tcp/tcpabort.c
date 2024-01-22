/* tcpabort.c - tcpabort */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>

/*------------------------------------------------------------------------
 *  tcpabort -  abort an active TCP connection
 *------------------------------------------------------------------------
 */
int tcpabort(ptcb, error)
     struct	tcb	*ptcb;
     int		error;
{
    tcpkilltimers(ptcb);	/* delete all pending events */
    ptcb->tcb_flags |= TCBF_RDONE|TCBF_SDONE;
    ptcb->tcb_error = error;
    tcpwakeup(READERS|WRITERS, ptcb);
    return OK;
}
