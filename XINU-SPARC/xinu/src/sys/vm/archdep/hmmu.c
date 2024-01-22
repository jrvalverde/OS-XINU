/* hmmu.c - high-level  routines to use the Sparc mmu */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <mem.h>
#include <vmem.h>
#include <q.h>
#include <openprom.h>
#include <traps.h>


/*#define PRINTERRORS*/
/*#define DEBUG_MISC*/
/*#define DEBUG_SETASEG*/
/*#define DEBUG_LDSEGS*/
/*#define DEBUG_LDSEGS_DETAIL*/
/*#define DEBUG_LL_PFAULT*/
/*#define DEBUG_UPDATECONTEXTS*/
#define MEM_OUTPUT_DEBUG

/* Location of SPARC access exception vectors */
#define INST_ACCESS_EXCEPTION_VECTOR	1
#define DATA_ACCESS_EXCEPTION_VECTOR	9

struct pfinfo pfinfo;

union pte pteTemp;	/* for the pte macros */

/* in memory copies of page table entries */

/* redundant copy of sys pg tbl */
struct savepmeg sysredpmeg[VM_KERNEL_NUMSEGS];

/* redundant copy of process pgtbl one for each process		 */
struct procpgtbl procpmegs[NPROC];

/* redundant copy of addr sp pgtbl one for each addr sp		*/
struct addrpgtbl addrpmegs[NADDR];

/* redundant copy of cp pages pg tbl */
struct savepmeg copypmeg;

/* mmu context stuff */
int curcontext;				/* current context		*/
struct	ctxte	ctxtbl[CTXTNUM]; /* for mamaging context in MMU	*/

/*
 *----------------------------------------------------------------------
 * mmuinit() -- set up the Sparc MMU
 *----------------------------------------------------------------------
 */
mmuinit()
{
    int SPARC_low_level_page_fault_handler();

    /* set the data access exection vector */
    set_evec(DATA_ACCESS_EXCEPTION_VECTOR * 4,
	     SPARC_low_level_page_fault_handler);

    /* set the instruction access exection vector */
    set_evec(INST_ACCESS_EXCEPTION_VECTOR * 4,
	     SPARC_low_level_page_fault_handler);
}


/*
 *----------------------------------------------------------------------
 * SPARC_low_level_page_fault_handler()
 *        - see what caused the page fault - then call pfault()
 *----------------------------------------------------------------------
 */
SPARC_low_level_page_fault_handler(trap_vector, excptn_ptr)
     unsigned int trap_vector;
     struct exception_frame *excptn_ptr;
{
    int		pmeg;
    int		id;

    /* What was the PC and NPC at the time of the fault? */
    pfinfo.pc = (unsigned int) excptn_ptr->pc;
    pfinfo.npc = (unsigned int) excptn_ptr->npc;

    /***************************************************/
    /*****	 The order of the following two	   *****/
    /*****	statements is extremely important  *****/
    /***************************************************/

    /* Where and why did the fault occur? */
    pfinfo.vaddr = (unsigned int) GetPageFaultAddress(); 
    pfinfo.reason = GetPageFaultType(); /* this also clears the fault */

    /* Was it a data or instruction access? */
    pfinfo.on_data_access =
	(trap_vector == DATA_ACCESS_EXCEPTION_VECTOR) ? TRUE : FALSE;

    /* Was it a read or a write access */
    pfinfo.on_write_access = (vm_write_access(pfinfo.reason)) ? TRUE : FALSE;

    if (vm_translation_error(pfinfo.reason)) { 	/* try to fix the fault */
	pmeg = GetSegMap(pfinfo.vaddr);
	if (pmeg == VM_PMEG_INVALID){		/* It's a TLB fault */
#ifdef DEBUG_LL_PFAULT
	    kprintf("in segment fault (no pmeg allocated), vaddr = %08x\n",
		    pfinfo.vaddr);
	    PrintFaultType();
#endif
	    if (pfinfo.vaddr <= (long)vmaddrsp.maxheap)
		id = proctab[pidindex(currpid)].asid;
	    else
		id = currpid;

	    /* allocate a pmeg, and map it */
	    /* getpmeg() calls mvptetommu() to copy the in memory */
	    /* pte entries to the mmu */
	    pmeg = getpmeg(id, pfinfo.vaddr);
	    if (pmeg == SYSERR) {
		/* couldn't allocation a new pmeg */
		pfkill(currpid, "Page Fault: no pmegs available");
		return;
	    }
	    
	    /* the TLB (seg map) is all better now - so return */
	    return;
	}
	else {			/* It's not a TLB fault */
	    pfault(pfinfo.vaddr, pfinfo.on_write_access);
	    /* the fault should have been corrected - so return */
	    return;
	}
    }
    else if (vm_protection_error(pfinfo.reason)) {
	/* Oooo... that's bad, kill it */
	pfkill(currpid, "Page Fault: detected memory protection violation\n");
    }
    else {
	/* It's some other fatal vm error, so kill it */
	pfkill(currpid, "Page Fault: detected unusual memory violation\n");
    }
}


