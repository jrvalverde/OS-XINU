/* ee_demux.c */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>
#include <ee.h>

/*#define DEBUG*/

#define PRINTERRORS
/*------------------------------------------------------------------------
 * ee_demux - receive frames from an Intel EtherExpress
 *------------------------------------------------------------------------
 */
int ee_demux(ped)
struct aldev    *ped;
{
    struct rbd  rbd;
    struct rfd  rfd;
    struct ep   *pep, *ee_get();
    unsigned short iobase, rfd_status, rfd_next, rbd_offset;
    int         ifnum;

#ifdef DEBUG
    kprintf("[ee_demux]");
#endif

    iobase = ped->ed_pdev->dvcsr;
    ifnum = ped->ed_ifnum;
    if (ifnum < 0 || ifnum >= NIF)
        return;

    for (;;) {
	outw(iobase+RDPTR, ped->ed_rfd);
	rfd_status = inw(iobase);   /* status */

        if (!(rfd_status & RFD_C))
            return;

        if (rfd_status & RFD_OK) {
	    outw(iobase+RDPTR, ped->ed_rfd+6);
	    rbd_offset = inw(iobase);
            ee_ram_blk_read(iobase, rbd_offset, &rbd, sizeof(rbd));
            if ((rbd.rbd_count & RBD_OK) != RBD_OK) {
#ifdef PRINTERRORS
                kprintf("**ee_demux: error in rbd_count=%x\n", rbd.rbd_count);
#endif
            }
            else {
                /*
                 * receive a good frame
                 */
                if (nif[ifnum].ni_state == NIS_UP)
                    pep = ee_get(ped, &rbd, iobase);
                else
                    pep = 0;
            }
        }
        else {
                nif[ifnum].ni_idiscard++;
#ifdef PRINTERRORS
                kprintf("**ee_demux: ERR, rfd_status=%x\n", rfd_status);
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
            /* check for a process waiting to read */
            if (ped->ed_rpid != BADPID) {
#ifdef DEBUG
                kprintf("ee_dmux: passed packet to proc %d\n", ped->ed_rpid);
#endif
                /* pass the pep to the reading process */
                if (enq(ped->ed_inq, pep, 0) < 0) {
#if defined(PRINTERRORS) || defined(DEBUG)
                    kprintf("ee_demux() couldn't enq()\n");
#endif
                    freebuf(pep);
                    return(SYSERR);
                }

                /* wake up the process */
                ready(ped->ed_rpid, RESCHNO);
                ped->ed_rpid = BADPID;
            }
            else {
                /*
                 * pass it to IP; may cause context switch
                 */
                ni_in(&nif[ifnum], pep, pep->ep_len);
            }
        }
    } /* end for */
}

/*-------------------------------------------------------------------------
 * ee_get - get frame
 *-------------------------------------------------------------------------
 */
struct ep *ee_get(ped, prbd, iobase)
struct aldev    *ped;
struct rbd      *prbd;
unsigned short  iobase;
{
    struct ep   *pep;
    int         ifnum = ped->ed_ifnum;

    pep = (struct ep *)nbgetbuf(Net.netpool);
    if (pep == 0) {
        if (ifnum >= 0 && ifnum < NIF)
            nif[ifnum].ni_idiscard++;
#ifdef PRINTERRORS
        kprintf("ee_get: ?? no buffer\n");
#endif
        return 0;
    }

    pep->ep_len = (prbd->rbd_count & RBD_COUNT_MASK);
#ifdef DEBUG
    kprintf("ee_get: len=%d\n", pep->ep_len);
#endif

    if (pep->ep_len - sizeof(struct eh) > EP_DLEN) {
#ifdef PRINTERRORS
        kprintf("ee: packet too large (%d)\n", pep->ep_len);
#endif
        nif[ifnum].ni_idiscard++;
        freebuf(pep);
        return 0;
    }

    /*
     * copy frame from on-board RAM to host memory
     */
    ee_ram_blk_read(iobase, prbd->rbd_buf, &pep->ep_eh, pep->ep_len);
    pep->ep_type = net2hs(pep->ep_type);
    pep->ep_order = EPO_NET;

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
