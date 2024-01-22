/* tcpnxtp.c - tcpnxtp */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>

#define	IPPORT_RESERVED		1024	/* from BSD */

/*------------------------------------------------------------------------
 *  tcpnxtp  -  return the next available TCP local "port" number
 *------------------------------------------------------------------------
 */
tcpnxtp()
{
    static	short	lastport = 1;	
    int		i, start;
    
    wait(tcps_tmutex);
    for (start=lastport++; start != lastport; ++lastport) {
	if (lastport == IPPORT_RESERVED)
	    lastport = 1;
	
	for (i=0; i<Ntcp; ++i)
	    if (tcbtab[i].tcb_state != TCPS_FREE &&
		tcbtab[i].tcb_lport == lastport)
		break;
	if (i == Ntcp)
	    break;
    }
    if (lastport == start)
	panic("out of TCP ports");
    signal(tcps_tmutex);
    return lastport;
}