/*
 * ====================================================================
 * mvptetommu - move one PMEG page table entries from in-memory copy
 *              to MMU chip.
 * ====================================================================
 */
mvptetommu(pmeg)
     int	pmeg;
{
    int i;
    int pid;
    long seg;
    long raddr;
    long vaddr;
    union pte *pteptr;
    struct savepmeg *psavpmeg;

#ifdef DEBUG_MISC
    kprintf("mvptetommu: pmeg = %d\n", pmeg);
#endif

    /* map to curent address space */
    SetSegMap(vaddr=VM_MAPPING_SEGMENT, pmeg);

    /* to make this routine extremely efficient, we use the machine	*/
    /* dependent fact that the pte's we are moving into the mmu	*/
    /* are in consecutive memory locations.  Thus we simply inc	*/
    /* pteptr rather than calling the slow nextppte(ppte) routine	*/

    pid = pmegtbl[pmeg].pid;
    raddr = pmegtbl[pmeg].vaddr;
    seg = VMSegOf(raddr);

    if (isasid(pid)) {
	psavpmeg = &(addrpmegs[asidindex(pid)].redpmeg[seg]);
    }
    else if (ispid(pid)) {
	psavpmeg = &(procpmegs[pidindex(pid)].redpmeg[procsegindex(raddr)]);
    }
    else {
	kprintf("mvptetommu: error: invalid id in pmegtbl\n");
	return(SYSERR);
    }

    pteptr = (union pte *) &(psavpmeg->savpte[0]);

    /* move from in-memory page table to MMU */
    for (i=0; i< VM_PGS_PER_SEG; i++, pteptr++, vaddr += PGSIZ) {
	SetPageMap(vaddr, pteptr->value);
    }

    return(OK);
}

/*
 * ====================================================================
 * mvptetomem - move page table entry from MMU chip to memory.
 * ====================================================================
 */
mvptetomem(pmeg)
     int pmeg;
{
    struct pmege *pmegptr;
    int  	id, seg, i;
    long	vaddr;
    struct 	savepmeg *svptr;
    long	temppte;

    pmegptr = &pmegtbl[pmeg];

    SetSegMap(VM_MAPPING_SEGMENT,pmegptr->num);
    vaddr = VM_MAPPING_SEGMENT;

    id = pmegptr->pid;
    if (isasid(id)) {	
	seg = VMSegOf(pmegptr->vaddr);
	svptr = &addrpmegs[asidindex(id)].redpmeg[seg];
	for (i=0; i< VM_PGS_PER_SEG; i++, vaddr += PGSIZ) {
	    temppte = GetPageMap(vaddr);
	    cpmmupte(&temppte, &(svptr->savpte[i]));
	}
    }else if (ispid(id)) {
	seg = procsegindex(pmegptr->vaddr);
	svptr = &procpmegs[pidindex(id)].redpmeg[seg];
	for (i=0; i< VM_PGS_PER_SEG; i++, vaddr += PGSIZ) { 
	    temppte = GetPageMap(vaddr);
	    cpmmupte(&temppte, &(svptr->savpte[i]));
	}
    }else
	return(SYSERR);

    return(OK);
}

/*---------------------------------------------------------------------------
 * cpmmupte - copy a mmu page table entry to a memory pte 
 *---------------------------------------------------------------------------
 */
cpmmupte(pptefrom, ppteto)
     union pte *pptefrom, *ppteto;
{
    /* copy all fields except inmem, list, tvalid */
    ppteto->st.hvalid = pptefrom->st.hvalid;
    ppteto->st.prot = pptefrom->st.prot;
    ppteto->st.dontcache = pptefrom->st.dontcache;
    ppteto->st.type = pptefrom->st.type;
    ppteto->st.ref = pptefrom->st.ref;
    ppteto->st.mod = pptefrom->st.mod;
    ppteto->st.pfn = pptefrom->st.pfn;
}

/*
 * ====================================================================
 * newctxt -- obtain a new (free or used) context index
 * ====================================================================
 */		
