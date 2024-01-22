/* ee_init.c - ee_init */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <i386.h>
#include <ee.h>
#include <q.h>

static unsigned char	eeirq[] = {255, 9, 3, 4, 5, 10, 11, 255 };

unsigned short eecf[] = {
    0x080c,	/* byte  7-6: FIFO LIM: 8, BYTE CNT: 12 bytes of parameters */
    0x2e00,	/* byte  9-8: PREM LEN: 8, AT LOC: 1, ADDR LEN: 6	    */
    0x2000,	/* byte11-10: InterFrame Spacing: 32			    */
    0xf200,	/* byte13-12: RETRY NUM: 15, SLOT TIME: 0x200		    */
    0x0000,	/* byte15-14: 0						    */
    0x0040,	/* byte17-16: MIN FRAME LEN: 64				    */
};

/*#define DEBUG*/
#define CU_START_DEBUG
#define RU_START_DEBUG

/*------------------------------------------------------------------------
 * ee_init - startup initialization of Intel EtherExpress device
 *------------------------------------------------------------------------
 */
int ee_init(pdev)
struct devsw	*pdev;
{
    struct etdev	*ped;
    unsigned short	iobase, eedat, rom[64];
    int			i, irq, ee_int();

    pdev->dvioblk = (char *) ped = &ee[pdev->dvminor];
    ped->ed_mcset = 0;	/* no multicast */
    ped->ed_pdev = pdev;
    ped->ed_outq = newq(ETOUTQSZ, QF_NOWAIT);
    ped->ed_ifnum = -1;
    ped->ed_descr = "Intel EtherExpress";
    ped->ed_xpending = 0;
    ped->ed_mcc = 0;

    iobase = ped->ed_pdev->dvcsr;
#ifdef DEBUG
    kprintf("iobase=0x%x\n", iobase);
#endif
    
    /* read ROM data */
    for (i=0; i<EE_ROMSIZE; ++i) {
	rom[i] = ee_romread(iobase, i);
    }
    kprintf("rom[0]=%X, [1]=%X, [2]=%X, [6]=%X\n", rom[0], rom[1], rom[2],
	    rom[6]);

    for (i=0; i<16; ++i)
	if ((inb(iobase + AID) & 0xf) != i)
	    break;
    
    if (i < 16) {
	kprintf("ee_init: no EtherExpress at 0x%03x\n", iobase);
	return SYSERR;
    }
	
    /* set IRQ */
    irq = pdev->dvivec - IRQBASE;
    for (i=0; i<sizeof(eeirq); ++i)
	if (eeirq[i] == irq) {
	    ped->ed_irq = i;
	    break;
	}
    if (i == sizeof(eeirq)) {
	kprintf("ee%d: invalid IRQ (%d)\n", irq);
	return SYSERR;
    } else {
	eedat = rom[EE_BOARD];
	eedat >>= 13;
	if (eeirq[eedat] != irq)
	    kprintf("ee%d: WARNING ROM IRQ (%d) differs\n",
		    pdev->dvminor, eeirq[eedat]);
	ped->ed_irq = eedat;
	irq = eeirq[eedat];
    }

    
    for (i=0; i<EP_ALEN/2 ; ++i) {
	eedat = rom[EE_EADDR + i];

	ped->ed_paddr[EP_ALEN - 2*i -1] = eedat & 0xff;
	ped->ed_paddr[EP_ALEN - 2*i -2] = eedat >> 8;
	ped->ed_bcast[2*i] = ~0;
	ped->ed_bcast[2*i+1] = ~0;
    }
    
#ifdef DEBUG
    kprintf("ee%d: etheraddr %02x:%02x:%02x:%02x:%02x:%02x ",
	    pdev->dvminor,
	    ped->ed_paddr[0] & 0xff,
	    ped->ed_paddr[1] & 0xff,
	    ped->ed_paddr[2] & 0xff,
	    ped->ed_paddr[3] & 0xff,
	    ped->ed_paddr[4] & 0xff,
	    ped->ed_paddr[5] & 0xff);
    kprintf("irq %d\n", irq);
#endif
    
