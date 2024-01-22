/* ee_cmd.c - ee_cmd, ee_wait, ee_ack */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <i386.h>
#include <ee.h>

/*#define DEBUG*/

/*------------------------------------------------------------------------
 * ee_cmd - issue an Intel 82586 command
 *------------------------------------------------------------------------
 */
int ee_cmd(ped, cmd, arg1, arg2, arg3)
struct etdev	*ped;
unsigned short	cmd;
unsigned char	*arg1, *arg2, *arg3;
{
    struct devsw	*pdev = ped->ed_pdev;
    unsigned short	iobase, *p, cbl_status, w;
    int i, ret;

    iobase = ped->ed_pdev->dvcsr;
    
    /* build command block */
    outw(iobase+WRPTR, ped->ed_cbl);
    outw(iobase, 0);			/* status */
    outw(iobase, cmd | CUCMD_EL);	/* command */
    outw(iobase, 0xffff);		/* next */
    
    switch (cmd & CUCMD_MASK) {
    case CUCMD_CONFIG:
	/* copy configuration data */
	p = (unsigned short *)arg1;
	for (i = 0; i < CONFIG_LEN; i++) {
#ifdef DEBUG
	    kprintf("[%x]", p[i]);
#endif
	    outw(iobase, p[i]);
	}
#ifdef DEBUG
	kprintf("\n");
#endif
	break;
	
    case CUCMD_IASET:
	for (i = 0; i < EP_ALEN; i++) {
#ifdef DEBUG	    
	    kprintf("[%x]", arg1[i]);
#endif
	    outb(iobase, arg1[i]);
	}
#ifdef DEBUG
	kprintf("\n");
#endif
	break;
	
    case CUCMD_NOP:
	break;

    case CUCMD_DUMP:
	outw(iobase, ped->ed_cbl + 6);
	break;

    case CUCMD_MCSET:
	kprintf("MCAST not supported yet\n");
	return SYSERR;
/*	pcbl->cbl_alen = EP_ALEN * (int)arg2;*/
/*	blkcopy(pcbl->cbl_mca, arg1, pcbl->cbl_alen);*/
    }
    
    outw(iobase+SCB_COMMAND, SCBCMD_CUS);
    outw(iobase+SCB_STATUS, 0);
    outw(iobase+SCB_CBL, ped->ed_cbl);
    outb(iobase+CAC, 0);	
    
    for (i = 0; i < 640000; i++) {
	outw(iobase+RDPTR, ped->ed_cbl);
	cbl_status = inw(iobase);
	
	if (cbl_status & CUCMD_COMPLETE) {
	    if (!(cbl_status & CUCMD_OK)) {
		kprintf("ee: command %d failed, CBL status %x\n",
			cmd, cbl_status);
		ret = SYSERR;
	    }
	    else {
#ifdef DEBUG
		kprintf("command %d OK\n", cmd);
#endif
		if (cmd == CUCMD_DUMP) {
		    outw(iobase+RDPTR, ped->ed_cbl+6);	/* dump duffer */
#ifdef DEBUG
		    kprintf("Dump buffer....\n");
#endif
		    for (i = 0; i < (DUMP_SIZE >> 1); i++) {
			w = inw(iobase);
#ifdef DEBUG
			kprintf("[%d,%x]", i, w);
#endif
		    }
		}
		ret = OK;
	    }

	    ee_ack(iobase);
	    return ret;
	}
    }
    
    outw(iobase+RDPTR, ped->ed_cbl);
    cbl_status = inw(iobase);
    kprintf("ee: command timed out, cmd=%x, CBL status+%x\n", cmd, cbl_status);
    return SYSERR;
}

/*------------------------------------------------------------------------
 * ee_wait - wait for 82586 to become ready
 *------------------------------------------------------------------------
 */
ee_wait(iobase)
    unsigned short iobase;
{
    unsigned short scb_command, scb_status;
    int	     i;
	
    for (i=0; i<640000; ++i) {
	scb_command = inw(iobase+SCB_COMMAND);
	if (scb_command == 0)
	    return;
    }
    scb_status = inw(iobase+SCB_STATUS);
    kprintf("ee: ee_wait timed out, SCB status %x\n", scb_status);
}

/*------------------------------------------------------------------------
 * ee_ack - acknowledge status return by Intel 82586
 *------------------------------------------------------------------------
 */
ee_ack(iobase)
unsigned short iobase;
{
    unsigned short scb_status, data;

    scb_status = inw(iobase+SCB_STATUS);
    data = scb_status & 0xf000;
    if (data == 0) {
	return;
    }
    outw(iobase+SCB_COMMAND, data);
    outb(iobase+CAC, 0);
    ee_wait(iobase);
}
