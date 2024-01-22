/* rarpsend.c - rarpsend */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  rarpsend  -  broadcast a RARP packet to obtain my IP address
 *------------------------------------------------------------------------
 */
int rarpsend(ifn)
     int	ifn;		/* the interface number */
{
    STATWORD	ps;
    static struct ep	*mkrarp();
    struct	netif	*pni = &nif[ifn];
    struct	ep	*pep;
    int			i, mypid, resp;

    mypid = getpid();
    
    for (i=0; i<ARP_MAXRETRY; ++i) {
	pep = mkrarp(ifn);
	if (pep == (struct ep *)SYSERR)
	    break;
	
	disable(ps);
	rarppid = mypid;
	restore(ps);
	
	recvclr();
	write(pni->ni_dev, pep, EP_MINLEN);
	/* ARP_RESEND is in secs, recvtim uses 1/10's of secs	*/
	resp = recvtim(10*ARP_RESEND<<i);
	
	if (resp != TIMEOUT) {
	    if (ifn != NI_PRIMARY) {
		/* For NI_PRIMARY interface, we set them in netstart() */
		/* host route */
		rtadd(pni->ni_ip, ip_maskall, pni->ni_ip, 0, NI_LOCAL, RT_INF);
		
		/* non-subnetted route */
		rtadd(pni->ni_net, ip_maskall, pni->ni_ip, 0,NI_LOCAL, RT_INF);
	    }
	    return OK;
	}
    }
    panic("No response to RARP");
    return SYSERR;
}


/*------------------------------------------------------------------------
 *  mkrarp  -  allocate and fill in an RARP packet
 *------------------------------------------------------------------------
 */
static struct ep *mkrarp(ifn)
int	ifn;
{
    register struct	arp	*parp;
    struct	ep	*pep;
    
    pep = (struct ep *) getbuf(Net.netpool);
    if (pep == (struct ep *)SYSERR)
	return (struct ep *)SYSERR;

    blkcopy(pep->ep_dst, nif[ifn].ni_hwb.ha_addr, EP_ALEN);
    pep->ep_type = EPT_RARP;
    parp = (struct arp *)pep->ep_data;
    parp->ar_hwtype = hs2net(AR_HARDWARE);
    parp->ar_prtype = hs2net(EPT_IP);
    parp->ar_hwlen = EP_ALEN;
    parp->ar_prlen = IP_ALEN;
    parp->ar_op = hs2net(RA_REQUEST);
    blkcopy(SHA(parp), nif[ifn].ni_hwa.ha_addr, EP_ALEN);
    bzero(SPA(parp), IP_ALEN);
    blkcopy(THA(parp), nif[ifn].ni_hwa.ha_addr, EP_ALEN);
    bzero(TPA(parp), IP_ALEN);
    return pep;
}
