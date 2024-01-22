/* ethwrite.c - ethwrite */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>

/*#define DEBUG*/

/*------------------------------------------------------------------------
 *  ethwrite - write a single packet to the ethernet
 *------------------------------------------------------------------------
 */

ethwrite(devptr, pep, len)
     struct	devsw	*devptr;
     struct     ep      *pep;
     int	len;
{
    STATWORD ps;    
    struct	etblk	*etptr;
    
#ifdef DEBUG
    {
	char junk[80];
	
    kprintf("ethwrite(devptr,0x%x,%d) called\n", pep, len);
    kprintf("next_hop = %s\n", ip2dot(junk, pep->ep_nexthop));
    kprintf("ep_dst: %02x:%02x:%02x:%02x:%02x:%02x\n", pep->ep_dst[0]&0xff,
	    pep->ep_dst[1]&0xff, pep->ep_dst[2]&0xff, pep->ep_dst[3]&0xff,
	    pep->ep_dst[4]&0xff, pep->ep_dst[5]&0xff);
    kprintf("ep_type: %x\n", pep->ep_type);
    }
#endif	
    
    if ((len <= 0) || (len > EP_MAXLEN)) {
	nif[etptr->etintf].ni_odiscard++;
	freebuf(pep);
	return SYSERR;
    }
    
    etptr = (struct etblk *)devptr->dvioblk;
    /* subtract the local header */
    len -= (int)&pep->ep_eh - (int)pep;
    if (len < EP_MINLEN)
	len = EP_MINLEN;
    
    blkcopy(pep->ep_src, etptr->etpaddr, EP_ALEN);
    
    pep->ep_len = len;
    pep->ep_type = hs2net(pep->ep_type);
    
#ifdef DEBUG
    kprintf("ep_src: %02x:%02x:%02x:%02x:%02x:%02x\n", pep->ep_src[0]&0xff,
	    pep->ep_src[1]&0xff, pep->ep_src[2]&0xff, pep->ep_src[3]&0xff,
	    pep->ep_src[4]&0xff, pep->ep_src[5]&0xff);
    kprintf("ep_len: %d\n", pep->ep_len);
#endif

    if (enq(etptr->etoutq, pep, 0) < 0) {
#if defined(PRINTERRORS) || defined(DEBUG)
	kprintf("ethwrite() ==> SYSERR, couldn't enq()\n");
#endif	
	nif[etptr->etintf].ni_odiscard++;
	freebuf(pep);
	return SYSERR;
    }
    
    disable(ps);
    etptr->etoutqlen++;
    if (etptr->etoutqlen == 1)
	ethwstrt(etptr);
    
    restore(ps);
    return OK;
}
