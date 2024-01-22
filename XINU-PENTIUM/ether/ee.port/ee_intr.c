/* ee_intr.c - ee_intr */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <ee.h>

#define DEBUG

/*------------------------------------------------------------------------
 * ee_intr - handle an Intel EtherExpress16 device interrupt
 *------------------------------------------------------------------------
 */
int ee_intr()
{
    struct etdev	*ped;
    unsigned short	iobase, scb_status;
    
    ped = &ee[0];
    iobase = ped->ed_pdev->dvcsr;

    /* disable interrupt from 82586 */
/*    outb(iobase+SIRQ, ped->ed_irq);*/
    
    ee_wait(iobase);
    
    scb_status = inw(iobase+SCB_STATUS);
    
#ifdef DEBUG
    printf("ee_intr, scb_status=%X\n", scb_status);
#endif

    /*
     * ACK 82586
     */
    ee_ack(iobase);
        
    /*
     * Check if CU completes a TX command.
     */
    if (scb_status & SCBSTAT_CX) {
	if (ee_xintr(ped, iobase) == SYSERR) {
	    outb(iobase+SIRQ, ped->ed_irq);
	    outb(iobase+SIRQ, ped->ed_irq | SIRQ_IEN);
	    kprintf("** TX error\n");
	    return;
	}
    }

    if (!(scb_status & SCBSTAT_CUA)) {
	kprintf("** ee_intr: CU not active!\n");
	/*
	 * restart CU
	 */
	init_tx(ped, iobase, RAM_NOP1);
	/*
	 * start 82586 Command Unit
	 */
	outw(iobase + SCB_COMMAND, SCBCMD_CUS);
	outw(iobase + SCB_STATUS, 0);
	outw(iobase + SCB_CBL, ped->ed_nopcbl[0]);
	outb(iobase + CAC, 0);	
	ee_wait(iobase);
	ee_ack(iobase);
    }
    
    /*
     * ee_demux can cause a context switch, so must re-enable board
     * interrupt latch before calling it.
     */
    outb(iobase+SIRQ, ped->ed_irq);
    outb(iobase+SIRQ, ped->ed_irq | SIRQ_IEN);
    
    if (scb_status & SCBSTAT_FR)
	ee_demux(ped, iobase);

    /*
     * check if RU is ready
     */
    if (!(scb_status & SCBSTAT_RUR)) {
	if (scb_status & SCBSTAT_RNORS) {
	    ee_demux(ped, iobase);
	}
	kprintf("** ee_intr: RU not ready!\n");
	
	init_rx(ped, iobase, RAM_RFD);
	
	/* start 82586 Receive Unit */
	outw(iobase + SCB_COMMAND, SCBCMD_RUS);
	outw(iobase + SCB_STATUS, 0);
	outb(iobase + CAC, 0);
	ee_wait(iobase);
	ee_ack(iobase);
    }

    /*
     * if packets waiting in the output queue, then transmit it
     */
    if (lenq(ped->ed_outq))
	ee_wstrt(ped);
}

/*------------------------------------------------------------------------
 * ee_xintr - handle a transmit interrupt on an Intel EtherExpress
 *------------------------------------------------------------------------
 */
int ee_xintr(ped, iobase)
struct etdev	*ped;
unsigned short  iobase;
{
    unsigned short cbl_status;
    
    outw(iobase+RDPTR, ped->ed_txcbl);
    cbl_status = inw(iobase);
    
#ifdef DEBUG
    printf("ee_xintr, cbl status=%X\n", cbl_status);
#endif

    if (ped->ed_mcset) {
	ped->ed_mcset = 0;
	kprintf("MCAST???\n");
	ped->ed_xpending--;
    }
    else if (cbl_status & CUCMD_COMPLETE) {
	if (!(cbl_status & CUCMD_OK)) {
	    kprintf("ee: tranmission error, CB status %x\n", cbl_status);
	    nif[ped->ed_ifnum].ni_oerrors++;
	    init_82586(ped);
	    ped->ed_xpending = 0;
	    return SYSERR;
	}
	
	if (!ped->ed_xpending)
	    kprintf("stray transmit interrupt\n");
	else
	    ped->ed_xpending--;

	outw(iobase+WRPTR, ped->ed_txcbl);
	outw(iobase, 0);	/* cbl status */
    }
    
    return OK;
}