    set_evec(pdev->dvivec, ee_int);
    
    init_82586(ped);
}


/*-------------------------------------------------------------------------
 * init_82586 - 
 *-------------------------------------------------------------------------
 */
int init_82586(ped)
    struct etdev	*ped;
{
    unsigned short	iobase, status;
    unsigned char	tmp;
    int i;

#ifdef DEBUG
    kprintf("init_82586.....\n");
#endif
    
    iobase = ped->ed_pdev->dvcsr;

    outb(iobase + EEC, EEC_586R);  	/* reset 82586          */
    outb(iobase + EEC, 0);		/* clear reset          */
    
    /* "warm up" DRAMS-- requires 16 bytes buf access */
    outb(iobase + RDPTR, 0);
    for (i=0; i<16; ++i)
	inw(iobase);

    /* init 82586 on-board memory */
    ee_init_mem(iobase, ped);

#ifdef DEBUG
    kprintf("init on-board DRAM data structures...\n");
#endif
    
    outb(iobase + CAC, 0);	/* Channel attention */
    
#ifdef DEBUG
    status = inw(iobase + SCB_STATUS);
    kprintf("82586 started, status = 0x%X\n", status);
    tmp = inb(iobase + EEC);
    kprintf("EEC = %X\n", tmp);
    tmp = inb(iobase + CONFIG);
    kprintf("CONFIG=%X\n", tmp);
#endif

    /* check the busy bit in ISCP */
    for (i = 2000; i > 0; --i) {
	outw(iobase + RDPTR, RAM_ISCP_OFFSET);
	tmp = inb(iobase);
	if (tmp == 0)
	    break;
	DELAY(1);
    }
	
    if (i <= 0) {
	status = inw(iobase + SCB_STATUS);
	kprintf("ee: failed initialization, status = %X\n", status);
	return SYSERR;
    }
	
    for (i = 2000; i > 0; --i) {
	status = inw(iobase + SCB_STATUS);
	if (status == SCBSTAT_CX|SCBSTAT_CNA)
	    break;
	DELAY(1);
    }
    
    if (i <= 0) {
	kprintf("ee: failed status check (status %X)\n", status);
	return SYSERR;
    }

#ifdef DEBUG
    kprintf("Diag...\n");
#endif
    /* DIAG */
    i = ee_cmd(ped, CUCMD_DIAG);
    kprintf("i82586 Diag %s.\n", (i == OK) ? "ok" : "failed");
    
#ifdef DEBUG
    kprintf("Configure...\n");
#endif    
    /* configure */
    i = ee_cmd(ped, CUCMD_CONFIG, eecf);
    kprintf("Configure i82586 %s.\n", (i == OK) ? "ok" : "failed");

#ifdef DEBUG
    kprintf("Set station address...\n");
#endif    
    /* set station address */
    i = ee_cmd(ped, CUCMD_IASET, ped->ed_paddr);
    kprintf("Set host Ethernet address %s.\n", (i == OK) ? "ok" : "failed");
    
    /* start 82586 Receive Unit */
    outw(iobase + SCB_COMMAND, SCBCMD_RUS);
    outw(iobase + SCB_STATUS, 0);
    outb(iobase + CAC, 0);
    ee_wait(iobase);
    ee_ack(iobase);
    
#ifdef DEBUG
    kprintf("RU started\n");
    status = inw(iobase + SCB_STATUS);
    kprintf("status=%X\n", status);
#endif
    
    /*
     * start 82586 Command Unit
     * NOTE: don't execute any commnd after starting the CU
     */
    outw(iobase + SCB_COMMAND, SCBCMD_CUS);
    outw(iobase + SCB_STATUS, 0);
    outw(iobase + SCB_CBL, ped->ed_nopcbl[0]);
    outb(iobase + CAC, 0);	
    ee_wait(iobase);
    ee_ack(iobase);
    
#ifdef DEBUG
    kprintf("CU started\n");
    status = inw(iobase + SCB_STATUS);
    kprintf("status=%X\n", status);
#endif
    
