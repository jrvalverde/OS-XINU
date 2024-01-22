/* ee_write.c - ee_write */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <ee.h>

/*#define DEBUG*/

/*------------------------------------------------------------------------
 * ee_write - write a single packet to an Intel EtherExpress
 *------------------------------------------------------------------------
 */
int ee_write(pdev, pep, len)
struct devsw    *pdev;
struct ep       *pep;
int             len;
{
    struct aldev        *ped;

#ifdef DEBUG
    kprintf("ee_write\n");
#endif

    ped = (struct aldev *)pdev->dvioblk;
    if (len > EP_MAXLEN) {
        nif[ped->ed_ifnum].ni_odiscard++;
        kprintf("ee_write: len(%d) > EP_MAXLEN(%d)\n", len, EP_MAXLEN);
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
    pep->ep_order &= ~EPO_NET;

    if (enq(ped->ed_outq, pep, 0) < 0) {
        u_short iobase = ped->ed_pdev->dvcsr;

        nif[ped->ed_ifnum].ni_odiscard++;
        kprintf("ee_write: cannot enqueue (len=%d)\n", lenq(ped->ed_outq));
        freebuf(pep);

        /* XXX */
        while (lenq(ped->ed_outq) > 1) {
            pep = (struct ep *)deq(ped->ed_outq);
            freebuf(pep);
        }

        kprintf("ee: scb status = %x\n", EE_READ_SCB_STATUS(iobase));
        /*
         * reset, then read on-board parameters
         */
        kprintf("ee: Reseting....\n");
        ee_start(ped, iobase);

        if (ped->ed_xpending)
            ped->ed_xpending = 0;

        kprintf("ee: scb status = %x\n", EE_READ_SCB_STATUS(iobase));
    }
    ee_wstrt(ped);
    return OK;
}
