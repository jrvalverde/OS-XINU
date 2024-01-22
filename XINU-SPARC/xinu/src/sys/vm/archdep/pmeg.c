/* pmeg.c - */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <vmem.h>


/*#define DEBUG_GETPMEG*/
/*#define DEBUG_RECLAIMPMEG*/
/*#define DEBUG_PMEG_INIT*/

/* Only define this if you are sure that you can take a page fault on a	*/
/* Kernel stack segment.  On the Sparc, we may never be able to turn 	*/
/* this on, since restore_context() causes an underflow trap that will	*/
/* try to access the stack.  If we steal the pmeg, then the underflow	*/
/* interrupt handler will cause a page fault.  So until you are	able	*/
/* to take a page fault in the window over/underflow interrupt handlers	*/
/* you should not set this variable.					*/
#define DONT_TAKE_KERNEL_STACK

struct pmege pmegtbl[VM_NUM_PMEGS + 4];

/*------------------------------------------------------------------------
 *  clearpmeg - clear a pmeg 
 *------------------------------------------------------------------------
 */
clearpmeg(ppmeg)
     struct pmege *ppmeg;
{
    ppmeg->stat  = VM_PMEG_FREE;
    ppmeg->pid   = SYSERR;
    ppmeg->vaddr = SYSERR;
}

/*------------------------------------------------------------------------
 *  to_pmg_list - add a pmeg onto in-use/free pmeg list tail
 *------------------------------------------------------------------------
 */
to_pmg_list(ppmeg, pmegptr)
     struct pmege *ppmeg;			/* pmeg(ptr) being added */
     struct pmege *pmegptr;			/* list tail pointer	 */
{
    pmegptr->prev->next = ppmeg;
    ppmeg->prev = pmegptr->prev;
    pmegptr->prev = ppmeg;
    ppmeg->next = pmegptr;
}

/*------------------------------------------------------------------------
 *  droppmeg - drop a pmeg from the pmeg/free pmeg list 
 *------------------------------------------------------------------------
 */
droppmeg(ppmeg)
     struct pmege *ppmeg;
{
    ppmeg->prev->next = ppmeg->next;
    ppmeg->next->prev = ppmeg->prev;
}

/*------------------------------------------------------------------------
 *  pmeg_init - initialize pmeg table entries
 *------------------------------------------------------------------------
 */
pmeg_init()
{
    int i;
    struct pmege *ppmeg;
    long addr;
    int pmeg;

    /* init */
    for (i=0; i<VM_NUM_PMEGS + 4; ++i) {
	ppmeg = &pmegtbl[i];
	ppmeg->next  = ppmeg->prev = (struct pmege *) NULL;
	ppmeg->num   = i;
	clearpmeg(ppmeg);
    }

    /* initialize pmegfree - make it head and tail of the free list */
    pmegtbl[VM_PMEG_FREEHD].next = &(pmegtbl[VM_PMEG_FREETL]);
    pmegtbl[VM_PMEG_FREETL].prev = &(pmegtbl[VM_PMEG_FREEHD]);

    /* initialize pmeg list head and tail */
    pmegtbl[VM_PMEG_LISTHD].next = &(pmegtbl[VM_PMEG_LISTTL]);
    pmegtbl[VM_PMEG_LISTTL].prev = &(pmegtbl[VM_PMEG_LISTHD]);

    /* Mark all PMEGS in use as being locked */
    for (addr = 0; addr < VM_MAXADDR; addr += SEGSIZ) {
	pmeg = GetSegMap(addr);
	if (pmeg != VM_PMEG_INVALID) {
#ifdef DEBUG_PMEG_INIT
	   kprintf("pmeg_init: pmeg %d is in use at addr= 0x%08x\n",pmeg,addr);
#endif
/*	   if (pmeg > 0x40)*/
/*	       print_seg_ptes(addr);*/

	    pmegtbl[pmeg].stat = VM_PMEG_LOCKED;
	    pmegtbl[pmeg].num = pmeg;
	    pmegtbl[pmeg].pid = XINUASID; /* kinda bogus */
	    pmegtbl[pmeg].vaddr = addr;
	}
    }

    /* make VM_PMEG_INVALID pmeg locked */
    pmeg = VM_PMEG_INVALID;
    if (pmegtbl[pmeg].stat != VM_PMEG_LOCKED) {
	pmegtbl[pmeg].stat = VM_PMEG_LOCKED;
	    pmegtbl[pmeg].num = pmeg;
	    pmegtbl[pmeg].pid = XINUASID; 	/* kinda bogus */
	    pmegtbl[pmeg].vaddr = 0;		/* bogus */
    }

    /* put all unlocked pmegs on the free list */
    for (i=0; i<VM_NUM_PMEGS; ++i) {
	if (pmegtbl[i].stat == VM_PMEG_FREE)
	    to_pmg_list(&pmegtbl[i],&pmegtbl[VM_PMEG_FREETL]);
    }
}


