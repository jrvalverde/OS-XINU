/* getstk.c - getstk */

#include <conf.h>
#include <kernel.h>
#include <mem.h>

/*------------------------------------------------------------------------
 * getstk  --  allocate stack memory, returning address of topmost int
 *------------------------------------------------------------------------
 */
SYSCALL	*getstk(nbytes)
	unsigned  int nbytes;
{
	PStype	ps;
	struct	mblock	*p, *q;	/* q follows p along memlist		*/
	struct	mblock	*fits, *fitsq;
	unsigned len;

	disable(ps);
	if (nbytes == 0) {
		restore(ps);
		return( (int *)SYSERR );
	}
	nbytes = (unsigned)roundew(nbytes);
	fits = (struct mblock *)NULL;
	q = &memlist;
	for (p = q->mnext ; (char *)p != NULL ; q = p,p = p->mnext)
		if ( p->mlen >= nbytes) {
			fitsq = q;
			fits = p;
		}
	if ((char *)fits == NULL) {
		restore(ps);
		return( (int *)SYSERR );
	}
	if (nbytes == (len = fits->mlen) ) {
		fitsq->mnext = fits->mnext;
	} else {
		fits->mlen -= nbytes;
	}
	fits = (struct mblock *)((unsigned)fits + len - sizeof(int));
	restore(ps);
	return( (int *)fits );
}
