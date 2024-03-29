/* lsr_queue.c - lsr_queue */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <ospf.h>

/*------------------------------------------------------------------------
 *  lsr_queue - generate Link State Request packets
 *------------------------------------------------------------------------
 */
int lsr_queue(pif, pnb, pepin)
struct ospf_if	*pif;
struct ospf_nb	*pnb;
struct ep	*pepin;
{
	struct	ep	*pep = 0, *lsr_add();
	struct	ip	*pipin = (struct ip *)pepin->ep_data;
	struct	ospf	*poin = (struct ospf *)pipin->ip_data;
	struct	ospf_dd	*pdd = (struct ospf_dd *)poin->ospf_data;
	struct	ospf_lss *plss;
	struct	ospf_db	*pdb, *db_lookup();
	int		i, nlss;

kprintf("lsrq ospf_len %d MINDDLEN %d LSSHDRLEN %d\n", poin->ospf_len,
MINDDLEN, LSSHDRLEN);
	nlss = (poin->ospf_len - MINDDLEN) / LSSHDRLEN;
	plss = pdd->dd_lss;
kprintf("lsr_queue nlss %d\n", nlss);
	for (i=0; i<nlss; ++i, ++plss) {
		pdb = db_lookup(pif->if_area, plss->lss_type,
			plss->lss_lsid);
kprintf("lsr_q i %d pdb %X\n", i, pdb);
		if (pdb == 0)
			pep = lsr_add(pif, plss, pep);

/* if plss newer, pep = lsr_add(pif, plss, pep); */
	}
	if (pep && enq(pnb->nb_lsrl, pep, 0) < 0)
		freebuf(pep);
	lsr_xmit(pif, pnb);
}
