/* freeheap.c - freeheap */

#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <vmem.h>
#include <proc.h>

/*#define DEBUG*/
/*#define MIN_DEBUG*/

/*------------------------------------------------------------------------
 *  freeheap  --  free heap storage in P0 region, returning it to vmemlist
 *		  Also release frames that were used by the freed heap space.
 *------------------------------------------------------------------------
 */
SYSCALL	freeheap(block, size)
	struct	mblock	*block;
	unsigned size;
{
    PStype	ps;			/* save process status reg	*/
    struct	mblock	*p, *q;		/* temp mblock ptrs		*/
    unsigned top;			/* top of previous free block	*/
    int pid;				/* process id			*/
    int asindx;				/* addr space array index	*/

#if defined(DEBUG) || defined(MIN_DEBUG)
    kprintf("freeheap: addr = 0x%08x, size = 0x%x\n", (unsigned) block, size);
#endif
    pid = getpid();
    asindx = asidindex(proctab[pidindex(pid)].asid);
    if (size==0 || (unsigned)block < (unsigned)vmaddrsp.minaddr)
	return(SYSERR);
    
    size = (unsigned)roundmb(size);
    
    disable(ps);
    for (p = vmemlist[asindx].mnext, q = &vmemlist[asindx];
	((char *)p != NULL && p < block); q=p,p=p->mnext)
	;

    if ((q != &vmemlist[asindx] &&
	 (top = q->mlen+(unsigned)q) > (unsigned)block) ||
	(char *)p != NULL && (size+(unsigned)block) > (unsigned)p ) {
	restore(ps);
	return(SYSERR);
    }

    if (q!= &vmemlist[asindx] && top == (unsigned)block) {
	q->mlen += size;
    }
    else {
	q->mnext = block;
	block->mlen = size;
	block->mnext = p;
	q = block;
    }
    if ((unsigned)( q->mlen + (unsigned)q ) == (unsigned)p) {
	q->mlen += p->mlen;
	q->mnext = p->mnext;
    }

    /* let the frame mager to release the frames */
    
    restore(ps);
    return(OK);
}
