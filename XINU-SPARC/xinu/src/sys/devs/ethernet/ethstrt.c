/* ethstrt.c - ethstrt */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <lereg.h>
#include <idprom.h>
#include <vmem.h>

/*#define DEBUG*/

int ethinter();


/*------------------------------------------------------------------------
 *  ethstrt - start the LANCE ethernet chip
 *------------------------------------------------------------------------
 */
ethstrt(etptr)
	struct	etblk	*etptr;
{
    struct	le_device	*le;
    struct	devsw		*devptr;
    short	status;
    int		w;

    le = etptr->etle;
    devptr = etptr->etdev;
#ifdef DEBUG
    kprintf("ethstrt: le=0x%08x le->le_rap=0x%08x\n", le, &(le->le_rap));
#endif
    
    /* freeze the chip */
    le->le_rap = LE_CSR0;
    le->le_rdp = LE_STOP;

#ifdef DEBUG
    kprintf("ethstrt: made it past freeze the chip\n");
#endif
    /* set the interrupt vector */
    set_evec(devptr->dvivec, ethinter);

    /* hand the init block to the chip */
    le->le_rap = LE_CSR1;
#ifdef DEBUG    
    kprintf("le_rdp[1] = 0x%04x\n",low16(LANCE_ADDR(etptr->etib));
#endif	    
    le->le_rdp = low16(LANCE_ADDR(etptr->etib));
    
    le->le_rap = LE_CSR2;
#ifdef DEBUG    
    kprintf("le_rdp[2] = 0x%04x\n",hi8(LANCE_ADDR(etptr->etib));
#endif	    
    le->le_rdp = hi8(LANCE_ADDR(etptr->etib));
    
    le->le_rap = LE_CSR3;
    le->le_rdp = LE_BSWP | LE_ACON | LE_BCON;
	    
    /* initialize the chip */
    le->le_rap = LE_CSR0;
    le->le_rdp = LE_INIT;

    /* wait for init to complete */
    le->le_rap = LE_CSR0;
    status = le->le_rdp;
    for (w=10000; (w >= 0) && (!(status & LE_IDON)); --w) {
	if (w==0) {
	    panic("Unable to initialize Ethernet chip");
	}
	status = le->le_rdp;
    }
    
    /* reset the interrupt */
    le->le_rap = LE_CSR0;
    le->le_csr = LE_IDON;

    /* start the chip */
    le->le_csr = LE_STRT | LE_INEA;

    return(OK);
}

