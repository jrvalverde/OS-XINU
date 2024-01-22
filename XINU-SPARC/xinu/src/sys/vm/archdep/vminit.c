 /* vminit.c -- vminit */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <vmem.h>
#include <mem.h>
#include <q.h>

/*#define DEBUG_VMINIT*/
/*#define DEBUG_MAPDEVS*/
/*#define DEBUG_MAPKERNEL*/
/*#define DEBUG_DVMAINIT*/

extern char *etext, *edata;
extern char *start;		/* Margaret	*/

/*---------------------------------------------------------------------------
 * vminit - initialize virtaul memory and related data structures
 *          MACHINE DEPENDANT
 *---------------------------------------------------------------------------
 */
vminit()
{
    int locked_pmeg = 0;	/* locked PMEGs */

    /* FIX ME! There should have a better way */
    /* make sure the MMU constants are the same as those in PROM monitor */
    if (mmu_nctx != VM_NUM_CONTEXTS)
	panic("vminit: the VM_NUM_CONTEXTS constant should be %d", mmu_nctx);
    
    if (mmu_npmeg != VM_NUM_PMEGS)
	panic("vminit: the VM_NUM_PMEGS constant should be %d", mmu_npmeg);

    if (vac_line_size != VAC_LINESIZE)
	panic("vminit: the VAC_LINESIZE constant should be %d", vac_line_size);
    
    if (vac_size != VAC_SIZE)
	panic("vminit: the VAC_SIZE constant should be %d", vac_size);
    
    xkernsize = VM_KERNEL_SIZE + VM_DMA_NBYTES;

#ifdef DEBUG_VMINIT
    kprintf("calling vac_init(xkernsize = %d)\n", xkernsize);
#endif    
    /* make sure the VAC is turned off while we are initializing the	*/
    /* various VM contexts. Vac_Init() initializes and disables the VAC	*/
    vac_init();

#ifdef DEBUG_VMINIT
    kprintf("calling mmuinit()\n");
#endif    

    mmuinit();

#ifdef DEBUG_VMINIT
    kprintf("calling map_kernel()\n");
#endif    

    map_kernel(&locked_pmeg);

#ifdef DEBUG_VMINIT
    kprintf("calling map_devs()\n");
#endif    

    /* Map the devices into all contexts */
    map_devs(&locked_pmeg);

#ifdef DEBUG_VMINIT
    kprintf("calling dvma_init()\n");
#endif    

    dvma_init(&locked_pmeg);

#ifdef DEBUG_VMINIT
    kprintf("calling pmeg_init(locked_pmeg = %d)\n", locked_pmeg);
#endif    

    /* initialize pmeg table entries and free pmeg list */
    pmeg_init();
    
#ifdef DEBUG_VMINIT
    kprintf("calling ctxt_init()\n");
#endif    

    ctxt_init();

#ifdef DEBUG_VMINIT
    kprintf("calling fmtbl_init()\n");
#endif    

    fmtbl_init();

#ifdef USE_THE_VAC
    /* It should be safe to enable the VAC now */
    vac_Enable();
#endif
    
#ifdef DEBUG_VMINIT
    kprintf("vminit() done\n");
#endif    
}

/*---------------------------------------------------------------------------
 * map_kernel - initialize the virtual memory
 *---------------------------------------------------------------------------
 */
