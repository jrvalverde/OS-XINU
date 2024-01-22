/* ospf_hin.c - ospf_hin */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <ospf.h>

/*------------------------------------------------------------------------
 * ospf_hin - handled input of OSPF HELLO packets
 *------------------------------------------------------------------------
 */
int
ospf_hin(pep)
struct ep	*pep;
{
	struct ospf_if		*pif = &ospf_if[pep->ep_ifn];
	struct ospf_nb		*pnb, *nb_add();
	struct ip		*pip;
	struct ospf		*po;
	struct ospf_hello	*poh;

	pip = (struct ip *)pep->ep_data;
	po = (struct ospf *)((char *)pip + IP_HLEN(pip));
	poh = (struct ospf_hello *)po->ospf_data;

	if (net2hs(poh->oh_hintv) != pif->if_hintv ||
	    net2hl(poh->oh_rdintv) != pif->if_rdintv)
		return;
	if (po->ospf_rid == pif->if_rid)
		return;		/* one of our own packets */

	pnb = nb_add(pif, po);
	if (pnb == 0)
		return;		/* neighbor list overflowed */
	pnb->nb_ipa = pip->ip_src;
	if (nb_switch(pif, pnb, pep) == 0)
		return;
	if (poh->oh_prio != pnb->nb_prio) {
		pif->if_event |= IFE_NCHNG;
		pnb->nb_prio = poh->oh_prio;
	}
	if (poh->oh_drid == pnb->nb_rid) {	/* Neighbor claims DR	*/
		if (poh->oh_brid == 0 && pif->if_state == IFS_WAITING)
			pif->if_event |= IFE_BSEEN;
		else if (pnb->nb_drid != pnb->nb_rid)
			pif->if_event |= IFE_NCHNG;
	} else if (pnb->nb_drid == pnb->nb_rid)
		pif->if_event |= IFE_NCHNG;
	pnb->nb_drid = poh->oh_drid;
	if (poh->oh_brid == pnb->nb_rid) {
		if (pif->if_state == IFS_WAITING)
			pif->if_event |= IFE_BSEEN;
		else if (pnb->nb_brid != pnb->nb_rid)
			pif->if_event |= IFE_NCHNG;
	} else if (pnb->nb_brid == pnb->nb_rid)
		pif->if_event |= IFE_NCHNG;
	pnb->nb_brid = poh->oh_brid;
}
