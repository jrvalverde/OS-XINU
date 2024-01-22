#include <./mon/monnetwork.h>
#include <./mon/monitor.h>
#include <./mon/monee.h>

/*#define DEBUG*/
/*#define DEBUG*/

/*------------------------------------------------------------------------
 * mon_ee_cmd - issue an Intel 82586 command
 *------------------------------------------------------------------------
 */
int mon_ee_cmd(ped, cmd, arg1, arg2, arg3)
struct aldev    *ped;
unsigned short  cmd;
unsigned char   *arg1, *arg2, *arg3;
{
    struct cbl   cbl;
    struct scb   scb;
    unsigned short iobase, cbl_status;
    int i, ret;

    iobase = EE_IOBASE;

    /* build command block */
    cbl.cbl_status = 0;
    cbl.cbl_cmd = cmd | CUCMD_EL;
    cbl.cbl_next = 0xffff;

    switch (cmd & CUCMD_MASK) {
    case CUCMD_CONFIG:
        blkcopy(cbl.cbl_cfg, arg1, CONFIG_LEN);
        break;

    case CUCMD_IASET:
        blkcopy(cbl.cbl_addr, arg1, EP_ALEN);
        break;
	
    default:
	break;
    }

    mon_ee_ram_blk_write(iobase, ped->ed_cbl, &cbl, sizeof(cbl));

    scb.scb_cmd = SCBCMD_CUS;
    scb.scb_status = 0;
    scb.scb_cbloff = ped->ed_cbl;
    mon_ee_ram_blk_write(iobase, EE_SCB_OFFSET, &scb, sizeof(scb));

    EE_CA(iobase);

    for (i = 0; i < 640000; i++) {
        outw(iobase+RDPTR, ped->ed_cbl);
        cbl_status = inw(iobase);

        if (cbl_status & CUCMD_COMPLETE) {
            if (!(cbl_status & CUCMD_OK)) {
                kprintf("ee: action command %d failed, status %x\n", cmd,
			cbl_status);
                ret = SYSERR;
            }
            else {
#ifdef DEBUG
                kprintf("command %d OK\n", cmd);
#endif
                ret = OK;
            }

            mon_ee_ack(ped);
            return ret;
        }
    }
    kprintf("ee: action command timed out, CBL status %x\n", cbl_status);
}

/*------------------------------------------------------------------------
 * mon_ee_wait - wait for 82586 to become ready
 *------------------------------------------------------------------------
 */
mon_ee_wait(ped)
struct aldev    *ped;
{
    unsigned short iobase, scb_command, scb_status;
    int      i;

    iobase = EE_IOBASE;

    for (i=0; i<640000; ++i) {
	scb_command = inw(iobase+EE_SCB_COMMAND);
        if (scb_command == 0)
            return;
    }
    scb_status = EE_READ_SCB_STATUS(iobase);
    kprintf("ee: mon_ee_wait timed out, SCB status %x\n", scb_status);
}

/*------------------------------------------------------------------------
 * mon_ee_ack - acknowledge status return by Intel 82586
 *------------------------------------------------------------------------
 */
mon_ee_ack(ped)
struct aldev    *ped;
{
    unsigned short iobase, data, scb_status;

    iobase = EE_IOBASE;

    scb_status = EE_READ_SCB_STATUS(iobase);
    data = scb_status & 0xf000;
    if (data == 0) {
        return;
    }
    
    outw(iobase+EE_SCB_COMMAND, data);
    
    EE_CA(iobase);

    mon_ee_wait(ped);
}