map_kernel(locked_pmeg)
     int *locked_pmeg;
{
    unsigned long	addr, addr2;
    int			pmeg;
    int			pgnum;
    unsigned int	value;
    union pte		pte;
    int			i;
    

#ifdef DEBUG_MAPKERNEL
    kprintf("in map_kernel()\n");
#endif    

    /* set up the definition of the address space		*/

    /* Set the Text, Data, Bss, Heap bounds */
    vmaddrsp.minaddr  = (char *) VM_MINADDR;	
    vmaddrsp.maxheap  = (char *) VM_USER_HEAP_END;
    
    /* Set the S/P, RSA, K-STK, and U-STK bounds.		*/
    /* Note, all these point to sizeof(int) bytes before the	*/
    /* end of the real region (this is for the stacks sake).	*/
    vmaddrsp.maxsp    = (char *) (VM_KERNEL_OFFSET - sizeof(int));
    vmaddrsp.rsaio    = (char *) (vmaddrsp.maxsp - VM_SP_SIZE);
    vmaddrsp.kernstk  = (char *) (vmaddrsp.rsaio - VM_RSA_SIZE);
    vmaddrsp.stkstrt  = (char *) (vmaddrsp.kernstk - VM_KSTK_SIZE);
    vmaddrsp.minstk   = (char *) (vmaddrsp.stkstrt - VM_USER_STACK_SIZE
				  + sizeof(int));
    
    /* make stack starting address 8-byte aligned */
    vmaddrsp.kernstk  = (char *) STK_ALIGN_8(vmaddrsp.kernstk);
    vmaddrsp.stkstrt  = (char *) STK_ALIGN_8(vmaddrsp.stkstrt);
    vmaddrsp.xinu     = (char *) &start;
    vmaddrsp.devio    = (char *) VM_DEVIO;
    vmaddrsp.pgtbl    = (char *) (-1);	/* not used by sun4c */
    vmaddrsp.ftbl     = (char *) &end;		
    vmaddrsp.xheapmin = (char *) 0;	/* set later in fmtbl_init */
    vmaddrsp.xheapmax = (char *) VM_KERNEL_MAXADDR;
    vmaddrsp.copyaddr = (char *) VM_COPY_BEGIN;
    vmaddrsp.pgscpaddr = (char *) PGS_COPY_BEGIN;
    vmaddrsp.pgrcpaddr = (char *) PGR_COPY_BEGIN;
    /* vmaddrsp.copypte set later below */
    vmaddrsp.copysize = VM_COPY_END-VM_COPY_BEGIN;

    /* From VM_KERNEL_MAXADDR to VM_MAXADDR */
    addr = downtoseg(VM_KERNEL_MAXADDR+PGSIZ);
    for (; addr < VM_MAXADDR; addr += SEGSIZ) {
	value = GetSegMap(addr);
	
	/* Unmap only if it points to low 16MB of phys mem.	*/
	/* Some mappings in this range point to the lower	*/
	/* 16MB of phys mem and are used by the PROM	*/
	if (value <= 0x40) {
	    value = VM_PMEG_INVALID;
	    i = 0;	/* to all contexts */
	}
	else {
	    i = 1;	/* copy it to other contexts */
	}
	
	for (; i < VM_NUM_CONTEXTS; i++) {	
	     setaseg(i, addr, value);
	 }
    }
    
    /* We are really already running in virtual memory cause	*/
    /* enough segments in high mem were mapped to pmegs 0 - ?	*/
    /* back in startup.s 					*/

#ifdef DEBUG_MAPKERNEL
    kprintf("map the kernel into all contexts\n");
#endif
    /* Map the kernel into all contexts */
    for (i = 0; i < VM_NUM_CONTEXTS; i++) { /* do it for each context */

	/* first mark all memory (except kernel t/d/b) as invalid	*/
	/* Note: unmapping the first segment on the ELC seems to	*/
	/* mess up the PROM monitor.  So go ahead and unmap it, but	*/
	/* map it back in in ret_mon so the PROM monitor will work	*/
	/* correctly.							*/
	for (addr = 0; addr < ((unsigned) vmaddrsp.maxsp); addr+=SEGSIZ) {
	    setaseg(i, addr, VM_PMEG_INVALID);
	}
	
	/* Since we have not yet initialized the kernel's free pmeg	*/
	/* list at this point, these will never appear on any pmeg	*/
	/* list.  Consequently, they will not be reclaimed.		*/
	addr = downtoseg(VM_KERNEL_TEXT);
	pmeg = 0;
	for (; addr <= VM_KERNEL_MAXADDR; addr += SEGSIZ) {
	    setaseg(i, addr, pmeg);
	    pmeg++;
	}

	/* Set up the copy buffer */
	setaseg(i, VM_COPY_BEGIN, pmeg);
    }

    /* only set it once */
    vmaddrsp.copypte.inmmu = TRUE;
    vmaddrsp.copypte.loc.mmuindex.pmeg = pmeg;
    vmaddrsp.copypte.loc.mmuindex.ptenum = VMPageOf(VM_COPY_BEGIN);
    
    vmaddrsp.pgscopypte.inmmu = TRUE;
    vmaddrsp.pgscopypte.loc.mmuindex.pmeg = pmeg;
    vmaddrsp.pgscopypte.loc.mmuindex.ptenum = VMPageOf(PGS_COPY_BEGIN);
    
    vmaddrsp.pgrcopypte.inmmu = TRUE;
    vmaddrsp.pgrcopypte.loc.mmuindex.pmeg = pmeg;
    vmaddrsp.pgrcopypte.loc.mmuindex.ptenum = VMPageOf(PGR_COPY_BEGIN);
    
    *locked_pmeg = pmeg + 1;	/* mark the pmegs that are locked */
   
    /* Start in context 0 */
    SetContext(0);
    curcontext = 0;
#ifdef DEBUG_MAPKERNEL
    kprintf("start in context zero\n");
#endif

    /* Map the kernel to low physical memory - just in case PROM didn't	*/
    /* do it for us.  At the same time, set the protection on the	*/
    /* kernel so users cannot access it.				*/

    /* First map the text segment */
    pte.value = 0;
    pte.st.hvalid = TRUE;
    pte.st.prot = U_KR;
    pte.st.dontcache = DO_CACHE;
    value = pte.value;
#ifdef DEBUG_MAPKERNEL
    kprintf("about to set kernel's text pte's to 0x%08x\n", value);
    kprintf("&etext = 0x%08x\n", &etext);
#endif
    for (addr = downtoseg(VM_KERNEL_TEXT); addr <= (unsigned int) &etext;
	 value++, addr += PGSIZ) {
	SetPageMap(addr, value);
    }

#ifdef DEBUG_MAPKERNEL
    kprintf("after setting kernel's text pte's, value = 0x%08x\n", value);
#endif
    /* Now map the data, bss, and heap */
    pte.value = value;			/* continue on from the last frame */
    pte.st.hvalid = TRUE;
    pte.st.prot = U_KRW;
    pte.st.dontcache = DO_CACHE;
    value = pte.value;
#ifdef DEBUG_MAPKERNEL
    kprintf("about to set kernel's d,b,h pte's to 0x%08x\n", value);
    kprintf("starting at address 0x%08x\n", addr);
#endif
    /* since the last part of the text segment and the first part of	*/
    /* the data segment are on the same page, we must make the page	*/
    /* writeable (i.e., part of the text will be writeable)		*/
    for (; addr <= VM_KERNEL_MAXADDR; value++, addr += PGSIZ) {
	SetPageMap(addr, value);
    }

    /* Make the page containing proc_start() readable by the user. This	*/
    /* should go away once we change processes to be self initializing.	*/
    {
	extern int proc_start();
	extern int change_to_user_mode();
	    
	pte.value = GetPageMap(proc_start);
	pte.st.prot = UR_KR;
	SetPageMap(proc_start, pte.value);

	pte.value = GetPageMap(change_to_user_mode);
	pte.st.prot = UR_KR;
	SetPageMap(change_to_user_mode, pte.value);
    }

    /* My favorite location to write to while doing assembly lang debugging. */
    /* Feel free to delete these lines.  It will help you find my memory */
    /* debugging statements :-) */
    pte.value = GetPageMap(0xfd02000);
    pte.st.prot = U_KRW;
    SetPageMap(0xfd02000, pte.value);
    pte.value = GetPageMap(0xfd03000);
    pte.st.prot = U_KRW;
    SetPageMap(0xfd03000, pte.value);

#ifdef DEBUG_MAPKERNEL
    kprintf("fill in the kernel (system space) redundant page map\n");
#endif
    /* fill in the kernel (system space) redundant page map */
    addr = downtoseg(VM_KERNEL_TEXT);
    for (pmeg=0; addr <= VM_KERNEL_MAXADDR; addr += SEGSIZ) {
	sysredpmeg[pmeg].pmeg = pmeg;
	for (i=0, addr2=addr; i<16; i++) {
	    sysredpmeg[pmeg].savpte[i] = GetPageMap(addr2);
	    addr2 += PGSIZ;
	}
	++pmeg;
    }
}

