/* mkpool.c - mkpool */

#include <kernel.h>
#include <bufpool.h>
#include <mem.h>
#include <errno.h>

/*------------------------------------------------------------------------
 *  mkpool  --  allocate memory for a buffer pool and link together
 *------------------------------------------------------------------------
 */
SYSCALL	mkpool(bufsiz, numbufs)
int	bufsiz, numbufs;
{
	long	ps;
	int	poolid;
	int	*where;

	if ( !bpmark )
		poolinit();

	disable(ps);
	if (bufsiz<BPMINB || bufsiz>BPMAXB ||
	    numbufs<1 || numbufs>BPMAXN ||
	    nbpools >= NBPOOLS ||
	    (int)(where=(int *)getmem((bufsiz+sizeof(int))*numbufs))==SYSERR) {
		errno = ((int)where == SYSERR) ? errno : EINVAL;
		restore(ps);
		return(SYSERR);
	}
	poolid = nbpools++;
	bptab[poolid].bpnext = where;
	bptab[poolid].bpsize = bufsiz;
	bptab[poolid].bpsem = screate(numbufs);
	bufsiz+=sizeof(int);
	for (numbufs-- ; numbufs>0 ; numbufs--,where+=bufsiz)
		*( (int *) where ) = (int)(where+bufsiz);
	*( (int *) where) = (int) NULL;
	restore(ps);

	return(poolid);
}
