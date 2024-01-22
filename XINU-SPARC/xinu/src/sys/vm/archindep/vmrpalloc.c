/* vmrpalloc.c - vmrpalloc, vmrpfree */

#include <conf.h>
#include <kernel.h>
#include <vmem.h>
#include <mark.h>

/*#define DEBUG*/
#define PRINTERRORS

struct vmres {
	Bool	inuse;
	char	*vaddr;
};

static struct vmres vmrestab[VM_RESPG_NPAGES];
static MARKER vmrpmark;
static int vmrpsem;

/* the page table entries for these pages */
union pte pte_vmrp[VM_RESPG_NPAGES];

/*
 *------------------------------------------------------------------------
 * vmrpalloc - allocate a virtual page in high memory, return its address
 *             Thees high memory addresses are for copying from one
 *	       address space to another.
 *------------------------------------------------------------------------
 */
char *vmrpalloc()
{
    int i;

    if (unmarked(vmrpmark)) {
#ifdef DEBUG
	kprintf("vmrpalloc: creating %d reserved virtual pages\n",
		VM_RESPG_NPAGES);
#endif 
	for (i = 0; i < VM_RESPG_NPAGES; ++i) {
	    vmrestab[i].inuse = FALSE;
	    vmrestab[i].vaddr = (char *) (VM_RESPG_BEGIN + i * PGSIZ);
#ifdef DEBUG
	    kprintf("vmrpalloc: page %2d = 0x%08x\n", i, vmrestab[i].vaddr);
#endif
	}

	vmrpsem = screate(1);
	mark(vmrpmark);
    }

    wait(vmrpsem);

    for (i = 0; i < VM_RESPG_NPAGES; ++i) {
	if (!vmrestab[i].inuse) {
	    vmrestab[i].inuse = TRUE;
#ifdef DEBUG
	    kprintf("vmrpalloc: returning virtual page %d = 0x%08x\n",
		    i, vmrestab[i].vaddr);
#endif DEBUG
	    signal(vmrpsem);
	    return(vmrestab[i].vaddr);
	}
    }

    signal(vmrpsem);
#if defined(DEBUG) || defined(PRINTERRORS)
    kprintf("vmrpalloc ==> SYSERR, no available reserved virtual pages\n");
#endif
    panic("vmrpalloc: no more virtual pages to allocate");
    return((char *) SYSERR);
}


/*
 *------------------------------------------------------------------------
 * vmrpfree - deallocate a virtual page in high memory
 *------------------------------------------------------------------------
 */
vmrpfree(vaddr)
     char *vaddr;
{
    int pgnum;
    int ix;

    wait(vmrpsem);
    pgnum = topgnum(vaddr);
    ix = pgnum - topgnum(VM_RESPG_BEGIN);
    if ((ix < 0) || (ix >= VM_RESPG_NPAGES)) {
	kprintf("vmrpfree(0x%08x) ==> SYSERR, bad address (ix:%d)\n",
		vaddr,ix);
	panic("vmrpfree(0x%08x): bad virtual page address", vaddr);
	return(SYSERR);
    }

    if (!vmrestab[ix].inuse || (vmrestab[ix].vaddr != vaddr)) {
	signal(vmrpsem);
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("vmrpfree(0x%08x) ==> SYSERR, not an allocated page\n", vaddr);
#endif
	panic("vmrpfree(0x%08x): internal error", vaddr);
	return(SYSERR);
    }

#ifdef DEBUG
    kprintf("vmrpfree(0x%x) freed page\n", vaddr);
#endif
    vmrestab[ix].inuse = FALSE;
    signal(vmrpsem);
    return(OK);
}
