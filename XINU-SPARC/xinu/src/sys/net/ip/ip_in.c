/* ip_in.c - ip_in */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*#define DEBUG*/
#define PRINTERRORS

/*------------------------------------------------------------------------
 *  ip_in - IP input function
 *------------------------------------------------------------------------
 */
int ip_in(pni, pep)
     struct	netif	*pni;
     struct	ep	*pep;
{
    struct	ip	*pip = (struct ip *)pep->ep_data;
    
#ifdef DEBUG
    kprintf("ip_in: pep=0x%08x (pool id=%d)\n", pep, *((int *)pep - 1));
#endif
    
    IpInReceives++;
    if (enq(pni->ni_ipinq, pep, pip->ip_tos & IP_PREC) < 0) {
#ifdef PRINTERRORS
	kprintf("ip_in: couldn't enq(), dropping (pep=0x%08x)\n", pep);
#endif 
	IpInDiscards++;
	freebuf(pep);
    }

    /* Ethernet interrupt handler (ethinter()) will send a message to IP */
    /* process once packets are received.				 */
    return OK;
}