newctxt(pid)
    int	pid;
{
    int 	i;
    struct  ctxte *ctxtptr;

    /* get new context from the head of the list */
    if ((ctxtptr = ctxtbl[CTXTHD].nextc)->used == TRUE)
	proctab[pidindex(ctxtptr->pid)].ctxt = EMPTY;
    else 
	ctxtptr->used = TRUE;

    ctxtptr->pid = pid;
    proctab[pidindex(pid)].ctxt = ctxtptr->num;
    return(ctxtptr->num);
}

/*
 * ===================================================================
 * mru -- put a context in the most recently used position
 * ====================================================================
 */
mru(ctp)
     struct  ctxte *ctp;
{
    struct  ctxte *p;
    
    /* unlink it */
    ctp->prevc->nextc = ctp->nextc;
    ctp->nextc->prevc = ctp->prevc;
    
    /* link it to the tail - i.e. the most recently used */
    p = ctxtbl[CTXTTL].prevc;
    ctxtbl[CTXTTL].prevc = ctp;
    ctp->nextc = p->nextc;
    p->nextc = ctp;
    ctp->prevc = p;
}

/*
 * ===================================================================
 * freectxt -- free a  context index
 * ====================================================================
 */
freectxt(ctxtnum)
     int ctxtnum;
{
    struct ctxte *ctxtptr;

    ctxtptr = &ctxtbl[ctxtnum];
    ctxtptr->used = FALSE;

    /* unlink it */
    ctxtptr->prevc->nextc = ctxtptr->nextc;
    ctxtptr->nextc->prevc = ctxtptr->prevc;

    /* link it in front */
    toctxt(ctxtptr, &ctxtbl[CTXTHD]);
}

/*
 *===================================================================
 * toctxt -- put a context table entry after another
 *           onto the the context queue
 * ====================================================================
 */
toctxt(ctp, ctqp)
     struct ctxte *ctp;			/* point to the ctxt to be inserted */
     struct ctxte *ctqp;		/* point to the one before inserted*/
{
    ctqp->nextc->prevc = ctp;
    ctp->nextc = ctqp->nextc;
    ctqp->nextc = ctp;
    ctp->prevc = ctqp;
}

/*
 * ====================================================================
 * prctxttbl -- print free context list
 * ====================================================================
 */		
prctxttbl()
{
    struct ctxte *ctxtptr;

    ctxtptr = ctxtbl[CTXTHD].nextc;
    while (ctxtptr != &ctxtbl[CTXTTL]) {
	kprintf("->%c(%d,%d)", ctxtptr->used ? 'u' : '?', ctxtptr->num,
	       ctxtptr->pid);
	
	ctxtptr = ctxtptr->nextc;
    }
    kprintf("\n");
}


/*
 * ====================================================================
 * loadsegs -- load process's segments into context
 *		called when it is about to run a process which got a new
 *		context(segment table) - we assume that currpid is the
 *		process that needs its segments loaded - we also assume
 *		that we do not have a valid stack under us, so we must 
 *		make sure this routine does not access the stack region
 *		at all.  This is one place that register windows come in
 *		handy - we can still make procedure calls as long as none
 *		of the routines called tries to access the stack space.
 * ====================================================================
 */
/* global variables to make sure we do not touch the stack */
int	ldsegs_id;
char	*ldsegs_vaddr;
struct 	savepmeg  *ldsegs_svptr;

loadsegs()
{
    /* load user stack PMEGs, kernel stack PMEGs, ...etc. */
    ldsegs_svptr = &(procpmegs[currpid].redpmeg[0]);
    ldsegs_vaddr = (char *) downtoseg((unsigned) vmaddrsp.minstk);
    
#ifdef DEBUG_LDSEGS
    kprintf("&(procpmegs[%d].redpmeg[0]) = 0x%08x\n", currpid, ldsegs_svptr);
#endif
    
    for (; ldsegs_vaddr < vmaddrsp.maxsp;
	 ldsegs_vaddr += SEGSIZ, ldsegs_svptr++) {
	SetSegMap(ldsegs_vaddr, ldsegs_svptr->pmeg);
#ifdef DEBUG_LDSEGS_DETAIL
	if (ldsegs_svptr->pmeg != VM_PMEG_INVALID)
	    kprintf("SetSegMap(vaddr=0x%08x, pmeg=0x%x)\n", ldsegs_vaddr,
		    ldsegs_svptr->pmeg);
#endif
    }

    /* load address space area PMEGs */
    ldsegs_id = proctab[currpid].asid;
    if (ldsegs_id != XINUASID) {
	ldsegs_id = asidindex(ldsegs_id);
	ldsegs_svptr = &(addrpmegs[ldsegs_id].redpmeg[0]);
	ldsegs_vaddr = (char *) downtoseg((unsigned) vmaddrsp.minaddr);
#ifdef DEBUG_LDSEGS
	kprintf("&(addrpmegs[%d].redpmeg[0]) = 0x%08x\n", ldsegs_id,
		ldsegs_svptr);
#endif
	for (; ldsegs_vaddr < vmaddrsp.maxheap;
	     ldsegs_vaddr += SEGSIZ, ldsegs_svptr++) {
	    SetSegMap(ldsegs_vaddr, ldsegs_svptr->pmeg);
#ifdef DEBUG_LDSEGS_DETAIL
	    if (ldsegs_svptr->pmeg != VM_PMEG_INVALID)
		kprintf("SetSegMap(vaddr=0x%08x, pmeg=0x%x)\n",
			ldsegs_vaddr, ldsegs_svptr->pmeg);
#endif
	} /* for */
    }
}

