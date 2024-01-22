/* setmask.c - setmask */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*#define DEBUG*/

IPaddr	ip_maskall = { 255, 255, 255, 255 };

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
    
#ifdef DEBUG
    kprintf("setmask: setting mask for if%d to %08x\n", inum,ip2i(mask));
#endif
    if (nif[inum].ni_svalid) {
	/* one set already-- fix things */
	
	rtdel(nif[inum].ni_subnet, nif[inum].ni_mask);
	rtdel(nif[inum].ni_brc, ip_maskall);
	rtdel(nif[inum].ni_subnet, ip_maskall);
    }
    IP_COPYADDR(nif[inum].ni_mask, mask);
    nif[inum].ni_svalid = TRUE;
    netmask(defmask, nif[inum].ni_ip);
    
    for (i=0; i<IP_ALEN; ++i) {
	nif[inum].ni_subnet[i] =
	    nif[inum].ni_ip[i] & nif[inum].ni_mask[i]; 
	if (bsdbrc) {
	    nif[inum].ni_brc[i] = nif[inum].ni_subnet[i];
	    aobrc[i] = nif[inum].ni_subnet[i] |
		~nif[inum].ni_mask[i];
	} else
	    nif[inum].ni_brc[i] = nif[inum].ni_subnet[i] |
		~nif[inum].ni_mask[i];
	/* set network (not subnet) broadcast */
	nif[inum].ni_nbrc[i] =
	    nif[inum].ni_ip[i] | ~defmask[i];
    }
    
    /* install routes */
    /* net */
    rtadd(nif[inum].ni_subnet, nif[inum].ni_mask, nif[inum].ni_ip,
	  0, inum, RT_INF);
    if (bsdbrc)
	rtadd(aobrc, ip_maskall, nif[inum].ni_ip, 0,
	      NI_LOCAL, RT_INF);
    else	/* broadcast (all 1's) */
	rtadd(nif[inum].ni_brc, ip_maskall, nif[inum].ni_ip, 0,
	      NI_LOCAL, RT_INF);
    /* broadcast (all 0's) */
    rtadd(nif[inum].ni_subnet, ip_maskall, nif[inum].ni_ip, 0,
	  NI_LOCAL, RT_INF);
#ifdef DEBUG
    kprintf("setmask: new network interface structure:\n");
    nifdump(inum);
    kprintf("setmask: new route tables:\n");
    rtdump();
#endif
    return OK;
}

