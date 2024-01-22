/* ee_cmd.c - ee_cmd, ee_wait, ee_ack */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <i386.h>
#include <ee.h>

/*#define DEBUG*/
/*#define DEBUG*/

/*------------------------------------------------------------------------
 * ee_cmd - issue an Intel 82586 command
 *------------------------------------------------------------------------
 */
int ee_cmd(ped, cmd, arg1, arg2, arg3)
struct aldev    *ped;
unsigned short  cmd;
unsigned char   *arg1, *arg2, *arg3;
{
    struct devsw *pdev = ped->ed_pdev;
    struct cbl   cbl;
    struct scb   scb;
    unsigned short iobase, cbl_status;
    int i, ret;

    iobase = ped->ed_pdev->dvcsr;

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

    case CUCMD_NOP:
        break;

    case CUCMD_DUMP:
        cbl.cbl_dumpbuf_off = ped->ed_cbl + DUMP_OFFSET;
        break;

    case CUCMD_MCSET:
/*      pcbl->cbl_alen = EP_ALEN * (int)arg2;*/
/*      blkcopy(pcbl->cbl_mca, arg1, pcbl->cbl_alen);*/
        kprintf("MCAST???\n");
        break;
    }

    ee_ram_blk_write(iobase, ped->ed_cbl, &cbl, sizeof(cbl));

    scb.scb_cmd = SCBCMD_CUS;
    scb.scb_status = 0;
    scb.scb_cbloff = ped->ed_cbl;
    ee_ram_blk_write(iobase, EE_SCB_OFFSET, &scb, sizeof(scb));

    EE_CA(iobase);

    for (i = 0; i < 640000; i++) {
        outw(iobase+RDPTR, ped->ed_cbl);
        cbl_status = inw(iobase);

        if (cbl_status & CUCMD_COMPLETE) {
            if (!(cbl_status & CUCMD_OK)) {
                kprintf("ee%d: action command %d failed, status %x\n",
                        pdev->dvminor, cmd, cbl_status);
                ret = SYSERR;
            }
            else {
#ifdef DEBUG
                kprintf("command %d OK\n", cmd);
#endif
                if (cmd == CUCMD_DUMP) {
                    unsigned short *w;

                    kprintf("Dump buffer....\n");
                    ee_ram_blk_read(iobase, ped->ed_cbl, &cbl, sizeof(cbl));
                    w = (unsigned short *)cbl.cbl_dumpbuf;
                    for (i = 0; i < (DUMP_SIZE >> 1); i++) {
                        kprintf("[%d,%x]", i, w[i]);
                    }
                }
                ret = OK;
            }

            ee_ack(ped);
            return ret;
        }
    }
    kprintf("ee%d: action command timed out, CBL status %x\n",
            pdev->dvminor, cbl_status);
}

/*------------------------------------------------------------------------
 * ee_wait - wait for 82586 to become ready
 *------------------------------------------------------------------------
 */
ee_wait(ped)
struct aldev    *ped;
{
    unsigned short iobase, scb_command, scb_status;
    int      i;

    iobase = ped->ed_pdev->dvcsr;

    for (i=0; i<640000; ++i) {
	scb_command = inw(iobase+EE_SCB_COMMAND);
        if (scb_command == 0)
            return;
    }
    scb_status = EE_READ_SCB_STATUS(iobase);
    kprintf("ee: ee_wait timed out, SCB status %x\n", scb_status);
}

/*------------------------------------------------------------------------
 * ee_ack - acknowledge status return by Intel 82586
 *------------------------------------------------------------------------
 */
ee_ack(ped)
struct aldev    *ped;
{

    unsigned short iobase, data, scb_status;

    iobase = ped->ed_pdev->dvcsr;

    scb_status = EE_READ_SCB_STATUS(iobase);
    data = scb_status & 0xf000;
    if (data == 0) {
        return;
    }
    
    outw(iobase+EE_SCB_COMMAND, data);
    
    EE_CA(iobase);

    ee_wait(ped);
}