/*---------------------------------------------------------------------------
 * setaseg - Set a Segment Map in a specific context
 *---------------------------------------------------------------------------
 */
setaseg(cntxt, vaddr, pmeg)
     int cntxt;
     unsigned vaddr;
     int pmeg;
{
#ifdef DEBUG_SETASEG
    kprintf("setaseg(%d, %x, %d) may not work yet\n", cntxt, vaddr, pmeg);
#endif

    /* The only way I know how to set a segment map in a different	*/
    /* context is by calling the v_setcxsegmap routine in the PROM	*/
    (*romp->v_setcxsegmap)(cntxt, roundew(vaddr), pmeg);
}

/*---------------------------------------------------------------------------
 * updatecontexts - Change all the mmu contexts associated with id
 *---------------------------------------------------------------------------
 */
updatecontexts(id, vaddr, pmegnum)
     int 	id;
     long	vaddr;
     int 	pmegnum;
{
    int i, j;

    if (isasid(id)) {
	addrpmegs[asidindex(id)].redpmeg[VMSegOf(vaddr)].pmeg = pmegnum;
	/* set all the contexts containing this addr sp */
	if ((i=addrtab[asidindex(id)].thd_id) != EMPTY) {
	    if ((j=proctab[pidindex(i)].ctxt)!=EMPTY)
		setaseg(j, vaddr, pmegnum);

	    /* for all threads in the address space */
	    while ((i=proctab[pidindex(i)].next_thd) !=EMPTY) {
		if ((j=proctab[pidindex(i)].ctxt) !=EMPTY)
		    setaseg(j, vaddr, pmegnum);
	    }
	}
    }
    else if (ispid(id)) {
#ifdef DEBUG_UPDATECONTEXTS
	kprintf("updatecontexts: ");
	kprintf("setting procpmegs[%d].redpmeg[%d].pmeg = %d\n",
		pidindex(id), procsegindex(vaddr), pmegnum);
	kprintf("updatecontexts: ");
	kprintf("&procpmegs... = 0x%08x\n",
		&(procpmegs[pidindex(id)].redpmeg[procsegindex(vaddr)].pmeg) );
#endif
	procpmegs[pidindex(id)].redpmeg[procsegindex(vaddr)].pmeg = pmegnum;
	/* set the context containing this thread */
	if ((j=proctab[pidindex(id)].ctxt)!=EMPTY)
	    setaseg(j, vaddr, pmegnum);
    }
    else {	/* the sysredpmeg tbl is set up at startup and we 	*/
	        /* don't mess with it ever again			*/
	kprintf("updatecontexts: unknown id %d\n", id);
    }
}

/*---------------------------------------------------------------------------
 * PrintFaultType() - Print out the most recent page fault information
 *		      This is called by by pfkill()
 *---------------------------------------------------------------------------
 */
PrintFaultType()
{
    char reason[80];

    kprintf("Page Fault Information:\n");
    kprintf("\tPC:                0x%08x\n", pfinfo.pc);
    kprintf("\tNPC:               0x%08x\n", pfinfo.npc);
    kprintf("\tFaulting Address:  0x%08x\n", pfinfo.vaddr);
    if (vm_protection_error(pfinfo.reason))
	strcpy(reason, "Protection Violation");
    else if (vm_translation_error(pfinfo.reason))
	strcpy(reason, "Translation Error");
    else if (vm_timeout_error(pfinfo.reason))
	strcpy(reason, "Timeout Error");
    else if (vm_sbus_error(pfinfo.reason))
	strcpy(reason, "Sbus Error");
    else if (vm_sync_mem_error(pfinfo.reason))
	strcpy(reason, "Synchronous Memory Access Error");
    else if (vm_addr_size_error(pfinfo.reason))
	strcpy(reason, "Address Size Error");
    else if (vm_watchdog_reset_error(pfinfo.reason))
	strcpy(reason, "Watchdog Reset Error");
    else
	strcpy(reason, "Unknown Error");
    kprintf("\tFault Type:        %s\n", reason);
    kprintf("\tAccess Type:       %s\n",
	    pfinfo.on_data_access ? "Data Access" : "Instruction Access" );
    kprintf("\tRead/Write:        %s\n",
	    pfinfo.on_write_access ? "Write Access" : "Read Access" );
    if (pfinfo.on_data_access) {
	kprintf("\tValue at PC is:    0x%08x\n",
		*((unsigned *)pfinfo.pc) );
	kprintf("\tValue at NPC is:   0x%08x\n",
		*((unsigned *)pfinfo.npc) );
    }
}