/*------------------------------------------------------------------------
 *  getpmeg - find a free pmeg, and map it at the address specified
 *------------------------------------------------------------------------
 */
getpmeg(pid, vaddr)
     int pid;
     unsigned long vaddr;
{
    PStype ps;
    int    pmeg;

#ifdef DEBUG_GETPMEG
    kprintf("in getpmeg(pid=%d, vaddr=0x%08x)\n", pid, vaddr);
#endif
    disable(ps);

    pmeg = findapmeg(pid, vaddr);
#ifdef DEBUG_GETPMEG
    kprintf("find a free pmeg %d\n", pmeg);
#endif

    /* make sure we were able to find one */
    if (pmeg == SYSERR) {
	restore(ps);
	return(SYSERR);
    }

#ifdef DEBUG_GETPMEG
    kprintf("updating contexts\n");
#endif
    /* update segment map in mmu for the context associated with pid */
    updatecontexts(pid, vaddr, pmeg);

#ifdef DEBUG_GETPMEG
    kprintf("move pte to mmu\n");
#endif
    /* copy the in memory pte entries to the mmu */
    mvptetommu(pmeg);
    
    restore(ps);

#ifdef DEBUG_GETPMEG
    kprintf("getpmeg: pmeg %d is free, use it.\n", pmeg);
#endif
    return(pmeg);
}


/*---------------------------------------------------------------------------
 * findapmeg - find a free pmeg
 *---------------------------------------------------------------------------
 */
findapmeg(pid,vaddr)
     int   pid;
     long	vaddr;
{
    PStype ps;
    struct pmege *ppmeg;
    int    pmeg;
    int  seg;

    disable(ps);

    pmeg = SYSERR;

    /* first check for a pmeg on the free list */
    if (pmegtbl[VM_PMEG_FREEHD].next != &pmegtbl[VM_PMEG_FREETL]) {
	pmeg = pmegtbl[VM_PMEG_FREEHD].next->num;
    }

    /* if no free pmeg, try to reclaim one */
    if (pmeg == SYSERR) {
	pmeg = reclaimpmeg();
    }

    /* make sure we were able to find one */
    if (pmeg == SYSERR) {
	restore(ps);
	return(SYSERR);
    }

    /* at this point, 'pmeg' is a valid pmeg number which we can use */
    ppmeg = &pmegtbl[pmeg];

    /* remove it from its current list */
    droppmeg(ppmeg);

    /* fill in its fields */
    ppmeg->stat = VM_PMEG_INUSE;
    ppmeg->pid  = pid;
    ppmeg->vaddr = downtoseg(vaddr);	/* beginning v-addr of this segment */

    /* put this pmeg at the tail of the 'in use' list */
    to_pmg_list(ppmeg, &pmegtbl[VM_PMEG_LISTTL]);

    restore(ps);
    return(ppmeg->num);
}


/*------------------------------------------------------------------------
 *  reclaimpmeg - steal a pmeg and return its number
 *------------------------------------------------------------------------
 */