    /* enable interrupt */
    outb(iobase+SIRQ, ped->ed_irq | SIRQ_IEN);

    return OK;
}

/*#define INIT_MEM_DEBUG*/

/*-------------------------------------------------------------------------
 * ee_init_mem - 
 *-------------------------------------------------------------------------
 */
int ee_init_mem(iobase, ped)
    unsigned short iobase;
    struct etdev   *ped;
{
    int i;

    /* clear it */
    outw(iobase+WRPTR, 0);
    for (i = 0; i < (EE_DRAM_SIZE + 1) >> 1; i++)
	outw(iobase, 0);
    
    /* set up SCP */
    outw(iobase+WRPTR, SCP);
    outw(iobase, SYSBUS_16); /* 16-bit bus */
    outw(iobase, 0); 	/* skip unused words */
    outw(iobase, 0);
    outw(iobase, ISCP & 0xffff);
    outw(iobase, ISCP >> 16);

    /* setup ISCP */
    outw(iobase+WRPTR, ISCP);
    outw(iobase, 0x01); 	/* 82586 busy	*/
    outw(iobase, SCB_OFFSET);
    outw(iobase, 0);		/* SCB base	*/
    outw(iobase, 0);

    /* setup SCB */
    ped->ed_scb = SCB_OFFSET;
    
    /* allocate a generat command block */
    ped->ed_cbl = SCB_OFFSET + sizeof(scb);
    outw(iobase+WRPTR, offset);
    outw(iobase, 0x0); 		/* status 	*/
    outw(iobase, (CUCMD_EL | CUCMD_NOP));	/* command 	*/
    outw(iobase, 0xffff);		/* null next command */
#ifdef INIT_MEM_DEBUG
    kprintf("CBL=%x\n", offset);
#endif
    }


/*-------------------------------------------------------------------------
 * ee_cu_start - init tx data structures and start CU.
 *		 The first command to CU is a infinite-loop NOP command
 *-------------------------------------------------------------------------
 */
int ee_cu_start(ped)
    struct etdev   *ped;    
{
    unsigned short iobase, offset, tbd, nop, buf;
    int count = 0;

    iobase = ped->ed_pdev->dvcsr;

    offset = ped->ed_txint = ped->ed_txcbl = RAM_TX_BEGIN;
    ped->ed_nop = RAM_TX_BEGIN + sizeof(txcbl) + sizeof(tbd);

    while (offset < RAM_TX_END) {
	/*
	 * allocate a TX cmd, a TX buffter descriptor, a NOP cmd, and
	 * a TX buffer.
	 */
	tbd = offset + sizeof(txcbl);
	nop = tbd + sizeof(tbd);
	buf = nop + sizeof(nopcbl);
	    
	/* allocate a TX command block */
	outw(iobase+WRPTR, offset);
	outw(iobase, 0x0); 			/* status 	*/
	outw(iobase, (CUCMD_I | CUCMD_TX));	/* command 	*/
	outw(iobase, nop);			/* point to NOP */
	outw(iobase, tbd);			/* point to TDB */
    
#ifdef CU_START_DEBUG
	{
	    unsigned short w, i;
	
	    kprintf("txcbl_head=%x\n", offset);
	    outw(iobase+RDPTR, ped->ed_txcbl);
	    kprintf("txcbl_off=%x ", ped->ed_txcbl);
	    for (i = 0; i < 4; i++) {
		w = inw(iobase);
		kprintf("[%d]=%x ", i, w);
	    }
	    kprintf("\n");
	}
#endif

	/* a TX buffer descriptor */
	outw(iobase+WRPTR, tbd);
	outw(iobase, 0x0); 	/* status 	*/
	outw(iobase, 0xffff);	/* null next tbd */
	outw(iobase, buf);	/* tx buffer addrress - Low 	*/
	outw(iobase, 0x0);	/* tx buffer addrress - High 	*/

#ifdef CU_START_DEBUG
	{
	    unsigned short w, i;
	
	    kprintf("tbd=%x\n", tbd);
	    outw(iobase+RDPTR, tbd);
	    for (i = 0; i < 4; i++) {
		w = inw(iobase);
		kprintf("[%d]=%x ", i, w);
	    }
	    kprintf("\n");
	}
#endif
	/* a NOP */
	outw(iobase+WRPTR, nop);
	outw(iobase, 0x0); 		/* status 	*/
	outw(iobase, CUCMD_NOP);	/* command 	*/
	outw(iobase, nop);		/* next, point to self */

#ifdef CU_START_DEBUG
	{
	    unsigned short w, i;
	
	    kprintf("nop=%x\n", nop);
	    outw(iobase+RDPTR, nop);
	    for (i = 0; i < 3; i++) {
		w = inw(iobase);
		kprintf("[%d]=%x ", i, w);
	    }
	    kprintf("\n");
	}
#endif
	/* a TX buffer */
	count++;
	offset += TX_BLOCK_SIZE
    }
#ifdef CU_START_DEBUG
    kprintf("TX blocks=%d\n", count);
#endif

