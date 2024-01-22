/* ee_wstrt.c - ee_wstrt */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <ee.h>

static count = 0;

/*#define DEBUG*/

/*------------------------------------------------------------------------
 * ee_wstrt - start output on an Intel EtherExpress
 *------------------------------------------------------------------------
 */
int ee_wstrt(ped)
struct etdev	*ped;
{
    STATWORD	ps;
    unsigned short iobase = ped->ed_pdev->dvcsr;
    struct ep	*pep;
    int		len, isbcast;

#ifdef DEBUG
    kprintf("ee_wstrt\n");
#endif
    
    disable(ps);
    
    if (ped->ed_xpending) {
#ifdef DEBUG
	kprintf("ee_wstrt: pending\n");
#endif	
	kprintf("ee_wstrt: pending\n");
	restore(ps);
	return OK;
    }
    
    ped->ed_xpending++;
    
    if (ped->ed_mcset) {
/*	ee_cmd(ped, CUCMD_MCSET|CUCMD_I, ped->ed_mca, ped->ed_mcc);*/
	restore(ps);
	kprintf("MCAST??\n");
	return OK;
    }
    
    pep = (struct ep *)deq(ped->ed_outq);
    if (pep == 0) {
	ped->ed_xpending--;
#ifdef DEBUG
    kprintf("ee_wstrt: pep==0\n");
#endif	
	restore(ps);
	return OK;
    }
    
    isbcast = blkequ(pep->ep_dst, ped->ed_bcast, EP_ALEN);
    len = pep->ep_len;
    
    kprintf("type=%x\n", pep->ep_type);
    