reclaimpmeg()
{
    long         p_vaddr;
    unsigned int pmeg_maxaddr;
    unsigned int pmeg_minaddr;
    int	 pmeg_id;
    int        pmeg_num;
    int        pmeg_ctxt;
    struct pmege *ppmeg;

    /* check each pmeg on the in-use list for a good one to steal */
    /* only VM_PMEG_INUSE type pmegs are on this list */
    for (ppmeg = pmegtbl[VM_PMEG_LISTHD].next;
	 ppmeg != &pmegtbl[VM_PMEG_LISTTL];
	 ppmeg = ppmeg->next) {

	pmeg_id      = ppmeg->pid;
	pmeg_num     = ppmeg->num;
	pmeg_minaddr = ppmeg->vaddr;
	pmeg_maxaddr = ppmeg->vaddr + SEGSIZ - 1;

	/* FIXME */
	/* for now, don't steal your own stack pages, that can be ugly */
	if (pmeg_id == getpid())
	    continue;

	/* Don't steal from the null process, because those pmegs are shared */
	if (pmeg_id == 0)
	    continue;

	/* is it owned by a process or an address space? */
	if (ispid(pmeg_id)) {
	    /* owned by a process: */
	    /* this can be process stack, kernel stack, RSA or S/P */

	    /* user or kernel process? */
	    if (iskernproc(pmeg_id)) {
		/* kernel process: */
		if (pmeg_maxaddr > (unsigned int) vmaddrsp.stkstrt)
		    /* We can't take the pages for S/P, RSA, or kernstack */
		    continue;

#ifdef DONT_TAKE_KERNEL_STACK
		if (pmeg_maxaddr > proctab[pmeg_id].plimit)
		    /* don't take pmegs from the used portion of */
		    /* a kernel process's stack */
		    continue;
#endif


#ifdef DEBUG_RECLAIMPMEG
		kprintf("stealing pmeg %d from kern proc %d, addr=%08x-%08x\n",
			pmeg_num, pmeg_id, pmeg_minaddr, pmeg_maxaddr);
#endif
	    } else {
		/* user process: */
		/* this is a user process stack, etc. */
		if (pmeg_maxaddr > (unsigned int) vmaddrsp.stkstrt)
		    /* We can't take the pages for S/P, RSA, or kernstack */
		    continue;
#ifdef DEBUG_RECLAIMPMEG
		kprintf("stealing pmeg %d from user proc %d, addr=%08x-%08x\n",
			pmeg_num, pmeg_id, pmeg_minaddr, pmeg_maxaddr);
#endif
	    }
	} else {
	    /* owned by an address space */
	    /* this can by text, data, or bss.  Any of those are reclaimable */
#ifdef DEBUG_RECLAIMPMEG
	    kprintf("stealing pmeg %d from asid %d, addr=%08x-%08x\n",
		    pmeg_num, pmeg_id, pmeg_minaddr, pmeg_maxaddr);
#endif
	}

	/* at this point, ppmege is the pmeg we want to steal */
	p_vaddr = pmeg_minaddr;

	/* copy the mmu resident info into the in-memory copy */
	/* for the original owner */
	mvptetomem(ppmeg->num);		/* move all pte's into memory	*/

	/* invalidate the in-memory copy */
	updatecontexts(pmeg_id, p_vaddr, VM_PMEG_INVALID);


	/* break out of the loop and return the pmeg found */
	return(pmeg_num);
    }

#ifdef DEBUG_RECLAIMPMEG
    kprintf("reclaimpmeg: couldn't find one\n");
#endif    

    /* couldn't find one */
    return(SYSERR);
}

/*------------------------------------------------------------------------
 *  freepmeg - return an unused pmeg to the end of the pmeg free list
 *------------------------------------------------------------------------
 */
freepmeg(pmeg)
     int pmeg;
{
    struct pmege *ppmeg;

    if (pmeg < 0 || pmeg >= VM_PMEG_INVALID)
	return(SYSERR);

    ppmeg = &pmegtbl[pmeg];
    /* maybe not too crucial to update contexts, but do it anyway */
    /*    updatecontexts(ppmeg->pid, ppmeg->vaddr, VM_PMEG_INVALID);*/
    /* If I add the above line, the # of free frames runs down	*/
    /* to 0 and the system hangs - dunno why			*/
    droppmeg(ppmeg);				/* drop it from pmeg list   */
    to_pmg_list(ppmeg,&pmegtbl[VM_PMEG_FREETL]);/* add it to free pmeg list */
    clearpmeg(ppmeg);

    return(OK);
}

/*---------------------------------------------------------------------------
 * prpmeglist - print out the free pmeg list
 *---------------------------------------------------------------------------
 */
prpmeglist()
{
    struct pmege *ppmeg;

    ppmeg = &(pmegtbl[VM_PMEG_FREEHD]);
    ppmeg = ppmeg->next;
    while (ppmeg != (&(pmegtbl[VM_PMEG_FREETL]))) {
	kprintf("pmeg %d is free\n", ppmeg->num);
	ppmeg = ppmeg->next;
    }
}

