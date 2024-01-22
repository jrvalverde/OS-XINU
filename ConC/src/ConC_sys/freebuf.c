/* freebuf.c - freebuf */

#include <kernel.h>
#include <bufpool.h>
#include <errno.h>

/*------------------------------------------------------------------------
 *  freebuf  --  free a buffer that was allocated from a pool by getbuf
 *------------------------------------------------------------------------
 */
SYSCALL	freebuf(buf)
int *buf;
{
	long	ps;
	int	poolid;

	if ( !bpmark ) {
		errno = CEBADBP;
		return(SYSERR);
		}

	poolid = *(--buf);
	if (poolid<0 || poolid>=nbpools) {
		errno = CEBADBP;
		return(SYSERR);
		}
	disable(ps);
	*buf = (int)bptab[poolid].bpnext;
	bptab[poolid].bpnext = buf;
	restore(ps);
	ssignal(bptab[poolid].bpsem);

	return(OK);
}
