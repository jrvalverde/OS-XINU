/* udp_in.c - udp_in */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>

#define PRINTERRORS
/*#define PRINT_BAD_PACKETS*/
/*#define DEBUG*/

int	udpmutex;

/*------------------------------------------------------------------------
 *  udp_in -  handle an inbound UDP datagram
 *------------------------------------------------------------------------
 */
int udp_in(pni, pep)
     struct	netif	*pni;
     struct	ep	*pep;
{
    struct	ip	*pip = (struct ip *)pep->ep_data;
    struct	udp	*pudp = (struct udp *)pip->ip_data;
    struct	upq	*pup;
    unsigned short	dst;
    int		i;
    
#ifdef DEBUG
    kprintf("udp_in(pni, pep) called\n");
#endif
    if (pudp->u_cksum && udpcksum(pip)) {
#if defined(PRINTERRORS) || defined(DEBUG)
	kprintf("udp_in(pni,pep) checksum error, discarding:\n");
#ifdef PRINT_BAD_PACKETS
	ipdump(pep);
	kprintf("   packet cksum: 0x%04x", pudp->u_cksum);
	pudp->u_cksum = 0;
	kprintf("   should be: 0x%04x\n", udpcksum(pip));
#endif
#endif
	freebuf(pep);
	return SYSERR;		/* checksum error */
    }
    udpnet2h(pudp);		/* convert UDP header to host order */
    dst = pudp->u_dst;
    wait(udpmutex);
    for (i=0; i<UPPS ; i++) {
	pup = &upqs[i];
	if (pup->up_port == dst) {
	    /* drop instead of blocking on psend */ 
	    if (pcount(pup->up_xport) >= UPPLEN) {
#ifdef PRINTERRORS
		kprintf("!! udp_in: queue to long. Drop packet!\n");
#endif
		signal(udpmutex);
		freebuf(pep);
		UdpInErrors++;
		return SYSERR;
	    }
	    psend(pup->up_xport, pep);
	    UdpInDatagrams++;
	    if (!isbadpid(pup->up_pid)) {
		send(pup->up_pid, OK);
		pup->up_pid = BADPID;
	    }
	    signal(udpmutex);

#ifdef DEBUG
	    kprintf("** udp_in: dest port %d\n", dst);
#endif
	    return OK;
	}
    }

    signal(udpmutex);
    UdpNoPorts++;
    udph2net(pudp);		/* convert back to net order for error */
    icmp(ICT_DESTUR, ICC_PORTUR, pip->ip_src, pep);
#ifdef DEBUG
    kprintf("udp_in: exit\n");
#endif
    return OK;
}

