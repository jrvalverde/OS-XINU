/* initgate.c - initgate */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <bootp.h>

/* static IP addresses, if used */

#if Noth > 0
 /* Need to convert to correct formula */
 IPaddr	iftoip[] = { 0, 0, 0xBF130200, 0xD1062400 };
#else
 IPaddr	iftoip[] = { 0, 0, 0x800A0B00, 0x800A0C00 };
#endif

#ifdef WIRELESS
static	int	itod[] = { NI_LOCAL, ETHER, WIRELESSETH };
#else
itod[] = { NI_LOCAL, ETHER };
#endif
/*
#ifdef	OTHER1
static	int	itod[] = { NI_LOCAL, ETHER, OTHER1, OTHER2 };
#else	OTHER1
static	int	itod[] = { -1, ETHER };
#endif	OTHER1
*/

#define DEBUG

/*------------------------------------------------------------------------
 * initgate  --  initialize a gateway's interface structures
 *------------------------------------------------------------------------
 */
int initgate()
{
    int	i;

    Net.netpool = mkpool(MAXNETBUF, NETBUFS);
    if (Net.netpool == SYSERR) 
	panic("Cannot allocate buffer pool");
    Net.lrgpool = mkpool(MAXLRGBUF, LRGBUFS);
    if (Net.lrgpool == SYSERR) 
	panic("Cannot large allocate buffer pool");
    Net.sema = screate(1);
    Net.nif = NIF;

#ifdef	IPADDR1
    iftoip[1] = dot2ip(IPADDR1);
#endif	/* IPADDR1 */

    for (i=0; i<Net.nif; ++i) {
	/* start off with all 0's (esp. statistics) */
	bzero(&nif[i], sizeof(nif[i]));

	nif[i].ni_state = NIS_DOWN;
	nif[i].ni_admstate = NIS_UP;
	nif[i].ni_ivalid = nif[i].ni_nvalid = FALSE;
	nif[i].ni_svalid = FALSE;
	nif[i].ni_dev = itod[i];
	nif[i].ni_mcast = 0;
	if (i == NI_LOCAL) {
	    /* maxbuf - ether header - CRC - nexthop */
	    nif[i].ni_mtu = MAXLRGBUF-EP_HLEN-EP_CRC-IP_ALEN;
	    nif[i].ni_ip = ip_anyaddr;
	    continue;
	}
	switch(nif[i].ni_dev) {
#if	Noth > 0
	case OTHER1:
	case OTHER2:
	    ofaceinit(i);
	    break;
#endif	/* Noth > 0 */
	case ETHER:
	    efaceinit(i);
	    break;
#ifdef WIRELESS
	case WIRELESSETH:
	    wireless_if_init(i);
	    break;
#endif
	};
    }

    return OK;
}

#if	Noth > 0
ofaceinit(iface)
int	iface;
{
    struct otblk	*otptr;
    struct etdev	*etptr;
    int		i;

/*	nif[iface].ni_ip = iftoip[iface]; */

    otptr = (struct otblk *)devtab[nif[iface].ni_dev].dvioblk;
    otptr->ot_intf = iface;

    nif[iface].ni_descr = otptr->ot_descr;
    nif[iface].ni_mtype = 1;	/* RFC 1156, "other" :-)	*/
    nif[iface].ni_speed = 400000000; /* ~25Mz 32 bit mem copy :-)	*/
    nif[iface].ni_maxreasm = MAXLRGBUF;

    /* fill in physical net addresses */
    blkcopy(nif[iface].ni_hwa.ha_addr, otptr->ot_paddr, EP_ALEN);
    blkcopy(nif[iface].ni_hwb.ha_addr, otptr->ot_baddr, EP_ALEN);
    nif[iface].ni_hwa.ha_len = nif[iface].ni_hwb.ha_len = EP_ALEN;

    /* set the mtu */
    if (nif[iface].ni_dev == OTHER2)
	nif[iface].ni_mtu = SMALLMTU;
    else
	nif[iface].ni_mtu = EP_DLEN;

    etptr = (struct etdev *)devtab[otptr->ot_pdev].dvioblk;
    nif[iface].ni_outq = etptr->ed_outq;

    /* net num comes from ether addr */
    nif[iface].ni_ip = hl2net(iftoip[iface]) | (hl2net(0x000000ff) & dot2ip(Bootrecord.myip));
    nif[iface].ni_net = netnum(nif[iface].ni_ip);
    nif[iface].ni_ivalid = TRUE;

    /* set the mask (same for both) */

    setmask(iface, Bootrecord.subnetmask);

    /* host */
    rtadd(nif[iface].ni_ip, ip_maskall, nif[iface].ni_ip, 0,
	  NI_LOCAL, RT_INF);
    /* broadcast (all 0's, no subnet) */
    rtadd(nif[iface].ni_net, ip_maskall, nif[iface].ni_ip, 0,
	  NI_LOCAL, RT_INF);
}
#endif	/* Noth > 0 */


/*-------------------------------------------------------------------------
 * efaceinit - 
 *-------------------------------------------------------------------------
 */
efaceinit(iface)
int	iface;
{
    extern	int  ethmcast();
    IPaddr	mask;
    struct etdev *ped = (struct etdev *) devtab[nif[iface].ni_dev].dvioblk;
    
    blkcopy(nif[iface].ni_hwa.ha_addr, ped->ed_paddr, EP_ALEN);
    blkcopy(nif[iface].ni_hwb.ha_addr, ped->ed_bcast, EP_ALEN);
    nif[iface].ni_descr = ped->ed_descr;
    nif[iface].ni_mtype = 6;		/* RFC 1156, Ethernet CSMA/CD	*/
    nif[iface].ni_speed = 10000000;	/* bits per second		*/
    nif[iface].ni_maxreasm = MAXLRGBUF;
    nif[iface].ni_hwa.ha_len = EP_ALEN;
    nif[iface].ni_hwb.ha_len = EP_ALEN;
    nif[iface].ni_mtu = EP_DLEN;
    nif[iface].ni_outq = ped->ed_outq;
    nif[iface].ni_mcast = ethmcast;
    
    ped->ed_ifnum = iface;
}

#ifdef WIRELESS
/*-------------------------------------------------------------------------
 * wireless_if_init -
 *-------------------------------------------------------------------------
 */
wireless_if_init(iface)
int	iface;
{
    extern	int  ethmcast();
    IPaddr	mask;
    struct aldev *ped = (struct etdev *) devtab[nif[iface].ni_dev].dvioblk;
    
    blkcopy(nif[iface].ni_hwa.ha_addr, ped->ed_paddr, EP_ALEN);
    blkcopy(nif[iface].ni_hwb.ha_addr, ped->ed_bcast, EP_ALEN);
    nif[iface].ni_descr = ped->ed_descr;
    nif[iface].ni_mtype = 6;		/* RFC 1156, Ethernet CSMA/CD	*/
    /*
     * mark it negative; will restore it in netstart().
     */
    nif[iface].ni_speed = -2000000;	/* bits per second		*/
    
    nif[iface].ni_maxreasm = MAXLRGBUF;
    nif[iface].ni_hwa.ha_len = EP_ALEN;
    nif[iface].ni_hwb.ha_len = EP_ALEN;
    nif[iface].ni_mtu = EP_DLEN;
    nif[iface].ni_outq = ped->ed_outq;
    nif[iface].ni_mcast = ethmcast;
    
    ped->ed_ifnum = iface;
}
#endif





