/*------------------------------------------------------------------------
 *  mkarp  -  allocate and fill in an ARP or RARP packet
 *------------------------------------------------------------------------
 */
static struct ep *mkarp(ifn, type, op, spa, tpa)
int	ifn;
int	type;
int	op;
IPaddr	spa;
IPaddr	tpa;
{
    register struct	arp	*parp;
    struct	ep	*pep;
    
    pep = (struct ep *) getbuf(Net.netpool);
    if (pep == (struct ep *)SYSERR)
	return (struct ep *)SYSERR;
    
    blkcopy(pep->ep_dst, nif[ifn].ni_hwb.ha_addr, EP_ALEN);
    pep->ep_type = type;
    parp = (struct arp *)pep->ep_data;
    parp->ar_hwtype = hs2net(AR_HARDWARE);
    parp->ar_prtype = hs2net(EPT_IP);
    parp->ar_hwlen = EP_ALEN;
    parp->ar_prlen = IP_ALEN;
    parp->ar_op = hs2net(op);
    blkcopy(SHA(parp), nif[ifn].ni_hwa.ha_addr, EP_ALEN);
    IP_COPYADDR(SPA(parp), spa);
    blkcopy(THA(parp), nif[ifn].ni_hwa.ha_addr, EP_ALEN);
    IP_COPYADDR(TPA(parp), tpa);
    return pep;
}
