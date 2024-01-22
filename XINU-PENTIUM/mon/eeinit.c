/*
 * Author: John C. Lin (lin@cs.purdue.edu).
 * Modified from the version by Dave L. Stevens (dls@cc.purdue.edu).
 * Date: 02/09/95
 *
 * Ref: 1. For i82586: Intel LAN Components User's manual, March 1984.
 *         ISBN: 08359-4098-5
 *      2. EtherExpress 16 Family LAN Adapters - External Product Specification
 *         04/12/93
 *      3. Linux eexpress.c (driver by Donald Becker kecker@super.org.
 *      4. Linux AT&T/NCR WaveLAN driver by Bruce Janson
 *                                          (bruce@staff.cs.su.oz.au)
 *      5. Linux ni52.c by M.Hipp (Michael.Hipp@student.uni-tuebingen.de)
 *
 * Note: This version uses Port-I/O.
 */
#include <./mon/monnetwork.h>
#include <./mon/monitor.h>
#include <./mon/moni386.h>
#include <./mon/monee.h>

static unsigned char    mon_eeirq[] = {255, 9, 3, 4, 5, 10, 11, 255 };

unsigned char mon_eecf[] = {
    0x0c,       /* byte 6: BYTE CNT: 12 bytes of parameters     */
    0x08,       /* byte 7: FIFO LIM: 8                          */
    0x00,       /* byte 8: 0                                    */
    0x2e,       /* byte 9: PREM LEN: 8, AT LOC: 1, ADDR LEN: 6  */
    0x00,       /* byte10: 0                                    */
    0x20,       /* byte11: InterFrame Spacing: 32               */
    0x00,       /* byte12: SLOT TIME LOW: 0                     */
    0xf2,       /* byte13: SLOT TIME HI: 2, RETRY NUM: 15       */
    0x00,       /* byte14:                                      */
    0x00,       /* byte15: 0                                    */
    0x40,       /* byte16: MIN FRAME LEN: 64                    */
    0x00        /* byte17: UNUSED                               */
};

unsigned short  mon_board_type;
struct aldev    mon_ee[1];

/*#define DEBUG*/
/*#define CU_INIT_DEBUG*/
/*#define VERBOSE*/

/*------------------------------------------------------------------------
 * mon_ee_init - startup initialization of Intel EtherExpress device
 *------------------------------------------------------------------------
 */
int mon_ee_init()
{
    struct aldev        *ped;
    short  iobase;

    iobase = EE_IOBASE;

    ped = &mon_ee[0];
    ped->ed_outq = mon_newq(16);
    ped->ed_xpending = 0;

    /* reset 586 */
    EE_RESET(iobase);

    /*
     * check board
     */
    if (mon_ee_probe(iobase) == SYSERR) {
        kprintf("**ee: No EtherExpress card at 0x%03x\n", iobase);
        return SYSERR;
    }
     
    mon_ee_start(ped, iobase);
}


/*-------------------------------------------------------------------------
 * mon_ee_start - 
 *-------------------------------------------------------------------------
 */
int mon_ee_start(ped, iobase)
    struct aldev   *ped;
    unsigned short iobase;
{
    int i;

    /*
     * initialization, try 5 times
     */
    for (i = 0; i < 5; i++) {
        if (mon_ee_setup(ped, iobase) == OK)
            break;
    }

    if (i == 5) {
        kprintf("ee: Unable to initialize i82586\n");
        return SYSERR;
    }

    mon_ee_cu_init(ped, iobase);

    /* DIAG */
    if (mon_ee_cmd(ped, CUCMD_DIAG) != OK)
        kprintf("ee: i82586 Diag failed.\n");
       
    /* configure */
    if (mon_ee_cmd(ped, CUCMD_CONFIG, mon_eecf) != OK)
        kprintf("ee: i82586 Configure failed.\n");

    /* set station address */
    if (mon_ee_cmd(ped, CUCMD_IASET, ped->ed_paddr) != OK)
        kprintf("ee: i82586 set station address failed.\n");

    /*
     * start CU and RU
     */
    mon_ee_cu_start(ped, iobase);

    mon_ee_ru_start(ped, iobase);

    /*
     * enable interrupt
     */
    outb(iobase+SIRQ, ped->ed_irq | SIRQ_IEN);
}


