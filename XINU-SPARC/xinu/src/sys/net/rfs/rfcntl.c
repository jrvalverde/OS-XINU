/* rfcntl.c - rfcntl */

#include <conf.h>
#include <kernel.h>
#include <network.h>


/*#define DEBUG*/


/*------------------------------------------------------------------------
 *  rfcntl  --  control the remote file server access system
 *------------------------------------------------------------------------
 */
rfcntl(devptr, func, addr, addr2)
struct	devsw	*devptr;
int	func;
char	*addr;
char	*addr2;
{
	long	junk;
	int	len;

	junk = 0L;
	switch (func) {

	case RFCLEAR:
		/* clear port associated with rfserver */
		control(Rf.device, DG_CLEAR);
		return(OK);

	/* Universal file manipulation functions */

	case FLACCESS:
#ifdef DEBUG
		kprintf("rfcntl: calling FLACCESS with len = %d\n", addr2);
#endif
		return(rfmkpac(FS_ACCESS,addr,&junk,(char *)&junk,addr2));

	case FLREMOVE:
		return( rfmkpac(FS_UNLINK, addr, &junk, NULLSTR, 0) );

	case FLRENAME:
		len = strlen(addr2) + 1;
		return( rfmkpac(FS_RENAME, addr, &junk, addr2, len) );

	default:
		return(SYSERR);
	}	
}