/*------------------------------------------------------------------------
 *  map_devs -  Map device control registers into virtual memory.
 *		use the prom monitor i/o pmeg if we can.
 *------------------------------------------------------------------------
 */
map_devs(locked_pmeg)
     int *locked_pmeg;
{
    union pte ld_dev_pte;
    unsigned long vaddr;
    int i, iopmeg;

    /* get ROM monitor pmeg */
    iopmeg = GetSegMap(IO_BASE_ADDR);	       /* assume we are in context 0 */

    /* IO_BASE_ADDR not mapped; get a free one */
    if (iopmeg == VM_PMEG_INVALID) {
	iopmeg = (*locked_pmeg)++;
    }
    
#ifdef DEBUG_MAPDEVS
    kprintf("map_devs: using pmeg %d for I/O devices\n", iopmeg);
#endif
    /* Map the pmeg into all contexts */
    for (i=0; i<VM_NUM_CONTEXTS; i++) {
	setaseg(i, IO_BASE_ADDR, iopmeg);
    }

    /* all sparc devices are in the obio space - so init all but frame # */
    ld_dev_pte.st.hvalid    = 1;	/* the page is valid		*/
    ld_dev_pte.st.prot      = PG_WS;	/* read/write by supervisor	*/
    ld_dev_pte.st.dontcache = DONT_CACHE;/* don't cache ld_dev pages!	*/
    ld_dev_pte.st.type      = ONBOARD_IO; /* map to obio space	*/

    /* Now set the magic frame # for each device */
	
    /* Interrupt Base Register */
    vaddr = (unsigned int) (INTERRUPT_REG);
    ld_dev_pte.st.pfn     = 0x5000;	/* magic obio frame #	*/
    SetPageMap(vaddr, ld_dev_pte.value);
#ifdef DEBUG_MAPDEVS
    kprintf("map_devs: set interrupt_base: vaddr=0x%08x value=0x%08x\n",
    	vaddr, GetPageMap(vaddr));
#endif
	
    /* Clock Base Register */
    vaddr = (unsigned int) (CLOCK_REG);
    ld_dev_pte.st.pfn     = 0x3000;	/* magic obio frame #	*/
    SetPageMap(vaddr, ld_dev_pte.value);
#ifdef DEBUG_MAPDEVS
    kprintf("map_devs: set clock_base: vaddr=0x%08x value=0x%08x\n",
      vaddr, GetPageMap(vaddr));
#endif
	
    /* Serial Line Unit Base Register */
    vaddr = (unsigned int) (SLU_REG);
    ld_dev_pte.st.pfn     = 0x1000;	/* magic obio frame #	*/
    SetPageMap(vaddr, ld_dev_pte.value);
#ifdef DEBUG_MAPDEVS
    kprintf("map_devs: set slu_base: vaddr=0x%08x value=0x%08x\n",
    	vaddr, GetPageMap(vaddr));
#endif
	
    /* Ethernet Base Register */
    vaddr = (unsigned int) (ETHERNET_REG);
    ld_dev_pte.st.pfn     = 0x8c00;	/* magic obio frame #	*/
    SetPageMap(vaddr, ld_dev_pte.value);
#ifdef DEBUG_MAPDEVS
    kprintf("map_devs: set ether_base: vaddr=0x%08x value=0x%08x\n",
    	vaddr, GetPageMap(vaddr));
#endif
	
    /* EEPROM Base Register */
    vaddr = (unsigned int) (EEPROM_ADDR);
    ld_dev_pte.st.pfn     = 0x2000;	/* magic obio frame #	*/
    SetPageMap(vaddr, ld_dev_pte.value);
#ifdef DEBUG_MAPDEVS
    kprintf("map_devs: set eeprom_base: vaddr=0x%08x value=0x%08x\n",
    	vaddr, GetPageMap(vaddr));
#endif
}