/*-------------------------------------------------------------------------
 * mon_ee_setup -
 *-------------------------------------------------------------------------
 */
int mon_ee_setup(ped, iobase)
    struct aldev   *ped;
    unsigned short iobase;
{
    unsigned short      rom[64], eedat;
    unsigned char       irq, tmp;
    int i, mon_ee_int();
    unsigned short status;

    /* reset 586 */
    EE_RESET(iobase);

        /* read ROM data */
    for (i=0; i<EE_ROMSIZE; ++i) {
        rom[i] = mon_ee_romread(iobase, i);
    }
    
    /* set IRQ */
    irq = EE_IRQ; 
    for (i=0; i<sizeof(mon_eeirq); ++i) {
        if (mon_eeirq[i] == irq) {
            ped->ed_irq = i;
            break;
        }
    }

    if (i == sizeof(mon_eeirq)) {
        kprintf("ee0: invalid IRQ (%d)\n", irq);
        return SYSERR;
    } else {
        eedat = rom[EE_BOARD];
        eedat >>= 13;
        if (mon_eeirq[eedat] != irq)
            kprintf("ee: WARNING ROM IRQ (%d) differs\n", mon_eeirq[eedat]);
        ped->ed_irq = eedat;
        irq = mon_eeirq[eedat];
    }
      
    set_evec(IRQBASE+irq, mon_ee_int);

    /* set up SCP */
    outw(iobase+WRPTR, I82586_SCP);
    outw(iobase+DXREG, SYSBUS_16);      /* 16-bit bus */
    outw(iobase+DXREG, 0);      /* skip unused words */
    outw(iobase+DXREG, 0);
    outw(iobase+DXREG, I82586_ISCP & 0xffff);
    outw(iobase+DXREG, I82586_ISCP >> 16);

    for (i=0; i<EP_ALEN/2 ; ++i) {
        eedat = rom[EE_EADDR + i];

        ped->ed_paddr[EP_ALEN - 2*i -1] = eedat & 0xff;
        ped->ed_paddr[EP_ALEN - 2*i -2] = eedat >> 8;
        ped->ed_bcast[2*i] = ~0;
        ped->ed_bcast[2*i+1] = ~0;
    }

#ifdef VERBOSE
    kprintf("ee: Ethernet Address: %02x:%02x:%02x:%02x:%02x:%02x ",
            ped->ed_paddr[0] & 0xff,
            ped->ed_paddr[1] & 0xff,
            ped->ed_paddr[2] & 0xff,
            ped->ed_paddr[3] & 0xff,
            ped->ed_paddr[4] & 0xff,
            ped->ed_paddr[5] & 0xff);
    kprintf("irq %d iobase %x\n", irq, iobase);
#endif
    
    mon_ee_init_mem(ped, iobase);

    EE_CA(iobase);		/* Channel attention */

#ifdef DEBUG
    status = inw(iobase + EE_SCB_STATUS);
    kprintf("82586 started, status = 0x%X\n", status);
    tmp = inb(iobase + EEC);
    kprintf("EEC = %X\n", tmp);
    tmp = inb(iobase + CONFIG);
    kprintf("CONFIG=%X\n", tmp);
#endif

    /* check the busy bit in ISCP */
    for (i = 2000; i > 0; --i) {
        outw(iobase + RDPTR, I82586_ISCP);
        tmp = inb(iobase);
        if (tmp == 0)
            break;
        DELAY(1);
    }
       
    if (i <= 0) {
        status = inw(iobase + EE_SCB_STATUS);
        kprintf("ee: failed initialization, status = %X\n", status);
        return SYSERR;
    }
       
    for (i = 2000; i > 0; --i) {
        status = inw(iobase + EE_SCB_STATUS);
        if (status == SCBSTAT_CX|SCBSTAT_CNA)
            break;
        DELAY(1);
    }

    if (i <= 0) {
        kprintf("ee: failed status check (status %X)\n", status);
        return SYSERR;
    }

    return OK;
}

/*-------------------------------------------------------------------------
 * mon_ee_init_mem -
 *-------------------------------------------------------------------------
 */
