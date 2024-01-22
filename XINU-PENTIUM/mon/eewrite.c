#include <./mon/monnetwork.h>
#include <./mon/monitor.h>
#include <./mon/monee.h>

/*#define DEBUG*/

/*------------------------------------------------------------------------
 * mon_ee_write - write a single packet to an Intel EtherExpress
 *------------------------------------------------------------------------
 */
int mon_ee_write(pep, len)
struct ep       *pep;
int             len;
{
    struct aldev        *ped;

#ifdef DEBUG
    kprintf("mon_ee_write\n");
#endif

    ped = &mon_ee[0];
    
    if (len > EP_MAXLEN) {
        kprintf("mon_ee_write: len(%d) > EP_MAXLEN(%d)\n", len, EP_MAXLEN);
        freebuf(pep);
        return SYSERR;
    }

    /* subtract the local header */
    len -= (int)&pep->ep_eh - (int)pep;
    if (len < EP_MINLEN)
        len = EP_MINLEN;

    blkcopy(pep->ep_src, ped->ed_paddr, EP_ALEN);
    pep->ep_len = len;
    pep->ep_type = hs2net(pep->ep_type);

    if (mon_enq(ped->ed_outq, pep, 0) < 0) {
        kprintf("mon_ee_write: qull full (len=%d)\n", mon_lenq(ped->ed_outq));
        freebuf(pep);
	
        if (ped->ed_xpending)
            ped->ed_xpending = 0;
	
	/* restart 586 */
        mon_ee_start(ped, EE_IOBASE);
    }
    
    mon_ee_wstrt(ped);
    return OK;
}
