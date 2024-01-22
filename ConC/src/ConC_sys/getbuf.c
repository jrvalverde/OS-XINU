/* getbuf.c - getbuf */

#include <kernel.h>
#include <bufpool.h>
#include <errno.h>

/*------------------------------------------------------------------------
 *  getbuf  --  get a buffer from a preestablished buffer pool
 *------------------------------------------------------------------------
 */
char	*getbuf(poolid)
int poolid;
{
	long	ps;
	int	*buf;

	if ( !bpmark ) {
		errno = CEBADBP;
		return(NULL);
		}

	if (poolid<0 || poolid>=nbpools) {
		errno = CEBADBP;
		return(NULL);
		}
	swait(bptab[poolid].bpsem);
	disable(ps);
	buf = bptab[poolid].bpnext;
	bptab[poolid].bpnext = (int *)*buf;
	restore(ps);
	*buf++ = poolid;

	return((char *) buf);
}
