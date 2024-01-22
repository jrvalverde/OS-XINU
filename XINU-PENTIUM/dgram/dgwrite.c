/* dgwrite.c - dgwrite */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  dgwrite  -  write one datagram to a datagram pseudo-device
 *------------------------------------------------------------------------
 */
int dgwrite(pdev, pxg, len)
struct	devsw	*pdev;
struct	xgram	*pxg;
int		len;
{
    struct	ep	*pep;
    struct	ip	*pip;
    struct	udp	*pudp;
    struct	dgblk	*pdg;
    int		ipa;
    int		dst;		/* destination UDP port		*/

    if (len < 0 || len > U_MAXLEN) {
	kprintf("dgwrite: ** bad len (%d)\n", len);
	return SYSERR;
    }
    
    pdg = (struct dgblk *) pdev->dvioblk;
    pep = (struct ep *) getbuf(Net.netpool);
    if ((int)pep == SYSERR) {
	kprintf("dgwrite: ** not buffer\n");
	return SYSERR;
    }
    pep->ep_order = ~0;
    pip = (struct ip *) pep->ep_data;
    pudp = (struct udp *) pip->ip_data;
    dst = pdg->dg_fport;
    ipa = pdg->dg_fip;
    if ((pdg->dg_mode & DG_NMODE) != 0) {
	if (dst == 0) {
	    dst = pxg->xg_fport;
	    ipa = pxg->xg_fip;
	}
	blkcopy(pudp->u_data, pxg->xg_data, len);
    } else {
	if ( dst == 0) {
	    kprintf("dgwrite: ** dst = 0\n");
	    freebuf(pep);
	    return SYSERR;
	}
	blkcopy(pudp->u_data, pxg, len);
    }
    return udpsend(ipa, dst, pdg->dg_lport, pep, len, pdg->dg_mode & DG_CMODE);
}