int mon_ee_init_mem(ped, iobase)
    struct aldev   *ped;
    unsigned short iobase;
{
    int i;

    /* clear it */
    outw(iobase+WRPTR, 0);
    for (i = 0; i < (EE_RAM_SIZE + 1) >> 1; i++)
        outw(iobase, 0);

    /* set up SCP */
    outw(iobase+WRPTR, I82586_SCP);
    outw(iobase, SYSBUS_16); /* 16-bit bus */
    outw(iobase, 0);    /* skip unused words */
    outw(iobase, 0);
    outw(iobase, I82586_ISCP & 0xffff);
    outw(iobase, I82586_ISCP >> 16);

    /* setup ISCP */
    outw(iobase+WRPTR, I82586_ISCP);
    outw(iobase, 0x01);         /* 82586 busy   */
    outw(iobase, EE_SCB_OFFSET);
    outw(iobase, 0);            /* SCB base     */
    outw(iobase, 0);

    /* SCB offset */
    ped->ed_scb = EE_SCB_OFFSET;
}

/*-------------------------------------------------------------------------
 * mon_ee_cu_init - initialize TX related structures
 *-------------------------------------------------------------------------
 */
int mon_ee_cu_init(ped, iobase)
    struct aldev   *ped;
    unsigned short iobase;
{
    unsigned short offset;
    struct nopcbl  nop;
    struct txcbl   tx;
    struct tbd     tbd;

    offset = EE_SCB_OFFSET + sizeof(struct scb);

    /*
     * Allocate space for 4 command blocks:
     * one for others, two NOP commands, a TX command.
     */
    /*
     * for other commands
     */
    ped->ed_cbl = offset;
    offset += sizeof(struct cbl);

    /*
     * first NOP command
     */
    ped->ed_nopcbl[0] = offset;
    nop.cbl_status = 0;
    nop.cbl_cmd = CUCMD_NOP;
    nop.cbl_next = ped->ed_nopcbl[0];   /* point to self */
    mon_ee_ram_blk_write(iobase, ped->ed_nopcbl[0], &nop, sizeof(nop));
    offset += sizeof(struct nopcbl);
#ifdef CU_INIT_DEBUG
    {
        struct nopcbl nop;
        mon_ee_ram_blk_read(iobase, ped->ed_nopcbl[0], &nop, sizeof(nop));
        kprintf("ed_nopcbl[0]=%x, st=%x, cmd=%x, nx=%x\n", ped->ed_nopcbl[0],
                nop.cbl_status, nop.cbl_cmd, nop.cbl_next);
    }
#endif

    /*
     * another NOP
     */
    ped->ed_nopcbl[1] = offset;
    nop.cbl_status = 0;
    nop.cbl_cmd = CUCMD_NOP;
    nop.cbl_next = ped->ed_nopcbl[1];   /* point to self */
    mon_ee_ram_blk_write(iobase, ped->ed_nopcbl[1], &nop, sizeof(nop));
    offset += sizeof(struct nopcbl);
#ifdef CU_INIT_DEBUG
    {
        struct nopcbl nop;
        mon_ee_ram_blk_read(iobase, ped->ed_nopcbl[1], &nop, sizeof(nop));
        kprintf("ed_nopcbl[1]=%x, st=%x, cmd=%x, nx=%x\n", ped->ed_nopcbl[1],
                nop.cbl_status, nop.cbl_cmd, nop.cbl_next);
    }
#endif

    /*
     * construct a TX command block
     */
    ped->ed_txcbl = offset;
    tx.cbl_status = 0;
    tx.cbl_cmd = CUCMD_TX | CUCMD_I;
    tx.cbl_next = 0xffff;
    tx.cbl_tbd = offset + sizeof(struct txcbl); /* points to tbd */
    mon_ee_ram_blk_write(iobase, ped->ed_txcbl, &tx, sizeof(tx));
    offset += sizeof(struct txcbl);

#ifdef CU_INIT_DEBUG
    {
        struct txcbl tx;
        mon_ee_ram_blk_read(iobase, ped->ed_txcbl, &tx, sizeof(tx));

        kprintf("ped->ed_txcbl=%x, st=%x cmd=%x nx=%x tbd=%x\n", ped->ed_txcbl,
            tx.cbl_status, tx.cbl_cmd, tx.cbl_next, tx.cbl_tbd);
    }
#endif

    /* allocate transmit buffer descriptor */
    ped->ed_tbd = offset;
    tbd.tbd_status = 0;
    tbd.tbd_next = 0xffff;
    tbd.tbd_buf = offset + sizeof(tbd);
    mon_ee_ram_blk_write(iobase, ped->ed_tbd, &tbd, sizeof(tbd));

    /* allocate transmit buffer */
    ped->ed_txbuf = offset + sizeof(tbd);

#ifdef CU_INIT_DEBUG
    {
        struct tbd tbd;
        mon_ee_ram_blk_read(iobase, ped->ed_tbd, &tbd, sizeof(tbd));

        kprintf("ped->ed_tbd=%x, st=%x nx=%x tbd=%x, tx_buf=%x\n",
                ped->ed_tbd, tbd.tbd_status, tbd.tbd_next, tbd.tbd_buf,
                ped->ed_txbuf);
    }
#endif
}