/*---------------------------------------------------------------------------
 * dvma_init - map frames into the dvma space of every context
 *---------------------------------------------------------------------------
 */
dvma_init(locked_pmeg)
     int *locked_pmeg;
{
    char *dma_addr;
    union pte pte;
    int i, j, frame;
    unsigned long vaddr;
    
    /* Map DVMA space into all contexts */
    vaddr = VM_DMA_BEGIN;
    for (j = 0; j < VM_DMA_NUMSEGS; j++) {
#ifdef DEBUG_DVMAINIT
	kprintf("dvma_init: using pmeg %d for dvma (vaddr = 0x%08x)\n",
		*locked_pmeg, vaddr);
#endif
	for (i=0; i<VM_NUM_CONTEXTS; i++) {
	    setaseg(i, vaddr, (*locked_pmeg));
	}

	vaddr += SEGSIZ;
	(*locked_pmeg)++;
    }
    
    /* We'll use the frames that start right after VM_KERNEL_SIZE */
    frame = VM_KERNEL_SIZE / PGSIZ;
    
    /* set the default type for a DMA Page Table Entry */
    pte.st.hvalid    = TRUE;
    pte.st.prot      = U_KRW;
    pte.st.dontcache = TRUE;
    pte.st.type      = ONBOARD_MEM;
    
    for (dma_addr = (char *) VM_DMA_BEGIN;
	 dma_addr < (char *) (VM_DMA_BEGIN + VM_DMA_NBYTES);
	 dma_addr += PGSIZ) {
	pte.st.pfn = frame;
	SetPageMap(dma_addr, pte.value);
	frame++;
    }
}

