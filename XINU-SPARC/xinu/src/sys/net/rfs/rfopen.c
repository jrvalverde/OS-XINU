/* rfopen.c - rfopen */

#include <conf.h>
#include <kernel.h>
#include <network.h>


#define DEBUG
#define PRINTERRORS


/*------------------------------------------------------------------------
 *  rfopen  --  open a remote file
 *------------------------------------------------------------------------
 */
rfopen(devptr, name, mode)
struct	devsw	*devptr;
char	*name;
char	*mode;
{
	STATWORD ps;    
	struct	rfblk	*rfptr;
	int	i;
	int	mbits;
	int	devnum;

	disable(ps);
	if (strlen(name) > RNAMLEN || (mbits=ckmode(mode)) == SYSERR
	    || (i=rfalloc()) == SYSERR) {
		restore(ps);
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("rfopen(devptr,'%s','%s') ==> SYSERR(1)\n",
			name,mode);
#endif
		return(SYSERR);
	}

	rfptr = &Rf.rftab[i];
	devnum = rfptr->rf_dnum;
	strcpy(rfptr->rf_name, name);
	rfptr->rf_mode = mbits;
	rfptr->rf_pos = 0L;

	/* send remote file open request */

	if ( rfio(&devtab[devnum], FS_OPEN, NULLSTR, mbits) == SYSERR ) {
		rfptr->rf_state = RFREE;
		restore(ps);
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("rfopen(devptr,'%s','%s') ==> SYSERR(2)\n",
			name,mode);
#endif
		return(SYSERR);
	}
	restore(ps);

#ifdef DEBUG
	kprintf("rfopen(devptr,'%s','%s') ==> dev %d\n", name,mode,devnum);
#endif

	return(devnum);
}