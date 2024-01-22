/* ue_demux.c - ue_demux */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <ue.h>

/*------------------------------------------------------------------------
 * ue_demux - demultiplex an input packet from an SMC Ultra
 *------------------------------------------------------------------------
 */
int
ue_demux(pud)
struct utdev	*pud;
{
	struct ep	*pep;
	struct urb	*prb;
	int		ifnum;

	prb = (struct urb *) (pud->ud_iomem + (pud->ud_nextbuf << 8));
	ifnum = pud->ud_ifnum;
	if (ifnum < 0 || ifnum >= NIF)
		goto drop;
	if (nif[ifnum].ni_state != NIS_UP) {
		nif[ifnum].ni_idiscard++;
		goto drop;
	}
	while (prb->urb_rstat) {
		pep = (struct ep *)nbgetbuf(Net.netpool);
		if (pep == 0) {
			if (ifnum >= 0 && ifnum < NIF)
				nif[ifnum].ni_idiscard++;
			goto drop;
		}
/* kprintf("prb %X rstat %x len %d next %X pep %X\n", pud->ud_nextbuf, prb->urb_rstat,prb->urb_len,prb->urb_next, pep); */
		pud->ud_nextbuf = prb->urb_next;
		pep->ep_len = prb->urb_len - EP_CRC;	/* drop CRC */
		if (pep->ep_len > 1514) {
			ue_dbuf(pud);
			kprintf("ue_demux: len %d too large prb %X\n",
				pep->ep_len, prb);
				freebuf(pep);
		} else {
			ueget(&pep->ep_eh, prb->urb_data, pep->ep_len,
				pud->ud_rmin, pud->ud_rmax);
			pep->ep_type = net2hs(pep->ep_type);
			pep->ep_order = EPO_NET;
#ifdef	NETMON
			nm_in(&nif[ifnum], pep, pep->ep_len);
			if (!ethmatch(&nif[ifnum], pep->ep_dst, pep->ep_len))
				freebuf(pep);
			else
#endif	/* NETMON */
			ni_in(&nif[ifnum], pep, pep->ep_len);
		}
		uewr(pud->ud_pdev->dvcsr, BOUND, pud->ud_nextbuf);
		/* check for more */
		prb = (struct urb *) (pud->ud_iomem + (pud->ud_nextbuf << 8));
	}
	return OK;
drop:
	if (ifnum >= 0 && ifnum < NIF)
		nif[ifnum].ni_ierrors++;
	uewr(pud->ud_pdev->dvcsr, BOUND, prb->urb_next);
	pud->ud_nextbuf = prb->urb_next;
}

ueget(pdst, psrc, count, pmin, pmax)
unsigned char	*pdst, *psrc, *pmax, *pmin;
int		count;
{
	int		i, part;

	part = pmax - psrc;
	if (part < count) {
		blkcopy(pdst, psrc, part);
		/* zero each buffer's  RSTAT & len */
		(int)psrc &= ~(UE_BUFSIZE-1);
		for (; psrc < pmax; psrc += UE_BUFSIZE)
			*psrc = 0;
		psrc = pmin;
		pdst += part;
		count -= part;
	}
	blkcopy(pdst, psrc, count);
	/* zero each buffer's  RSTAT & len */
	pmax = psrc + count;
	(int)psrc &= ~(UE_BUFSIZE-1);
	for (; psrc < pmax; psrc += UE_BUFSIZE)
		*psrc = 0;
}

uering(pud, bn)
struct utdev	*pud;
int		bn;
{
	int	i;

kprintf("bn = %d\n", bn);
	for (i=0; i<UE_NRX*6; ++i) {
		struct urb *prb = (struct urb *)pud->ud_iomem + (i << 8);
kprintf("ue_demux (%d) rstat %x next %x len %d\n", i, prb->urb_rstat,
prb->urb_next, prb->urb_len);
	}
}