/*-------------------------------------------------------------------------
 * mon_ee_cu_start - start CU. CU executes on a endless NOP-loop
 *-------------------------------------------------------------------------
 */
int mon_ee_cu_start(ped, iobase)
    struct aldev        *ped;
    unsigned short      iobase;
{
    struct scb     scb;
    struct nopcbl  nop;

    ped->ed_curr_nop = 0;

    /* a NOP loop */
    nop.cbl_status = 0;
    nop.cbl_cmd = CUCMD_NOP;
    nop.cbl_next = ped->ed_nopcbl[0];   /* point to self */
    mon_ee_ram_blk_write(iobase, ped->ed_nopcbl[0], &nop, sizeof(nop));

    /* start 82586 Command Unit */
    bzero(&scb, sizeof(scb));
    scb.scb_cmd = SCBCMD_CUS;
    scb.scb_status = 0;
    scb.scb_cbloff = ped->ed_nopcbl[0];
    mon_ee_ram_blk_write(iobase, EE_SCB_OFFSET, &scb, sizeof(scb));
    EE_CA(iobase);	/* CA */
    mon_ee_wait(ped);

#ifdef DEBUG
    kprintf("ee0: CU started, status=%X\n", EE_READ_SCB_STATUS(iobase));
#endif
}

/*-------------------------------------------------------------------------
 * mon_ee_ru_start - init RX data structures and start RU
 *-------------------------------------------------------------------------
 */
int mon_ee_ru_start(ped, iobase)
    struct aldev        *ped;
    unsigned short      iobase;
{
    unsigned short      offset, rbd_offset, first_rbd;
    unsigned short      rfd_offset, buf_offset, i;
    struct rfd          rfd;
    struct rbd          rbd;
    struct scb          scb;

    offset = ped->ed_txbuf + BIG_FRAME_SIZE;    /* right after TX buffer */

    /*
     * allocate RFDs, RBDs, then RX buffers
     */
    rfd_offset = ped->ed_rfd = offset;  /* 1st receive frame descriptor */
    offset += EE_RFD_NUM * sizeof(struct rfd);
    rbd_offset = first_rbd = offset;
    offset += EE_RFD_NUM * sizeof(struct rbd);
    buf_offset = offset;

    for (i = 1; i <= EE_RFD_NUM; ++i) {
        rfd.rfd_status = 0;
        if (i == EE_RFD_NUM) {
            rfd.rfd_cmd = (RFD_EL | RFD_S); /* mark end of receive FD list */
            rfd.rfd_next = ped->ed_rfd;     /* make it circular */
            ped->ed_rfdend = rfd_offset;
        }
        else {
            rfd.rfd_cmd = 0;
            rfd.rfd_next = rfd_offset + sizeof(struct rfd);
        }
        if (i == 1)
            rfd.rfd_rbd = rbd_offset;   /* RBD of the first RFD */
        else
            rfd.rfd_rbd = 0xffff;       /* no RBD               */
        mon_ee_ram_blk_write(iobase, rfd_offset, &rfd, sizeof(rfd));

        rbd.rbd_count = 0;
        if (i == EE_RFD_NUM)
	    rbd.rbd_next = first_rbd;   /* make it circular */
        else
            rbd.rbd_next = rbd_offset + sizeof(rbd);
	
        rbd.rbd_buf = buf_offset;
        rbd.rbd_size = BIG_FRAME_SIZE;
        mon_ee_ram_blk_write(iobase, rbd_offset, &rbd, sizeof(rbd));

        rfd_offset += sizeof(struct rfd);
        rbd_offset += sizeof(rbd);
        buf_offset += BIG_FRAME_SIZE;
    }

    /*
     * start RU
     */
    scb.scb_rfaoff = ped->ed_rfd;
    scb.scb_cmd = SCBCMD_RUS;
    scb.scb_status = 0;
    mon_ee_ram_blk_write(iobase, EE_SCB_OFFSET, &scb, sizeof(scb));
    EE_CA(iobase);
    mon_ee_wait(ped);

#ifdef RU_INIT_DEBUG
    kprintf("ee_ru_init: iobase=%x, ed_rfd=%x ed_rfdend=%x\n", iobase,
            ped->ed_rfd, ped->ed_rfdend);
#endif

#ifdef DEBUG
    kprintf("ee0: RU started, status=%X\n", EE_READ_SCB_STATUS(iobase));
#endif
}

