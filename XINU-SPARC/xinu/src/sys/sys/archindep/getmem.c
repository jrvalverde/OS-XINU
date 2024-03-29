/* getmem.c - getmem */

#include <conf.h>
#include <kernel.h>
#include <mem.h>

/*#define DEBUG*/

/*------------------------------------------------------------------------
 * getmem  --  allocate heap storage, returning lowest WORD address
 *------------------------------------------------------------------------
 */
WORD *getmem(nbytes)
     unsigned nbytes;
{
    STATWORD ps;    
    struct	mblock	*p, *q, *leftover;
    
    disable(ps);
#ifdef DEBUG
    kprintf("getmem(%d) called\n", nbytes);
#endif
    
    if (nbytes==0 || memlist.mnext== (struct mblock *) NULL) {
	restore(ps);
	return((WORD *)SYSERR);
    }
    nbytes = (unsigned int) roundmb(nbytes);
    for (q= &memlist,p=memlist.mnext; p != (struct mblock *) NULL;
	 q=p,p=p->mnext)
	if (p->mlen == nbytes) {
	    q->mnext = p->mnext;
	    restore(ps);
	    return( (WORD *)p );
	} else if (p->mlen > nbytes) {
	    leftover = (struct mblock *)((unsigned)p + nbytes);
	    q->mnext = leftover;
	    leftover->mnext = p->mnext;
	    leftover->mlen = p->mlen - nbytes;
	    restore(ps);
	    return((WORD *)p);
	}
    restore(ps);
    kprintf("getmem: Kernel ran out of memory!\n");
    kprintf("getmem: We will try to continue, but make no promises\n");
    return((WORD *)SYSERR);
}
