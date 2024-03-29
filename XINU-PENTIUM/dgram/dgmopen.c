/* dgmopen.c - dgmopen */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  dgmopen  -  open a fresh datagram pseudo device and return descriptor
 *------------------------------------------------------------------------
 */
int dgmopen(pdev, fspec, lport)
struct	devsw	*pdev;
char		*fspec;
int		lport;
{
	struct	dgblk	*pdg;
	struct	upq	*pup;
	int		slot, upi, i;

	if ((slot=dgalloc()) == SYSERR)
		return SYSERR;
	pdg = &dgtab[slot];
	wait(udpmutex);
	if (lport == ANYLPORT)
		lport = udpnxtp();
	else {
		for (i=0 ; i<UPPS ; ++i) {
			pup = &upqs[i];
			if (pup->up_valid && pup->up_port == lport) {
				pdg->dg_state = DGS_FREE;
				signal(udpmutex);
				return SYSERR;
			}
		}
	}
	signal(udpmutex);
	if (dnparse(fspec, &pdg->dg_fip, &pdg->dg_fport) == SYSERR ||
	    (upi=upalloc()) == SYSERR) {
		pdg->dg_state = DGS_FREE;
		return SYSERR;
	}
	pup = &upqs[upi];
	pup->up_port = pdg->dg_lport = lport;
	pdg->dg_xport = pup->up_xport;
	pdg->dg_upq = upi;
	pdg->dg_mode = DG_NMODE|DG_CMODE;
	return pdg->dg_dnum;
}
