/* getppte.c -- getppte, nextppte, prevppte */

/*#define DEBUG*/
#define PRINTERRORS

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <vmem.h>
#include <q.h>
/*-------------------------------------------------------------------------
 * getppte - get pointer to pte for later reference
 *	     MACHINE DEPENDENT(This is for SPARC)
 *-------------------------------------------------------------------------
 */
getppte(pid, vaddr, ppteptr)
     int pid;
     unsigned long vaddr;
     struct ppte *ppteptr;
{
    PStype ps;
    int seg;
    int pmeg;
    int pagenum;
    struct savepmeg *ppmeg;
    int asid;
    int i,j;
    
    if (isbadpid(pid) && isbadasid(pid)) 
	return(SYSERR);

    disable(ps);
    seg = VMSegOf(vaddr);
    pagenum = VMPageOf(vaddr);
    
#ifdef DEBUG
    kprintf("just entered getppte, pid=%d, ", pid);
    kprintf("vaddr=0x%08x ", vaddr);
    kprintf("segment = 0x%08x, page = 0x%08x\n", seg, pagenum);
#endif

    /* addr in sys region	*/
    if (vaddr >= (unsigned)vmaddrsp.xinu) {	
	/* assume everything up here has a pmeg	permanently allocated.	*/
	ppteptr->inmmu = TRUE;
	ppteptr->loc.mmuindex.pmeg = GetSegMap(vaddr);
	ppteptr->loc.mmuindex.ptenum = pagenum;
	if (seg == VMSegOf(VM_COPY_BEGIN)) 
	    ppteptr->loc.pte = (union pte *) &(copypmeg.savpte[pagenum]);
	else			/* assume in normal kernel area */
	    ppteptr->loc.pte =(union pte *) &(sysredpmeg[seg].savpte[pagenum]);
	restore(ps);
	return(OK);
    }

    /* is addr space id		*/
    if (isasid(pid)) {		
	if (vaddr > (unsigned)vmaddrsp.maxheap) {
	    kprintf("getppte: asid=%d currpid=%d\n",pid,currpid);
	    kprintf("getppte: asid addr = 0x%08x out of range\n",
		    (unsigned)vaddr);
	    restore(ps);
	    return(SYSERR);
	}
	ppmeg = &(addrpmegs[asidindex(pid)].redpmeg[seg]);
    }
    else if (ispid(pid)) {		/* is process id		*/
	if (vaddr <= (unsigned)vmaddrsp.maxheap) { /* use asid */
	    asid = proctab[pidindex(pid)].asid;
	    ppmeg = &(addrpmegs[asidindex(asid)].redpmeg[seg]);
	}
	else if (vaddr >= (unsigned)vmaddrsp.minstk) {
	    ppmeg = &(procpmegs[pidindex(pid)].redpmeg[procsegindex(vaddr)]);
#ifdef DEBUG
	    kprintf("getppte: ppmeg = 0x%08x\n", ppmeg);
	    kprintf("getppte: ppmeg->pmeg = %d\n", ppmeg->pmeg);
#endif
	}
	else {
	    kprintf("getppte: address out of range 0x%08x\n", vaddr);
	    restore(ps);
	    return(SYSERR);
	}
    }
    else {
	kprintf("   don't know what asid type %d is\n",pid);
	restore(ps);
	return(SYSERR);
    }
    
    if (ppmeg->pmeg == VM_PMEG_INVALID) {	/* pte not in mmu */
	/* try letting pmegs page fault their way in */
	ppteptr->inmmu = FALSE;
    }
    else {			/* page table entry is in mmu */
	ppteptr->inmmu = TRUE;
    }
    ppteptr->loc.pte = (union pte *) &(ppmeg->savpte[pagenum]);
    ppteptr->loc.mmuindex.pmeg = ppmeg->pmeg;
    ppteptr->loc.mmuindex.ptenum = pagenum;
#ifdef DEBUG
    kprintf("getppte: pte %d in pmeg %d assigned to vaddr 0x%x\n",
	    ppteptr->loc.mmuindex.ptenum,
	    ppteptr->loc.mmuindex.pmeg, vaddr);
    kprintf("getppte: returning\n");
#endif
    restore(ps);
    return(OK);
}


/*
 * ====================================================================
 * nextppte - return next address
 * ====================================================================
 */
nextppte(pid, pvirtaddr, ppteptr)
     int pid;
     long  *pvirtaddr;
     struct ppte *ppteptr;
{
    *pvirtaddr += PGSIZ;
    getppte(pid, *pvirtaddr, ppteptr);
}


/*
 * ====================================================================
 * prevppte - return prev address
 * ====================================================================
 */
prevppte(pid, pvirtaddr, ppteptr)
     int pid;
     long  *pvirtaddr;
     struct ppte *ppteptr;
{
    *pvirtaddr -= PGSIZ;
    getppte(pid,*pvirtaddr, ppteptr);
}
