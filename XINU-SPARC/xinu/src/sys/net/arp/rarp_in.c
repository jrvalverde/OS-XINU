/* rarp_in.c - rarp_in */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  rarp_in  -  handle RARP packet coming in from Ethernet network
 *  	N.B. - Called by ni_in-- SHOULD NOT BLOCK
 *------------------------------------------------------------------------
 */
int rarp_in(pni, packet)
     struct	netif	*pni;
     struct	ep	*packet;
{
    STATWORD	ps;
    int		pid, ret;
    int		device = pni->ni_dev;
    struct	arp	*parp;
    struct	etblk	*etptr;
    
#ifdef DEBUG
    kprintf("rarp_in(pni, 0x%08x) called\n", pni, packet);
#endif
    parp = (struct arp *) packet->ep_data;
    parp->ar_op = net2hs(parp->ar_op);
    parp->ar_hwtype = net2hs(parp->ar_hwtype);
    parp->ar_prtype = net2hs(parp->ar_prtype);
    
    if (parp->ar_op == RA_REPLY) {
	etptr = (struct etblk *)devtab[device].dvioblk;
	if (blkequ(THA(parp), etptr->etpaddr, EP_ALEN)) {
	    IPaddr mask;
	    
	    IP_COPYADDR(pni->ni_ip, TPA(parp));
	    netnum(pni->ni_net, pni->ni_ip);
	    pni->ni_ivalid = TRUE;
	    netmask(mask, pni->ni_ip);
	    setmask(etptr->etintf, mask);
	    disable(ps);
	    pid = rarppid;
	    if (!isbadpid(pid)) {
		rarppid = BADPID;
		send(pid, OK);
	    }
	    restore(ps);
	}
	ret = OK;
    } else
	ret = SYSERR;
    freebuf(packet);
    return ret;
}