    /*
     * setup tx command list
     */
    ee_tx(ped, &pep->ep_eh, len, iobase);
    
#ifdef DEBUG
    ether_pr(ped, iobase);
    kprintf("len=%d (%d)\n", len, ((len + 1) >> 1));
#endif

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
int ee_tx2(ped, buf, len, iobase)
    struct etdev *ped;
    char   *buf;
    int    len;
    unsigned short iobase;
{
    int i;
    unsigned short w, *p;

#ifdef DEBUG
    kprintf("ee_tx...len=%d\n", len);
#endif
    
    /* set TX command block */
    outw(iobase+WRPTR, ped->ed_txcbl);
    outw(iobase, 0);				/* status */
    outw(iobase, (CUCMD_I | CUCMD_EL | CUCMD_TX));		/* command */
    outw(iobase, 0xffff);	/* next */
    outw(iobase, ped->ed_tbd);	/* tbd */
    
#ifdef DEBUG    
    outw(iobase+RDPTR, ped->ed_txcbl);
    kprintf("txcbl_off=%x ", ped->ed_txcbl);
    for (i = 0; i < 11; i++) {
	w = inw(iobase);
	kprintf("[%d]=%x ", i, w);
    }
    kprintf("\n");
#endif
    
    /* build TX buffer descriptor */
    outw(iobase+WRPTR, ped->ed_tbd);
    outw(iobase, (TBD_EOF | (TBD_ACNT_MASK & len)));	/* status */
    outw(iobase, 0xffff);		/* next */
    outw(iobase, ped->ed_txbuf);	/* txbuf lo */
    outw(iobase, 0);			/* txbuf hi */

#ifdef DEBUG
    outw(iobase+RDPTR, ped->ed_tbd);
    kprintf("tbd_off=%x ", ped->ed_tbd);
    for (i = 0; i <= 3; i++) {
	w = inw(iobase);
	kprintf("[%d]=%x ", i, w);
    }
    kprintf("\n");
#endif
    
    /*
     * Copy packet to tx bufer.
     * N.B. this version supports one packet per interrupt only
     */
    outw(iobase+WRPTR, ped->ed_txbuf);
    p = (unsigned short *)buf;
    for (i = 0; i < (len + 1) >> 1; ++i)
	outw(iobase, p[i]);
        
    /*
     * setup SCB entries
     */
    outw(iobase+SCB_STATUS, 0);
    outw(iobase+SCB_COMMAND, SCBCMD_CUS);
    outw(iobase+SCB_CBL, ped->ed_txcbl);
    
#ifdef DEBUG
    outw(iobase+RDPTR, SCB_OFFSET);
    kprintf("scb_off=%x ", SCB_OFFSET);
    for (i = 0; i <= 3; i++) {
	w = inw(iobase);
	kprintf("[%d]=%x ", i, w);
    }
    kprintf("\n");
#endif
    
    outb(iobase+CAC, 0);
}

/*-------------------------------------------------------------------------
 * ee_tx - transmit a frame
 *-------------------------------------------------------------------------
 */
int ee_tx(ped, buf, len, iobase)
    struct etdev *ped;
    char   *buf;
    int    len;
    unsigned short iobase;
{
    int    curr_nop = ped->ed_curr_nop, next_nop,i;
    unsigned short w, *p;

    next_nop = (curr_nop + 1) % 2;
#ifdef DEBUG
    kprintf("ee_tx...len=%d, curr_nop=%d\n", len, curr_nop);
#endif
    
    /*
     * Next NOP points to self
     */
    outw(iobase+WRPTR, ped->ed_nopcbl[next_nop]+4);
    outw(iobase, ped->ed_nopcbl[next_nop]);	/* next, point to self */
    
#ifdef DEBUG
    outw(iobase+RDPTR, ped->ed_nopcbl[next_nop]);
    kprintf("nopcbl[%d]_off=%x ", next_nop, ped->ed_nopcbl[next_nop]);
    for (i = 0; i < 3; i++) {
	w = inw(iobase);
	kprintf("[%d]=%x ", i, w);
    }
    kprintf("\n");
#endif
    
    /* set TX command block */
    outw(iobase+WRPTR, ped->ed_txcbl);
    outw(iobase, 0);				/* status */
    outw(iobase, (CUCMD_I | CUCMD_TX));		/* command */
    outw(iobase, ped->ed_nopcbl[next_nop]);	/* next */
    
#ifdef DEBUG
    outw(iobase+RDPTR, ped->ed_txcbl);
    kprintf("txcbl_off=%x ", ped->ed_txcbl);
    for (i = 0; i < 4; i++) {
	w = inw(iobase);
	kprintf("[%d]=%x ", i, w);
    }
    kprintf("\n");
#endif
    
    /* build TX buffer descriptor */
    outw(iobase+WRPTR, ped->ed_tbd);
    outw(iobase, (TBD_EOF | (TBD_ACNT_MASK & len)));	/* status */

#ifdef DEBUG
    outw(iobase+RDPTR, ped->ed_tbd);
    kprintf("tbd_off=%x ", ped->ed_tbd);
    for (i = 0; i <= 3; i++) {
	w = inw(iobase);
	kprintf("[%d]=%x ", i, w);
    }
    kprintf("\n");
#endif
    
    /*
     * Copy packet to tx bufer.
     * N.B. this version supports one packet per interrupt only
     */
    outw(iobase+WRPTR, ped->ed_txbuf);
    p = (unsigned short *)buf;
    for (i = 0; i < (len + 1) >> 1; ++i)
	outw(iobase, p[i]);
    
    ped->ed_curr_nop = next_nop;
    
    /*
     * make current NOP's next pointer point to TX cmd block
     */
    outw(iobase+WRPTR, ped->ed_nopcbl[curr_nop]+4);
    outw(iobase, ped->ed_txcbl);	/* next, point to txcbl */
    
#ifdef DEBUG
    outw(iobase+RDPTR, ped->ed_nopcbl[curr_nop]);
    kprintf("nopcbl[%d]_off=%x ", curr_nop, ped->ed_nopcbl[curr_nop]);
    for (i = 0; i < 3; i++) {
	w = inw(iobase);
	kprintf("[%d]=%x ", i, w);
    }
    kprintf("\n");
#endif
}


/*-------------------------------------------------------------------------
 * ether_pr - 
 *-------------------------------------------------------------------------
 */
int ether_pr(ped, iobase)
    struct etdev   *ped;
    unsigned short iobase;
{
    int i = 0;
    unsigned char b;
    unsigned short w;

    /*
     * print frame header in the TX buffer
     */
    outw(iobase+RDPTR, ped->ed_txbuf);

    b = inb(iobase);
    kprintf("ETHER: dst %02x", b);
    for (i=1; i<EP_ALEN; ++i) {
	b = inb(iobase);
	kprintf(":%02x", b);
    }

    b = inb(iobase);
    kprintf(" src %02x", b);
    for (i=1; i<EP_ALEN; ++i) {
	b = inb(iobase);
	kprintf(":%02x", b);
    }

    w = inw(iobase);
    kprintf("\nETHER: type %x\n", w);
}
