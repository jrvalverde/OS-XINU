/* getdmem.c - getdmem initdmamem */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <mark.h>
#include <mem.h>
#include <vmem.h>
#include <network.h>

/*#define DEBUG*/

long nextdma;
long dmaend;

MARKER dmamark;

/*------------------------------------------------------------------------
 * getdmem  --  allocate heap that can be used for DVMA transfers on the
 *              Sun 3 for device storage, returning lowest WORD address
 *------------------------------------------------------------------------
 */
SYSCALL getdmem(nbytes)
    long nbytes;
{
    long ret;
    
#ifdef DEBUG
    kprintf("getdmem(%ld) called\n",nbytes);
#endif    
    
    if (unmarked(dmamark)) {
        dmaend  = VM_DMA_BEGIN + VM_DMA_NBYTES - 1;
        nextdma = VM_DMA_BEGIN;
	mark(dmamark);
    }

#ifdef DEBUG
    kprintf("getdmem(next_dma= 0x%08x, dma_end= 0x%08x)\n", (unsigned)nextdma,
	    (unsigned)dmaend);
#endif
    
    /* round the request up to an even mblock boundary */
    nbytes = (long) roundmb(nbytes);

    if ((nextdma + nbytes) <= dmaend) {
	ret = nextdma;
	nextdma += nbytes;
	/* round up to next long address */
	nextdma = (long) (nextdma+3) & ~(unsigned long) 3;

#ifdef DEBUG
    kprintf("getdmem(%d) returns 0x%08x\n", nbytes, ret);
#endif    
	return(ret);
    }
    else {
	panic("No more memory for DMA buffers");
	return(SYSERR);
    }
}
