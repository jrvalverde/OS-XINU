/* freemem.c - freemem */

#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <vmem.h>

/*#define DEBUG*/
/*#define PRINTERRORS*/

/*------------------------------------------------------------------------
 *  freemem  --  free a memory block, returning it to memlist
 *------------------------------------------------------------------------
 */
SYSCALL	freemem(block, size)
     struct	mblock	*block;
     unsigned size;
{
    STATWORD ps;    
    struct	mblock	*p, *q;
    unsigned top;
    
    if (size==0
	|| (unsigned)block   > (unsigned)vmaddrsp.xheapmax
	|| ((unsigned)block) < (unsigned)vmaddrsp.xheapmin) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("freemem(0x%x,%d) == SYSERR (reason 1)\n",
		block,size);
#endif
	return(SYSERR);
    }
    size = (unsigned)roundmb(size);
#ifdef DEBUG
    kprintf("freemem(0x%x,%d) called\n", block,size);
#endif
    disable(ps);
    for( p=memlist.mnext,q= &memlist;
	p != (struct mblock *) NULL && p < block ;
	q=p,p=p->mnext )
	;
    if ((top=q->mlen+(unsigned)q)>(unsigned)block && q!= &memlist ||
	p != (struct mblock *)NULL && (size + (unsigned)block) > (unsigned)p) {
	restore(ps);
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("freemem(0x%x,%d) == SYSERR (reason 2)\n",
		block,size);
#endif
	return(SYSERR);
    }
    if ( q!= &memlist && top == (unsigned)block )
	q->mlen += size;
    else {
	block->mlen = size;
	block->mnext = p;
	q->mnext = block;
	q = block;
    }
    if ( (unsigned)( q->mlen + (unsigned)q ) == (unsigned)p) {
	q->mlen += p->mlen;
	q->mnext = p->mnext;
    }
    restore(ps);
    return(OK);
}