    /*
     * start 82586 Command Unit
     * NOTE: don't execute any commnd after starting the CU
     */
    outw(iobase + SCB_COMMAND, SCBCMD_CUS);
    outw(iobase + SCB_STATUS, 0);
    outw(iobase + SCB_CBL, ped->ed_nop);
    outb(iobase + CAC, 0);	
    ee_wait(iobase);
    ee_ack(iobase);
}

/*-------------------------------------------------------------------------
 * ee_ru_start - init RX data structures and start RU
 *-------------------------------------------------------------------------
 */
int ee_ru_start(ped)
    struct etdev   *ped;    
{
    unsigned short iobase, offset;
    unsigned short buf_off;

    iobase = ped->ed_pdev->dvcsr;

    offset = ped->rx_head = RAM_RX_BEGIN;
    /*
     * allocate receive frame area:
     * receive frame descriptors, receive buffer descriptors, receive buffers
     */
    rfd = ped->ed_rfd = offset;
    offset += EE_NRX * sizeof(struct rfd);
    rbd = ped->ed_rbd = offset;
    offset += EE_NRX * sizeof(struct rbd);
    buf_off = offset;
    
#ifdef INIT_MEM_DEBUG
    kprintf("RX_RFD=%x (%x)\n", rfd, RAM_RFD);
    kprintf("RX_RBD=%x (%x)\n", rbd, RAM_RBD);
    kprintf("RX_RXBUF=%x (%x)\n", buf_off, RAM_RX_BUF);
#endif

    for (i = 1; i <= EE_NRX; i++) {
	/* frame descriptor */
	outw(iobase+WRPTR, rfd);
	outw(iobase, 0x0); 			/* status 	*/
	if (i == EE_NRX) {
	    outw(iobase, RFD_EL);		/* command 	*/
	    outw(iobase, ped->ed_rfd);		/* make it circular */
	}
	else {
	    outw(iobase, 0x0);			/* command 	*/
	    outw(iobase, rfd+sizeof(struct rfd));	/* next rfd 	*/
	}
	outw(iobase, rbd);			/* rbd		*/

	/* buffer descriptor */
	outw(iobase+WRPTR, rbd);
	outw(iobase, 0x0); 			/* status 	*/
	outw(iobase, 0xffff);			/* next rfd 	*/
	outw(iobase, buf_off);			/* buf addr- LO	*/
	outw(iobase, 0x0);			/* buf addr- HI	*/
	outw(iobase, (RBD_EL | EDLEN));		/* buf size	*/

	/* next one */
	rfd += sizeof(struct rfd);
	rbd += sizeof(struct rbd);
	buf_off += EDLEN;
    }

    ped->ed_rfdend = rfd - sizeof(struct rfd);
    ped->ed_rbdend = rbd - sizeof(struct rbd);
#ifdef INIT_MEM_DEBUG
    kprintf("ped->ed_rfdend=%x, ped->ed_rbdend=%x\n", ped->ed_rfdend,
	    ped->ed_rbdend);
#endif
    return buf_off;
}
