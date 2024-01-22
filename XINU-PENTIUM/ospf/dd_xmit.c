/* dd_xmit.c - dd_xmit */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <ospf.h>

/*------------------------------------------------------------------------
 *  dd_xmit - transmit pending Database Description packets
 *------------------------------------------------------------------------
 */
int dd_xmit(pif, pnb)
struct ospf_if	*pif;
struct ospf_nb	*pnb;
{
	struct	ep	*pephead, *pep, *ospfddtmpl();
	struct	ip	*pip;
	struct	ospf	*po;
	struct	ospf_dd	*pdd;
	int		len;

	if (pephead = (struct ep *)headq(pnb->nb_dsl)) {
kprintf("dd_xmit: queue not empty\n");
		pep = (struct ep *)getbuf(Net.netpool);
		if ((int)pep == SYSERR)
			return SYSERR;
		/* make a copy */
		pip = (struct ip *)pephead->ep_data;
		po = (struct ospf *)pip->ip_data;
		len = EP_HLEN + IPMHLEN + po->ospf_len;
		blkcopy(pep, pephead, len);
		pip = (struct ip *)pep->ep_data;
		po = (struct ospf *)pip->ip_data;
		pdd = (struct ospf_dd *)po->ospf_data;
	} else {
kprintf("dd_xmit: queue empty\n");
		/* no DD's to send; create an ACK-only */
		pep = ospfddtmpl(pif);
		pip = (struct ip *)pep->ep_data;
		po = (struct ospf *)pip->ip_data;
		pdd = (struct ospf_dd *)po->ospf_data;
		if (pnb->nb_master)
			pdd->dd_control = DDC_MSTR;
		else
			pdd->dd_control = 0;
	}
	pdd->dd_seq = hl2net(pnb->nb_seq);
	po->ospf_authtype = net2hs(pif->if_area->ar_authtype);
	len = po->ospf_len;
	po->ospf_len = net2hs(po->ospf_len);
	pep->ep_order &= ~EPO_OSPF;
	bzero(po->ospf_auth, AUTHLEN);
	po->ospf_cksum = 0;
	po->ospf_cksum = cksum(po, len);
	blkcopy(po->ospf_auth, pif->if_area->ar_auth, AUTHLEN);
	pip->ip_src = ip_anyaddr;
	ipsend(pnb->nb_ipa, pep, len, IPT_OSPF, IPP_INCTL, IP_TTL);
}