/*---------------------------------------------------------------------------
 * ctxt_init - initialize the list of free contexts
 *---------------------------------------------------------------------------
 */
ctxt_init()
{
    int i;
    struct ctxte *ctxtp;

    ctxtbl[CTXTHD].nextc = &ctxtbl[CTXTTL];
    ctxtbl[CTXTTL].prevc = &ctxtbl[CTXTHD];
    ctxtbl[CTXTHD].prevc = ctxtbl[CTXTTL].nextc = (struct ctxte *) EMPTY;
    for (i = 0; i < CTXTNUM-2; i++) {
	ctxtp = &ctxtbl[i];
	ctxtp->used = FALSE;
	ctxtp->pid  = EMPTY;
	ctxtp->num  = i;
	toctxt(ctxtp, &ctxtbl[CTXTHD]); /* linked after the queue head*/
    }
}


/*---------------------------------------------------------------------------
 * vac_init - Initialize the VAC and then enable it
 *---------------------------------------------------------------------------
 */
vac_init()
{
    vac_Initialize();
    vac_Disable();
}


/*---------------------------------------------------------------------------
 * print_whats_mapped() - debugging routine
 *---------------------------------------------------------------------------
 */
print_whats_mapped()
{
    unsigned int value, i, j;

    for (i=0; i < (unsigned int) 0x3ffc0000; i += SEGSIZ) {
	value = GetSegMap(i);
	kprintf("smeg(0x%08x) = 0x%08x\n", i, value);
    }
    
    for (i=(unsigned int) 0x3f000000; i < (unsigned int) 0x3ffc0000; i += SEGSIZ) {
	value = GetSegMap(i);
	kprintf("smeg(0x%08x) = 0x%08x\n", i, value);
    }
	
    for (i=0; i < (unsigned int) 0x3ffc0000; i += PGSIZ) {
	j = GetSegMap(i);
	if (j != VM_PMEG_INVALID) {
	    value = GetPageMap(i);
	    kprintf("smeg(0x%08x) = 0x%x, pmeg(0x%08x) = 0x%08x\n",
		    i, j, i, value);
	}
    }
}


/*-------------------------------------------------------------------------
 * print_seg_ptes - (for debugging) print ptes within a PMEG
 *-------------------------------------------------------------------------
 */
int print_seg_ptes(seg_addr)
     unsigned long seg_addr;	/* Segment address */
{
    unsigned int i, value;
    unsigned long addr = seg_addr;

    for (i = 0; i < VM_PGS_PER_SEG; i++) {
	value = GetPageMap(addr);
	kprintf("	addr = 0x%08x, pte = 0x%08x\n", addr, value);
	addr += PGSIZ;
    }
}

