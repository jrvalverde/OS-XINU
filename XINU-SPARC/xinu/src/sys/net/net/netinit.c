/* netinit.c - netinit */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <sleep.h>

/*------------------------------------------------------------------------
 *  netinit  -  initialize network data structures
 *------------------------------------------------------------------------
 */
netinit()
{
    /* Initialize pool of network buffers and rest of Net structure	*/
    
    if (clkruns == FALSE)
	panic("net: no clock");

    if ((Net.netpool = mkdpool(MAXNETBUF, NETBUFS)) == SYSERR)
	panic("netinit(): can't get net buffer pool");
    
    if ((Net.lrgpool = mkdpool(MAXLRGBUF, LRGBUFS)) == SYSERR)
	panic("netinit(): can't get large buffer pool");

    /* initialize the routing information */
    rtinit();
    return(OK);
}


