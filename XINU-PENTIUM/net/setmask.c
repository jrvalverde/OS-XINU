/* setmask.c - setmask */

#include <conf.h>
#include <kernel.h>
#include <network.h>

extern	int	bsdbrc;		/* use Berkeley (all-0's) broadcast	*/
/*------------------------------------------------------------------------
 *  setmask - set the net mask for an interface
 *------------------------------------------------------------------------
 */
int setmask(inum, mask)
int	inum;
IPaddr	mask;
{
    IPaddr	aobrc;		/* all 1's broadcast */
    IPaddr	defmask;
    int	i;

    if (nif[inum].ni_svalid) {
	/* one set already-- fix things */

	rtdel(nif[inum].ni_subnet, nif[inum].ni_mask);
	rtdel(nif[inum].ni_brc, ip_maskall);
	rtdel(nif[inum].ni_subnet, ip_maskall);
    }
    nif[inum].ni_mask = mask;
    nif[inum].ni_svalid = TRUE;
    defmask = netmask(nif[inum].ni_ip);

    nif[inum].ni_subnet = nif[inum].ni_ip & nif[inum].ni_mask; 
    if (bsdbrc) {
	nif[inum].ni_brc = nif[inum].ni_subnet;
	aobrc = nif[inum].ni_subnet | ~nif[inum].ni_mask;
    } else
	nif[inum].ni_brc = nif[inum].ni_subnet | ~nif[inum].ni_mask;
    
    /* set network (not subnet) broadcast */
    nif[inum].ni_nbrc = nif[inum].ni_ip | ~defmask;

    /* install routes */
    /* net */
    rtadd(nif[inum].ni_subnet, nif[inum].ni_mask, nif[inum].ni_ip,
	  0, inum, RT_INF);
    if (bsdbrc)
	rtadd(aobrc, ip_maskall, nif[inum].ni_ip, 0, NI_LOCAL, RT_INF);
    else	/* broadcast (all 1's) */
	rtadd(nif[inum].ni_brc, ip_maskall, nif[inum].ni_ip, 0, NI_LOCAL, RT_INF);
    /* broadcast (all 0's) */
    rtadd(nif[inum].ni_subnet, ip_maskall, nif[inum].ni_ip, 0, NI_LOCAL, RT_INF);
    return OK;
}

IPaddr	ip_maskall = 0xffffffff;
