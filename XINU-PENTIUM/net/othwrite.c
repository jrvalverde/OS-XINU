/* othwrite.c - othwrite */


#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>

#if Noth > 0

/*------------------------------------------------------------------------
 *  othwrite - write a single packet to the ethernet
 *------------------------------------------------------------------------
 */
int othwrite(devptr, pep, len)
struct	devsw	*devptr;
struct	ep	*pep;
int	len;
{
	STATWORD ps;
        struct	otblk	*otptr;
        struct	etdev	*etptr;

	if (len > EP_MAXLEN) {
		freebuf(pep);
		nif[otptr->ot_intf].ni_odiscard++;
		return SYSERR;
	}
	/* subtract the local header */
	len -= (int)&pep->ep_eh - (int)pep;

	if (len < EP_MINLEN)
		len = EP_MINLEN;

	otptr = (struct otblk *)devptr->dvioblk;
	blkcopy(pep->ep_src, otptr->ot_paddr, EP_ALEN);
	etptr = (struct etdev *)devtab[otptr->ot_pdev].dvioblk;

    pep->ep_len = len;
	pep->ep_type = hs2net(pep->ep_type);
	pep->ep_order &= ~EPO_NET;

#ifdef	DEBUG
	if (pep->ep_type == EPT_IP)
	{
		struct ip *pip = (struct ip *)pep->ep_data;
		if (pip->ip_proto == IPT_TCP) {
			kprintf("othwrite(%X, %d)\n", pep, len);
			pdump(pep);
		}
	}
#endif	/* DEBUG */
	pep->ep_len = len;
	if (enq(etptr->ed_outq, pep, 0) < 0) {
		nif[otptr->ot_intf].ni_odiscard++;
		freebuf(pep);
		return SYSERR;
	}
	nif[otptr->ot_intf].ni_oucast++;
	ee_wstrt(etptr);
	return OK;
}
#endif /* Noth > 0 */
