#include <./mon/monnetwork.h>
#include <./mon/monitor.h>
#include <./mon/monee.h>

/*#define DEBUG*/

/*------------------------------------------------------------------------
 * mon_ee_intr - handle an AirLAN device interrupt
 *------------------------------------------------------------------------
 */
int mon_ee_intr()
{
    STATWORD		ps;
    struct aldev        *ped;
    unsigned short      scb_status, iobase;

    disable(ps);

    ped = &mon_ee[0];
    iobase = EE_IOBASE;

    scb_status = EE_READ_SCB_STATUS(iobase);

#ifdef DEBUG
    kprintf("mon_ee_intr: scb_status=%X\n", scb_status);
#endif

    /*
     * ACK 82586
     */
    mon_ee_ack(ped);

    /*
     * Check if CU completes a TX command.
     */
    if (scb_status & SCBSTAT_CX)
        mon_ee_xintr(ped, iobase);

    /*
     * check if RU runs out resources or is suspended
     */
    if ((scb_status & SCBSTAT_RUNORS) || (scb_status & SCBSTAT_RUSUS)) {
        mon_ee_ru_start(ped, iobase);
    }

    /*
     * check if CU left active state
     */
    if (scb_status & SCBSTAT_CNA) {
	mon_ee_cu_init(ped, iobase);
        mon_ee_cu_start(ped, iobase);
    }

    /*
     * mon_ee_demux can cause a context switch, so must re-enable board
     * interrupt before calling it.
     */
    outb(iobase+SIRQ, ped->ed_irq);
    outb(iobase+SIRQ, ped->ed_irq | SIRQ_IEN);

    if (scb_status & SCBSTAT_FR)
        mon_ee_demux(ped);

#ifdef DEBUG
    kprintf("**mon_ee_intr: exit\n");
#endif
    restore(ps);
}

/*------------------------------------------------------------------------
 * mon_ee_xintr - handle a transmit interrupt
 *------------------------------------------------------------------------
 */
int mon_ee_xintr(ped, iobase)
struct aldev    *ped;
u_short iobase;
{
    struct txcbl txcbl = ped->ed_txcbl;
    unsigned short txcbl_status;

    ped->ed_xpending = 0;

    outw(iobase+RDPTR, ped->ed_txcbl);
    txcbl_status = inw(iobase);

#ifdef DEBUG
    kprintf("mon_ee_xintr, txcbl_status=%X\n", txcbl_status);
#endif

    if (txcbl_status & CUCMD_COMPLETE) {
        if (!(txcbl_status & CUCMD_OK)) {
	    kprintf("ee: Tx error\n");
        }
    }

    /*
     * if packets waiting in the output queue, then transmit it
     */
    if (mon_lenq(ped->ed_outq))
        mon_ee_wstrt(ped);
}
