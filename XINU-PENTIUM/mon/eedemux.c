#include <./mon/monnetwork.h>
#include <./mon/monitor.h>
#include <./mon/monee.h>

/*#define DEBUG*/
/*#define PRINTERRORS*/
/*------------------------------------------------------------------------
 * mon_ee_demux - receive frames from an Intel EtherExpress
 *------------------------------------------------------------------------
 */
int mon_ee_demux(ped)
struct aldev    *ped;
{
    struct rbd  rbd;
    struct rfd  rfd;
    struct ep   *pep, *mon_ee_get();
    unsigned short iobase, rfd_status, rfd_next, rbd_offset;

#ifdef DEBUG
    kprintf("[mon_ee_demux]");
#endif

    iobase = EE_IOBASE;
    
    for (;;) {
	outw(iobase+RDPTR, ped->ed_rfd);
	rfd_status = inw(iobase);   /* status */

        if (!(rfd_status & RFD_C))
            return;

        if (rfd_status & RFD_OK) {
	    outw(iobase+RDPTR, ped->ed_rfd+6);
	    rbd_offset = inw(iobase);
            mon_ee_ram_blk_read(iobase, rbd_offset, &rbd, sizeof(rbd));
            if ((rbd.rbd_count & RBD_OK) != RBD_OK) {
#ifdef PRINTERRORS
                kprintf("**mon_ee_demux: error in rbd_count=%x\n", rbd.rbd_count);
#endif
            }
            else {
                /*
                 * receive a good frame
                 */
		pep = mon_ee_get(ped, &rbd, iobase);
            }
        }
        else {
#ifdef PRINTERRORS
                kprintf("**mon_ee_demux: ERR, rfd_status=%x\n", rfd_status);
#endif
        } /* end if */

        /*
         * make the first RFD become the last one and the next one
         * become the first one.
         */
	outw(iobase+WRPTR, ped->ed_rfd);
        outw(iobase, 0);                /* status       */
        outw(iobase, RFD_EL|RFD_S);     /* command	*/
	
	outw(iobase+RDPTR, ped->ed_rfd+4);      /* next rfd */
        rfd_next = inw(iobase);

	outw(iobase+WRPTR, ped->ed_rfdend);
        outw(iobase, 0);                /* status      */
        outw(iobase, 0);                /* command      */
	ped->ed_rfdend = ped->ed_rfd;
        ped->ed_rfd = rfd_next;

        if (pep) {
	    if (mon_nif[0].ni_state != NIS_UP)
		freebuf(pep);
	    else
	        mon_ni_in(&mon_nif[0], pep, pep->ep_len);
	}
    } /* end for */
}

/*-------------------------------------------------------------------------
 * mon_ee_get - get frame
 *-------------------------------------------------------------------------
 */
struct ep *mon_ee_get(ped, prbd, iobase)
struct aldev    *ped;
struct rbd      *prbd;
unsigned short  iobase;
{
    struct ep   *pep;

    pep = (struct ep *)getbuf(mon_bufpool);
    if (pep == 0) {
#ifdef PRINTERRORS
        kprintf("mon_ee_get: ?? no buffer\n");
#endif
        return 0;
    }

    pep->ep_len = (prbd->rbd_count & RBD_COUNT_MASK);
#ifdef DEBUG
    kprintf("mon_ee_get: len=%d\n", pep->ep_len);
#endif

    if (pep->ep_len - sizeof(struct eh) > EP_DLEN) {
#ifdef PRINTERRORS
        kprintf("ee: packet too large (%d)\n", pep->ep_len);
#endif
        freebuf(pep);
        return 0;
    }

    /*
     * copy frame from on-board RAM to host memory
     */
    mon_ee_ram_blk_read(iobase, prbd->rbd_buf, &pep->ep_eh, pep->ep_len);
    pep->ep_type = net2hs(pep->ep_type);

#ifdef DEBUG
    {
        int i;

        kprintf("ETHER: dst %02x", pep->ep_dst[0]);
        for (i=1; i<EP_ALEN; ++i)
                kprintf(":%02x", pep->ep_dst[i]);
        kprintf(" src %02x", pep->ep_src[0]);
        for (i=1; i<EP_ALEN; ++i)
                kprintf(":%02x", pep->ep_src[i]);
        kprintf("\nETHER: type %x\n", pep->ep_type);
    }
#endif

    return pep;
}
