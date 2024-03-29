/* tcpidle.c - tcpidle */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>

/*------------------------------------------------------------------------
 *  tcpidle - handle events while a connection is idle
 *------------------------------------------------------------------------
 */
int tcpidle(tcbnum, event)
     int	tcbnum;
     int	event;
{
    if (event == SEND)
	tcpxmit(tcbnum, event);
    return OK;
}
