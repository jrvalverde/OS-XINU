/* setnettime.c - setnettime */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <sleep.h>
#include <date.h>

#define GUESSTIME

extern unsigned long clktime;

/*------------------------------------------------------------------------
 * setnettime  --  use net to obtain time in seconds past Jan 1, 1970, ut (gmt)
 *	N.B. assumes that clmutex is held
 *------------------------------------------------------------------------
 */
int	setnettime()
{
	int	dev;
	int	ret;
	long	utnow;
	int	status;
	char	*msgOPEN = "Can't open timeserver port\n";
	char	*msgTO   = "No time server response\n";

	ret = OK;

	if ((dev=open(UDP, TSERVER, ANYLPORT)) == SYSERR ||
	    control(dev,DG_SETMODE,DG_TMODE|DG_DMODE) == SYSERR) {
		panic(msgOPEN);
		ret = SYSERR;
	}
	write(dev, "xxxx", 2);		/* send junk packet to prompt */
#ifdef DEBUG
	kprintf("setnettime: waiting for time server response\n");
#endif
	if ((status = read(dev,&utnow,4)) != 4) {
#ifdef GUESSTIME
		if (status == TIMEOUT) {
			kprintf("\n\n\n");
			kprintf("==============================\n");
			kprintf("Timeserver is invalid\n");
			kprintf("please use a host which provides\n");
			kprintf("this service...\n");
			kprintf("(making up a time, for now)\n");
			kprintf("==============================\n");
			kprintf("\n\n");
			clktime = 1000000;
		}
#else
		if (status == TIMEOUT)
		    panic(msgTO);
		ret = SYSERR;
#endif
	} else
	    clktime = net2xt( net2hl(utnow) );
	close(dev);
	return(ret);
}