#ifdef DEBUG
/*
 * ====================================================================
 * PpteptrToString - convert a ppteptr to human readable form
 *		some fields are in the mmu, some in memory
 * ====================================================================
 */
LOCAL char *PpteptrToString(ppteptr)
     struct ppte *ppteptr;
{
    union pte pte;
    union pte *ppte;
    static char ptebuf[80];

    if (!(ppteptr->inmmu)) {		/* entry is in memory */
	ppte = ppteptr->loc.pte;
	/*  sprintf(ptebuf,"%c%c%c%c:%s:%c%c%c%c:%05x :val = %08lx",
	    (ppte->st.hvalid==1)?'V':'I',
	    (ppte->st.prot>=2)?'W':'R',
	    ((ppte->st.prot%2)==0)?'U':'S',
	    (ppte->st.cache==1)?'c':'C',
	    
	    (ppte->st.type==0)?"OB_MEM":
	    (ppte->st.type==1)?"OB_DEV":
	    (ppte->st.type==2)?"VME16":"VME32",
	    
	    (ppte->st.ref==1)?'R':'r',
	    (ppte->st.mod==1)?'D':'d',
	    (ppte->st.inmem==1)?'M':'m',
	    (ppte->st.tvalid==1)?'T':'t',
	    ppte->st.pfn, ppte->value
	    );*/
    }
    else {				/* entry in mmu and memory */
	ppte = ppteptr->loc.pte;	/* memory copy */
	/* get mmu copy */
	SetSegMap(VM_MAPPING_SEGMENT, (ppteptr)->loc.mmuindex.pmeg);
	pte.value = GetPageMap(VM_MAPPING_SEGMENT
			       +(PGSIZ*(ppteptr)->loc.mmuindex.ptenum));
	pte.st.inmem = ppte->st.inmem;
	pte.st.tvalid = ppte->st.tvalid;
	/* now print correct fields */
	/*    sprintf(ptebuf,"%c%c%c%c:%s:%c%c%c%c:%05x :val = %08lx",
	      (pte.st.hvalid==1)?'V':'I',
	      (pte.st.prot>=2)?'W':'R',
	      ((pte.st.prot%2)==0)?'U':'S',
	      (pte.st.cache==1)?'c':'C',
	      
	      (pte.st.type==0)?"OB_MEM":
	      (pte.st.type==1)?"OB_DEV":
	      (pte.st.type==2)?"VME16":"VME32",
	      
	      (pte.st.ref==1)?'R':'r',
	      (pte.st.mod==1)?'D':'d',
	      (pte.st.inmem==1)?'M':'m',
	      (pte.st.tvalid==1)?'T':'t',
	      pte.st.pfn, pte.value
	      );*/
    }
    return(ptebuf);
}


/*
 * ====================================================================
 * PteToString - convert a pte to human readable form
 * ====================================================================
 */

LOCAL char *PteToString(ppte)
     union pte *ppte;
{
    static char ptebuf[80];

    sprintf(ptebuf,"%c%c%c%c:%s:%c%c%c%c:%05x :val = %08lx",
	    (ppte->st.hvalid==1)?'V':'I',
	    (ppte->st.prot>=2)?'W':'R',
	    ((ppte->st.prot%2)==0)?'U':'S',
	    (ppte->st.dontcache==1)?'c':'C',

	    (ppte->st.type==0)?"OB_MEM":
	    (ppte->st.type==1)?"OB_DEV":
	    (ppte->st.type==2)?"VME16":"VME32",

	    (ppte->st.ref==1)?'R':'r',
	    (ppte->st.mod==1)?'D':'d',
	    (ppte->st.inmem==1)?'M':'m',
	    (ppte->st.tvalid==1)?'T':'t',
	    ppte->st.pfn, ppte->value
	    );
    return(ptebuf);
}
#endif
