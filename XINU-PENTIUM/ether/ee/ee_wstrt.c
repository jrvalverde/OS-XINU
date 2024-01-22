/* ee_wstrt.c - ee_wstrt */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <ee.h>

/*#define DEBUG*/

/*------------------------------------------------------------------------
 * ee_wstrt - start output on an Intel EtherExpress
 *------------------------------------------------------------------------
 */
int ee_wstrt(ped)
struct aldev    *ped;
{
    STATWORD            ps;
    unsigned short      iobase;
    struct ep           *pep;
    int                 len, isbcast;

#ifdef DEBUG
    kprintf("[ee_wstrt]");
#endif

    disable(ps);

    iobase = ped->ed_pdev->dvcsr;

    if (ped->ed_xpending) {
#ifdef DEBUG
        kprintf("[p]");
#endif
        restore(ps);
        return OK;
    }

    if (ped->ed_mcset) {
/*      ee_cmd(ped, EECMD_MCSET|EECMD_I, ped->ed_mca, ped->ed_mcc);*/
        restore(ps);
        kprintf("MCAST??\n");
        return OK;
    }

    /*
     * get the first packet from the queue without removing it
     */
    pep = (struct ep *)deq(ped->ed_outq);
    if (pep == 0) {
#ifdef DEBUG
    kprintf("ee_wstrt: pep==0\n");
#endif
        kprintf("ee_wstrt: empty output queue\n");
        restore(ps);
        return OK;
    }

    isbcast = blkequ(pep->ep_dst, ped->ed_bcast, EP_ALEN);
    len = pep->ep_len;

    /*
     * Copy frame to on-board RAM.
     * N.B. this version supports one packet per interrupt only
     */
    ee_ram_blk_write(iobase, ped->ed_txbuf, &pep->ep_eh, len);

#ifdef DEBUG
    ee_ether_pr(iobase, ped->ed_txbuf);
    kprintf("len=%d\n", len);
#endif

    /*
     * transmit packet
     */
    ee_tx(ped, len, iobase);

    freebuf(pep);

    restore(ps);

    if (isbcast)
        nif[ped->ed_ifnum].ni_onucast++;
    else
        nif[ped->ed_ifnum].ni_oucast++;

    nif[ped->ed_ifnum].ni_ooctets += len;
}

/*-------------------------------------------------------------------------
 * ee_tx - transmit a frame
 *-------------------------------------------------------------------------
 */
int ee_tx(ped, len, iobase)
    struct aldev *ped;
    int    len;
    unsigned short iobase;
{
    int    curr_nop, next_nop;

    ped->ed_xpending = 1;
    curr_nop = ped->ed_curr_nop;
    next_nop = (curr_nop + 1) % 2;

#ifdef DEBUG
    kprintf("ee_tx...len=%d, cnop=%d, nnop=%d\n", len, curr_nop, next_nop);
#endif

    /* build TX buffer descriptor */
    outw(iobase+WRPTR, ped->ed_tbd);
    outw(iobase, (TBD_EOF | (TBD_ACNT_MASK & len)));    /* status */

#ifdef DEBUG
    {
        struct tbd tbd;
        ee_ram_blk_read(iobase, ped->ed_tbd, &tbd, sizeof(tbd));
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
        ee_ram_blk_read(iobase, ped->ed_txcbl, &txcbl, sizeof(txcbl));
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

/*-------------------------------------------------------------------------
 * ee_ether_pr -
 *-------------------------------------------------------------------------
 */
int ee_ether_pr(iobase, offset)
    unsigned short iobase;
    unsigned short offset;
{
    unsigned char p[16];
    int i = 0, j;
    struct eh *pep = (struct eh *)p;

    ee_ram_blk_read(iobase, offset, p, 16);

    kprintf("ETHER: dst %02x", p[i]);
    for (i=1; i<EP_ALEN; ++i)
        kprintf(":%02x", p[i]);

    i = EP_ALEN;
    kprintf(" src %02x", p[i++]);
    for (j=1; j<EP_ALEN; ++j, ++i)
        kprintf(":%02x", p[i]);

    kprintf("\nETHER: type %x\n", pep->eh_type);
}
