/* al_cntl.c */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <i386.h>
#include <ee.h>

/*------------------------------------------------------------------------
 * ee_cntl - handle AirLAN control() functions
 *------------------------------------------------------------------------
 */
int ee_cntl(pdev, func, arg)
struct devsw    *pdev;
int             func;
char            *arg;
{
    STATWORD    ps;
    struct      aldev   *ped = &ee[pdev->dvminor];
    u_short     iobase;
    int         rv;

    iobase = pdev->dvcsr;

    disable(ps);
    switch (func) {
    case EPC_PROMON:    /* turn on promiscuous mode */
	ped->ed_proms = 1;
	rv = SYSERR;
	/* more work here */
        break;

    case EPC_PROMOFF:   /* turn off promiscuous mode */
        ped->ed_proms = 0;
	rv = SYSERR;
	/* more work here */
        break;

    case EPC_MADD:
        rv = SYSERR;
/*      rv = ee_mcadd(ped, arg);*/
        break;
    case EPC_MDEL:
        rv = SYSERR;
/*      rv = ee_mcdel(ped, arg);*/
        break;
    default:
        rv = SYSERR;
    }

    restore(ps);
    if (rv == SYSERR)
        kprintf("ee_cntl: func=%d error\n", func);
    return rv;
}
