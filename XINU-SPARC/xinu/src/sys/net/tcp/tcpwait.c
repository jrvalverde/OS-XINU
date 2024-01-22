/* tcpwait.c - tcpwait */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>

/*------------------------------------------------------------------------
 *  tcpwait - (re)schedule a DELETE event for 2MSL from now
 *------------------------------------------------------------------------
 */
int tcpwait(ptcb)
     struct	tcb	*ptcb;
{
    int	tcbnum = ptcb - &tcbtab[0];
    
    tcpkilltimers(ptcb);	/* kill all outstanding timers for a TCB */
    tmset(tcps_oport, TCPQLEN, MKEVENT(DELETE, tcbnum), TCP_TWOMSL);
    return OK;
}
