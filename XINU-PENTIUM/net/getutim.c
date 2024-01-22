/* getutim.c - getutim */

#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include <date.h>
#include <network.h>
#include <bootp.h>

/*------------------------------------------------------------------------
 * getutim  --  obtain time in seconds past Jan 1, 1970, ut (gmt)
 *------------------------------------------------------------------------
 */
SYSCALL	getutim(timvar)
long	*timvar;
{
    int	dev;
    int	len;
    int	ret;
    long	now;
    long	utnow;
    char	*msg = "No time server response\n";

    wait(clmutex);
    ret = OK;
    if (clktime < SECPERHR) {	/* assume small numbers invalid	*/
	if ((dev = open(UDP, TSERVER, ANYLPORT)) == SYSERR ||
	    control(dev, DG_SETMODE, DG_TMODE|DG_DMODE) == SYSERR) {
	    panic(msg);
	    ret = SYSERR;
	}
	if (write(dev, msg, 2) == SYSERR) {
	    kprintf("getutim: write error!");
	    signal(clmutex);
	    return (SYSERR);
	}
	if (read(dev, &utnow, 4) != 4) {
	    ret = SYSERR;
	} else {
	    clktime = net2xt( net2hl(utnow) );
	}
	close(dev);
    }
    *timvar = clktime;
    signal(clmutex);
    return(ret);
}
