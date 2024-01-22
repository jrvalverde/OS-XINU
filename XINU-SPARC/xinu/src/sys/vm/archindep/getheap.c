/* getheap.c - getheap */

/* 
 * getheap.c - allocates space in P0
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Dec  3 1987
 */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <mem.h>
#include <vmem.h>

/*#define DEBUG*/
/*#define MIN_DEBUG*/

/*------------------------------------------------------------------------
 * getheap  --  allocate heap storage in p0 space, returning lowest 
 *              integer address
 *------------------------------------------------------------------------
 */
SYSCALL	*getheap(nbytes)
	unsigned nbytes;
{
    PStype	ps;
    struct	mblock	*p, *q, *leftover;
    int pid, asid, asindx;
    int *i, j, k;
	
    /* Note: the way this routine is currently written we are	*/
    /*	assuming that all P0 pages are initially set to zero	*/
    /*	on demand and that they will page fault their way in	*/
    /*	when we access them to update the free list.  If they	*/
    /*	are not set to zero-on-demand, then things become a	*/
    /*	little trickier since we must make sure the page that	*/
    /*	'leftover' points to is valid.				*/

    pid = getpid();
    asid = proctab[pid].asid;
    asindx = asidindex(asid);
	
#ifdef DEBUG
    kprintf("In getheap, nbytes = 0x%x\n", nbytes);
#endif
    
    disable(ps);
    if (nbytes <= 0 || (char *)vmemlist[asindx].mnext == NULL) {
	restore(ps);
#ifdef DEBUG
	kprintf("getheap:==NULL, return SYSERR\n");
#endif
	return( (int *)SYSERR);
    }

    nbytes = (unsigned) roundmb(nbytes);

    for (q= &vmemlist[asindx],p=vmemlist[asindx].mnext;
	 (char *)p!=NULL ; q=p,p=p->mnext) {
#ifdef DEBUG
	kprintf("getheap: examining location 0x%08x\n", p);
	kprintf("getheap: its length is 0x%08x\n", p->mlen);
#endif
	if (p->mlen == nbytes) {
	    q->mnext = p->mnext;
	    restore(ps);
	    return( (int *)p );
	}
	else if ( p->mlen > nbytes ) {
	    leftover = (struct mblock *)( (unsigned)p + nbytes );
	    q->mnext = leftover;
	    leftover->mnext = p->mnext;
	    leftover->mlen = p->mlen - nbytes;
	    restore(ps);
	    return( (int *)p );
	}
    }
#ifdef DEBUG
    kprintf("getheap:didn't find, return(SYSERR)\n");
#endif
    restore(ps);
    return( (int *)SYSERR );
}
