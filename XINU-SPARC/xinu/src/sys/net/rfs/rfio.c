/* rfio.c - rfio */

#include <conf.h>
#include <kernel.h>
#include <network.h>

#define PRINTERRORS

/*------------------------------------------------------------------------
 *  rfio  --  perform input or output using remote file server
 *------------------------------------------------------------------------
 */
rfio(devptr, rop, buff, len)
struct	devsw	*devptr;
int	rop;
char	*buff;
int	len;
{
	struct	rfblk	*rfptr;
	int	retcode;

	rfptr = (struct rfblk *)devptr->dvioblk;
	wait(rfptr->rf_mutex);
	if (len < 0 || rfptr->rf_state == RFREE) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("rfio: dev number = %d\n", devptr->dvnum);
		if (len < 0)
		    kprintf("rfio: returning SYSERR, len = %d\n", len);
		else
		    kprintf("rfio: returning SYSERR, rfptr is RFFREE\n");
#endif
		signal(rfptr->rf_mutex);
		return(SYSERR);
	}
	retcode = rfmkpac(rop, rfptr->rf_name, &rfptr->rf_pos, buff, len);
	signal(rfptr->rf_mutex);
	return(retcode);
}
