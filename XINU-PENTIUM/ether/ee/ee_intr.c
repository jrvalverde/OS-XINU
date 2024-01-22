/* ee_intr.c */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <ee.h>

/*#define DEBUG*/

/*------------------------------------------------------------------------
 * ee_intr - handle an AirLAN device interrupt
 *------------------------------------------------------------------------
 */
int ee_intr()
{
    struct aldev        *ped;
    unsigned short      scb_status, iobase;

    ped = &ee[0];
    iobase = ped->ed_pdev->dvcsr;

    scb_status = EE_READ_SCB_STATUS(iobase);

#ifdef DEBUG
    printf("ee_intr: iobase=%x, scb_status=%X\n", iobase, scb_status);
#endif

    /*
     * ACK 82586
     */
    ee_ack(ped);

    /*
     * Check if CU completes a TX command.
     */
    if (scb_status & SCBSTAT_CX)
        ee_xintr(ped, iobase);

    /*
     * check if RU runs out resources or is suspended
     */
    if ((scb_status & SCBSTAT_RUNORS) || (scb_status & SCBSTAT_RUSUS)) {
        ee_ru_start(ped, iobase);
    }

    /*
     * check if CU left active state
     */
    if (scb_status & SCBSTAT_CNA) {
	ee_cu_init(ped, iobase);
        ee_cu_start(ped, iobase);
    }

    /*
     * ee_demux can cause a context switch, so must re-enable board
     * interrupt before calling it.
     */
    outb(iobase+SIRQ, ped->ed_irq);
    outb(iobase+SIRQ, ped->ed_irq | SIRQ_IEN);

    if (scb_status & SCBSTAT_FR)
        ee_demux(ped);

    /*
     * if packets waiting in the output queue, then transmit it
     */
    if (lenq(ped->ed_outq))
        ee_wstrt(ped);

#ifdef DEBUG
    kprintf("**ee_intr: exit\n");
#endif
}

/*------------------------------------------------------------------------
 * ee_xintr - handle a transmit interrupt
 *------------------------------------------------------------------------
 */
int ee_xintr(ped, iobase)
struct aldev    *ped;
u_short iobase;
{
    struct txcbl txcbl = ped->ed_txcbl;
    unsigned short txcbl_status;

    ped->ed_xpending = 0;

    outw(iobase+RDPTR, ped->ed_txcbl);
    txcbl_status = inw(iobase);

#ifdef DEBUG
    printf("ee_xintr, txcbl_status=%X\n", txcbl_status);
#endif


    if (ped->ed_mcset) {
        ped->ed_mcset = 0;
        kprintf("MCAST???\n");
    }
    else if (txcbl_status & CUCMD_COMPLETE) {
        if (!(txcbl_status & CUCMD_OK)) {
            nif[ped->ed_ifnum].ni_oerrors++;
            return SYSERR;
        }
    }

    return OK;
}
