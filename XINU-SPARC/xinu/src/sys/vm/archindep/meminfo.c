/* meminfo.c - various memory query routines */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <mem.h>
#include <vmem.h>
#include <frame.h>


#define DEBUG


/*------------------------------------------------------------------------
 * memusage - return number of accessed and in-memory pages on an address
 *            range
 *------------------------------------------------------------------------
 */
SYSCALL	memusage(pid,lowaddr,highaddr,accessed,inmem)
     int pid;
     unsigned int lowaddr;
     unsigned int highaddr;
     unsigned int *accessed;
     unsigned int *inmem;
{
    struct ppte ppte;
    struct ppte *ppteptr;
    unsigned int addr;

    if (!ispid(pid))
	return(SYSERR);

    ppteptr = &ppte;
    *accessed = 0;
    *inmem = 0;

    /* find the status of all pages in the range */
    for (addr = truncpg(lowaddr); addr <= highaddr; addr += PGSIZ) {
	if ((getppte(pid,addr,ppteptr) != OK) || (!gettvalid(ppteptr))) {
	    continue;
	}

	/* else it's a potentially valid page */
	if (getinmem(ppteptr)) {
	    /* it's somewhere in memory */
	    ++*accessed;
	    if (gethvalid(ppteptr)) {
		/* it's really there */
		++*inmem;
	    } else {
		/* it's on some reclaim list */
		;			/* we aren't interested... */
	    }
	} else {
	    /* it's NOT in memory */
	    /* we aren't interested in zero on demand */
	    if (getzero(ppteptr)) {
		;	
	    } else {
		/* paged out */
		++*accessed;
	    }
	}
    }
    return(OK);
}


/*------------------------------------------------------------------------
 * pageused - is there a page in use at the given address for the given
 *            process?
 *------------------------------------------------------------------------
 */
SYSCALL	pageused(id,addr)
     int id;
     unsigned int addr;
{
    struct ppte ppte;
    struct ppte *ppteptr;

    ppteptr = &ppte;

    if (!ispid(id) && !isasid(id))
	return(SYSERR);

    if (ispid(id) && (addr < (unsigned int) vmaddrsp.maxheap)) {
	/* REALLY in the address space */
	id = proctab[id].asid;
    }

    if ((getppte(id, addr, ppteptr) != OK) || (!gettvalid(ppteptr)))
	return(FALSE);

    if (getinmem(ppteptr) || getpfn(ppteptr) >= ftinfo.framebase)
	/* in memory or out on disk */
	return(TRUE);
    return(FALSE);
}





/*------------------------------------------------------------------------
 * getumext - return the lowest and highest extent of a user process's
 *	      memory (in the text,data,bss,heap range)
 *------------------------------------------------------------------------
 */
SYSCALL	getumext(id,plowest,phighest)
     int id;
     unsigned int *plowest;
     unsigned int *phighest;
{
    unsigned addr;

    if (!ispid(id) || (iskernproc(id)))
	return(SYSERR);

    *plowest = 0xffffffff;
    *phighest = 0;

    for (addr = 0; addr < (unsigned) vmaddrsp.maxheap; addr += PGSIZ) {
	if (pageused(id,addr)) {
	    if (addr < *plowest)
		*plowest = addr;
	    if (addr > *phighest)
		*phighest = addr - 1;
	}
    }

    return(OK);
}