/*-------------------------------------------------------------------------
 * mon_ee_probe - check board ID and verify checksum
 *-------------------------------------------------------------------------
 */
int mon_ee_probe(iobase)
    unsigned short iobase;
{
    unsigned short      id;
    unsigned char 	b, count;
    int i;
    char *bus_type;

    /*
     * check board ID
     */
    b = inb(iobase + AID);
    count = b & 0xf;
    id = (b >> 4);
    for (i = 1; i < 4; i++) {
        b = inb(iobase + AID);
        id |= (b >> 4) << (i << 2);
        /*
         * verify counter register
         */
        if ((b & 0xf) != (++count % 16)) {
            kprintf("ee: AUTO ID counter check failed\n");
            return SYSERR;
        }
    }
    if (id != EE_BOARD_ID) {
        kprintf("ee: verify ID failed (id = %x)\n", id);
        return SYSERR;
    }

    /*
     * determine board type; access the Shadow Auto ID byte
     */
    b = inb(iobase+AUTOID_OFFSET);
    count = b & 0xf;
    id = (b >> 4);
    for (i = 1; i < 4; i++) {
        b = inb(iobase+AUTOID_OFFSET);
        id |= (b >> 4) << (i << 2);
        /*
         * verify counter register
         */
        if ((b & 0xf) != (++count % 16)) {
	    kprintf("ee: Shadow AUTO ID counter check failed\n");
            return SYSERR;
        }
    }
    mon_board_type = id;

    /*
     * determine bus type
     */
    b = inb(iobase+ECR1_OFFSET);
    bus_type = (b & BUS_TYPE) ? "MCA bus" : "ISA bus";

#ifdef VERBOSE
    if (mon_board_type == EE_16_1ST)
        kprintf("EtherExpress 16/16TP (1st generation) board (%s)\n",bus_type);
    else if (mon_board_type == EE_16_2ND)
        kprintf("EtherExpress 16C/16C_Flash or 16/16TP (2nd Ed.) board (%s)\n",
		bus_type);
    else if (mon_board_type == EE_MCA)
        kprintf("EtherExpress MCA board (%s)\n", bus_type);
    else
        kprintf("EtherExpress board type unknown (ID=%x) (%s)\n",
		mon_board_type, bus_type);
#endif
    
    return OK;
}

/*-------------------------------------------------------------------------
 * mon_ee_ram_blk_write - write a block of data to on-board RAM
 *-------------------------------------------------------------------------
 */
int mon_ee_ram_blk_write(iobase, offset, buf, len)
    unsigned short iobase;
    unsigned short offset;
    char *buf;                  /* buffer containing data to be written */
    int  len;                   /* # of octets to be written            */
{
    register int i;
    register unsigned short *w = (unsigned short *)buf;

    len = (len + 1) >> 1;       /* # of words   */

    outw(iobase+WRPTR, offset);
    outsw(iobase, (unsigned short *)buf, len);
}

/*-------------------------------------------------------------------------
 * mon_ee_ram_blk_read - Read a block of data from on-board RAM
 *-------------------------------------------------------------------------
 */
int mon_ee_ram_blk_read(iobase, offset, buf, len)
    unsigned short iobase;
    unsigned short offset;
    char *buf;                  /* buffer containing data to be read    */
    int  len;                   /* # of octets to be read               */
{
    register int i;
    register unsigned short *w = (unsigned short *)buf;

    len = (len + 1) >> 1;       /* # of words   */

    outw(iobase+RDPTR, offset);
    insw(iobase, (unsigned short *)buf, len);
}
