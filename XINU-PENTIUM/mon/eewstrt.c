#include <./mon/monnetwork.h>
#include <./mon/monitor.h>
#include <./mon/monee.h>

/*#define DEBUG*/

/*------------------------------------------------------------------------
 * mon_ee_wstrt - start output on an Intel EtherExpress
 *------------------------------------------------------------------------
 */
int mon_ee_wstrt(ped)
struct aldev    *ped;
{
    STATWORD            ps;
    static int 		pcount = 0;
    unsigned short      iobase;
    struct ep           *pep;
    int                 len, isbcast;

#ifdef DEBUG
    kprintf("[mon_ee_wstrt]");
#endif

    disable(ps);

    iobase = EE_IOBASE;

    if (ped->ed_xpending) {
#ifdef DEBUG
        kprintf("[p]");
#endif
	if (++pcount <= 3) {
	    restore(ps);
	    return OK;
	}
	ped->ed_xpending = 0;	/* we may have lost an tx int ACK */
	kprintf("[?]");
    }
    pcount = 0;

    /*
     * get the first packet from the queue without removing it
     */
    pep = (struct ep *)mon_deq(ped->ed_outq);
    if (pep == 0) {
#ifdef DEBUG
    kprintf("mon_ee_wstrt: pep==0\n");
#endif
        kprintf("mon_ee_wstrt: empty output queue\n");
        restore(ps);
        return OK;
    }

    isbcast = mon_blkequ(pep->ep_dst, ped->ed_bcast, EP_ALEN);
    len = pep->ep_len;

    /*
     * Copy frame to on-board RAM.
     * N.B. this version supports one packet per interrupt only
     */
    mon_ee_ram_blk_write(iobase, ped->ed_txbuf, &pep->ep_eh, len);

#ifdef DEBUG
    kprintf("len=%d\n", len);
#endif

    /*
     * transmit packet
     */
    mon_ee_tx(ped, len, iobase);

    freebuf(pep);

    restore(ps);
}

/*-------------------------------------------------------------------------
 * mon_ee_tx - transmit a frame
 *-------------------------------------------------------------------------
 */
int mon_ee_tx(ped, len, iobase)
    struct aldev *ped;
    int    len;
    unsigned short iobase;
{
    int    curr_nop, next_nop;

    ped->ed_xpending = 1;
    curr_nop = ped->ed_curr_nop;
    next_nop = (curr_nop + 1) % 2;

#ifdef DEBUG
    kprintf("mon_ee_tx...len=%d, cnop=%d, nnop=%d\n", len, curr_nop, next_nop);
#endif

    /* build TX buffer descriptor */
    outw(iobase+WRPTR, ped->ed_tbd);
    outw(iobase, (TBD_EOF | (TBD_ACNT_MASK & len)));    /* status */

#ifdef DEBUG
    {
        struct tbd tbd;
        mon_ee_ram_blk_read(iobase, ped->ed_tbd, &tbd, sizeof(tbd));
        kprintf("tbd_status = %x, tbd_next=%x, tbd_buf=%x\n",
                tbd.tbd_status, tbd.tbd_next, tbd.tbd_buf);
    }
#endif

    /* set TX command block: clear status and point to NOP */
    outw(iobase+WRPTR, ped->ed_txcbl);
    outw(iobase, 0);
    outw(iobase+WRPTR, ped->ed_txcbl+4);
    outw(iobase, ped->ed_nopcbl[next_nop]);

#ifdef DEBUG
    {
        struct txcbl txcbl;
        mon_ee_ram_blk_read(iobase, ped->ed_txcbl, &txcbl, sizeof(txcbl));
        kprintf("txcbl_status = %x, cmd=%x, next=%x, tbd=%x\n",
                txcbl.cbl_status, txcbl.cbl_cmd, txcbl.cbl_next,
                txcbl.cbl_tbd);
    }
#endif

    /*
     * Next NOP points to self
     */
    outw(iobase+WRPTR, ped->ed_nopcbl[next_nop]+4);
    outw(iobase, ped->ed_nopcbl[next_nop]);
    
    /*
     * make current NOP's next pointer point to TX cmd block
     */
    outw(iobase+WRPTR, ped->ed_nopcbl[curr_nop]+4);
    outw(iobase, ped->ed_txcbl);

    ped->ed_curr_nop = next_nop;
}
