/* dgread.c - dgread */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  dgread  -  read one datagram from a datagram pseudo-device
 *------------------------------------------------------------------------
 */
int dgread(pdev, pxg, len)
struct	devsw	*pdev;
struct	xgram	*pxg;
int		len;
{
    struct	dgblk	*pdg;
    struct	upq	*pup;
    struct	ep	*pep;
    struct	ip	*pip;
    struct	udp	*pudp;
    int		datalen;

    pdg = (struct dgblk *)pdev->dvioblk;
    if (pdg->dg_mode & DG_TMODE) {
	pup = &upqs[pdg->dg_upq];
	if (!isbadpid(pup->up_pid))
	    return SYSERR;
	if (pcount(pdg->dg_xport) <= 0) {
	    pup->up_pid = getpid();
	    if (recvtim( DG_TIME) == TIMEOUT) {
		pup->up_pid = BADPID;
		return TIMEOUT;
	    }
	}
    }
    pep = (struct ep *) preceive(pdg->dg_xport);

    /* copy data into user's buffer & set length */

    pip = (struct ip *)pep->ep_data;
    pudp = (struct udp *)pip->ip_data;
    datalen = pudp->u_len - U_HLEN;
    if (pdg->dg_mode & DG_NMODE) {
	if (datalen+XGHLEN > len) {
	    freebuf(pep);
	    return SYSERR;
	}
	pxg->xg_fip = pip->ip_src;
	pxg->xg_fport = pudp->u_src;
	pxg->xg_lport = pdg->dg_lport;
	blkcopy(pxg->xg_data, pudp->u_data, datalen);
    } else {
	if (datalen > len)
	    datalen = len;
	blkcopy(pxg, pudp->u_data, datalen);
    }
    freebuf(pep);
    return datalen;
}
