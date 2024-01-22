/* 
 * lcreate.c - Low-level Address Space/Process(Thread) Creation
 */

#include <a.out.h>
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <mem.h>
#include <vmem.h>
#include <frame.h>
#include <q.h>
#include <io.h>
#include <name.h>
#include <asl.h>
#include <varargs.h>
#include <network.h>
#include <xpppgio.h>

#define OPENERR		(-4)

#define PRINTERRORS
/*#define DEBUG_KCREATE*/
/*#define DEBUG_ASCREATE*/
/*#define DEBUG_PROCCREATE*/
/*#define DEBUG_GETAOUT*/
/*#define DEBUG_DYNAMIC*/
/*#define DEBUG_VMLOAD*/

/*------------------------------------------------------------------------
 * newpid  --  obtain a new (free) process id
 *------------------------------------------------------------------------
 */
LOCAL newpid()
{
    int	pid;			/* process id to return		*/
    int	i;

    for (i=0 ; i<NPROC ; i++) {	/* check all NPROC slots	*/
	if ( (pid=nextproc--) <= 0)
	    nextproc = NPROC-1;
	if (proctab[pid].pstate == PRFREE) {
	    return(pid);
	}
    }
    return(SYSERR);
}

/*--------------------------------------------------------------------------
 *  newasid - obtain a new (free) address space id
 *--------------------------------------------------------------------------
 */
LOCAL  newasid()
{
    int  asidindx;
    int  i;

    for (i = 0; i < NADDR; i++) {
	if ((asidindx = nextaddr--) <= 0)
	    nextaddr = NADDR - 1;
	if (!addrtab[asidindx].valid) {
	    return(asidindx + ASIDBASE);
	}
    };
    return(SYSERR);
}

/*-------------------------------------------------------------------------
 * getaouthd - get a.out header
 *-------------------------------------------------------------------------
 */
LOCAL getaouthd(fname, exec, tsize, dsize, bsize, aentp, txtoff, loadstart)
     char *fname;
     struct exec *exec;
     int  *tsize;
     int  *dsize; 
     int  *bsize;
     unsigned *aentp;
     unsigned *txtoff;
     unsigned *loadstart;
{
    int dvnum;

#ifdef DEBUG_GETAOUT
    kprintf("getaouthd: reading from file '%s'\n", fname);
#endif

    if ((dvnum = fopen(fname, "ro"))==SYSERR) {
	return(OPENERR);
    }

    read(dvnum, exec, sizeof(struct exec));

#ifdef SPARC 
    if (exec->a_magic != ZMAGIC || exec->a_machtype != M_SPARC) {
	close(dvnum);
        return(SYSERR);
    }
#endif

#ifdef SUN3
    if (exec->a_magic != ZMAGIC || exec->a_machtype != M_68020) {
	close(dvnum);
        return(SYSERR);
    }
#endif

    *txtoff = N_TXTOFF(*exec);		/* get text offset */
    *tsize = N_TEXTSIZE(*exec);
    *dsize = N_DATASIZE(*exec);
    *bsize = N_BSSSIZE(*exec);
    *aentp = (*exec).a_entry;
    *loadstart = N_TXTADDR(*exec);

#ifdef DEBUG_GETAOUT
    kprintf("  txtoff:      0x%x\n", *txtoff);
    kprintf("  text size:   0x%x\n", *tsize);
    kprintf("  data size:   0x%x\n", *dsize);
    kprintf("  bss size:    0x%x\n", *bsize);
    kprintf("  entry point: 0x%x\n", *aentp);
    kprintf("  load start:  0x%x\n", *loadstart);
#endif

    return(dvnum);
}

/*--------------------------------------------------------------------------
 *  copypte - copy pte's from one page table to another one
 *	      The main use for this routine is to copy a parents
 *	      shared/private pgtbls to the child.
 *--------------------------------------------------------------------------
 */
LOCAL copypte(spid, dpid, vaddr, len)
     int   spid;                              /* source as/proc id         */
     int   dpid;                              /* destination proc id       */
     char    *vaddr;                            /* starting page address     */
     int     len;                               /* num of pte's to be copied */
{
    struct ppte  sppte, dppte;	/* temp ppte structures		*/
    struct ppte  *sppteptr, *dppteptr; /* temp ppte pointers	*/
    PStype ps;			/* save process status word	*/
    int i;				/* counter variable		*/
    char *staddr;			/* start address		*/

    if (isasid(dpid))
	return(SYSERR);		/* never copy to addr sp pte's	*/

    disable(ps);
    staddr = vaddr;
    sppteptr = &sppte;
    dppteptr = &dppte;
    getppte(spid, staddr, sppteptr);
    getppte(dpid, staddr, dppteptr);

    for (i = 0; i < len; i++) {
	setvalue(dppteptr, getvalue(sppteptr));
	nextppte(spid, &staddr, sppteptr);
	nextppte(dpid, &staddr, dppteptr);
    }
    restore(ps);
    return(OK);
}

/*--------------------------------------------------------------------------
 *  ascreate - create address space
 *--------------------------------------------------------------------------
 */
ascreate(fname, entp)
     char  *fname;	                      /* dynamic proc's a.out file  */
     unsigned  *entp;
{
    PStype 	ps;			/* saved processor status     */
    char   	*vaddr, *data_start, *bss_start;
    int 	fdnum, i;
    int 	textsize, datasize, bsssize;
    unsigned  	aentry, txtoffset, loadstart;
    int    	findex;
    int  	asid, asididx;
    register struct aentry  *aptr;
    struct ppte ppte;
    register struct ppte  *ppteptr;
    struct mblock	tmpmblock;
    char 	*heapstart;

    disable(ps);

    if ((asid = newasid())== SYSERR) {
	restore(ps);
	return(SYSERR);
    };

    numaddr++;
    aptr = &addrtab[asididx = asidindex(asid)]; /* index in addrtab */

#ifdef DEBUG_ASCREATE
    kprintf("in ascreate: about to read a.out header from '%s'\n", fname);
#endif

    fdnum = getaouthd(fname, &aptr->aout, &textsize, &datasize, &bsssize,
		      &aentry, &txtoffset, &loadstart);

    if (fdnum < 0) {
	if (fdnum == OPENERR)
	    kprintf("Can not open file: %s\n\n", fname);
	else
	    kprintf("File %s is not executable.\n", fname);
	restore(ps);
	return(SYSERR);
    }

    /* check to make sure the program will fit */
    if ((loadstart + textsize + datasize + bsssize) >
	(unsigned) vmaddrsp.maxheap) {
	kprintf("Program is too large for virtual memory\n");
	restore(ps);
	return(SYSERR);
    }

    findex = getframe();	/* frame for first page of heap */

    aptr->valid  = 1;
    aptr->psinformed = 0;	/* server hasn't been informed*/
    aptr->numthd = 0;		/* no threads for the moment */
    aptr->thd_id = EMPTY;

    /* initialize in-memory page table entries */
    bzero(&addrpmegs[asididx], sizeof(struct addrpgtbl));
    for (i=0; i<MAXADDRPMEGS; i++) {
	addrpmegs[asididx].redpmeg[i].pmeg = VM_PMEG_INVALID;
    }

    data_start = (char *)N_DATADDR(aptr->aout);
    bss_start = (char*)N_BSSADDR(aptr->aout);
    
    /* set pte "aout" bit on TEXT and DATA */
    vaddr = (char *) loadstart;
    ppteptr = &ppte;
    getppte(asid, vaddr, ppteptr);

    for (;;) {
	ppteptr->loc.pte->value = PTE_LOAD_AOUT;
	if (vaddr < data_start) {
	    setprot(ppteptr, UR_KR);	/* TEXT */
	}
	else {
	    setprot(ppteptr, URW_KRW);	/* DATA */
	}
	vaddr += PGSIZ;
	if (vaddr > bss_start)
	    break;
	getppte(asid, vaddr, ppteptr);
    };

    /* others zero-on-demand */
    vaddr = (char *) bss_start;
    ppteptr = &ppte;
    getppte(asid, vaddr, ppteptr);

    for (;;) {
	ppteptr->loc.pte->value = PTEZEROMASK;
	setprot(ppteptr, URW_KRW);
	vaddr += PGSIZ;
	if (vaddr > vmaddrsp.maxheap)
	    break;
	getppte(asid, vaddr, ppteptr);
    };

#ifdef DEBUG_ASCREATE
    kprintf("after init heap, end of heap is at vaddr=0x%08x\n", vaddr);
#endif

#ifdef DEBUG_ASCREATE
    kprintf("about to initialize the free heap list\n");
#endif
    /* initialize free virtual memory list */
    heapstart = (char *) roundpg(loadstart + textsize + datasize + bsssize);
    vaddr = heapstart;
    vmemlist[asididx].mnext = (struct mblock *)vaddr;

#ifdef DEBUG_ASCREATE
    kprintf("about to set tmpmblock\n");
#endif
    tmpmblock.mlen  = (unsigned) (vmaddrsp.maxheap - heapstart + 1);
    tmpmblock.mnext = (struct mblock *) NULL;

#ifdef DEBUG_ASCREATE
    kprintf("about to mapframe %d to vaddr 0x%08x in asid %d\n",
	    findex, vaddr, asid);
#endif
    /* make the first page writable  */
    mapframe(asid, findex, vaddr, URW_KRW, FALSE, FALSE);

#ifdef DEBUG_ASCREATE
    kprintf("about to fill it in via vbcopy\n");
#endif
    vbcopy(&tmpmblock, currpid, vaddr, asid, sizeof(struct mblock), TRUE);


    if (pgiocreate(asid) != OK) {
	kprintf("Couldn't create swap space for address space\n");
	restore(ps);
	killas(asid);
	return(SYSERR);
    }

    /* store the device number in the addr table for later loading */
    aptr->devload = fdnum;

    *entp = aentry;

#ifdef DEBUG_ASCREATE
    kprintf("ascreate: done with asid %d\n", asid);
#endif

    /* return addr space id, which is the index plus ASIDBASE */
    restore(ps);
    return(asid);                          
}


/*--------------------------------------------------------------------------
 *  procreate - create a process (thread) within a virtual address space
 *--------------------------------------------------------------------------
 */
procreate(asid, procaddr, priority, name, nargs, argpt)
     int    asid;       	/* addr space thread is in  */
     int    *procaddr;          /* entry point		    */
     int    priority;           /* process priority         */
     char   *name;              /* name (for debugging)     */
     int    nargs;              /* arguments(treated like an*/
     int    *argpt;             /* array in the code)       */
{
    PStype ps;
    unsigned    pid, saddr, i, temp;
    struct pentry	*pptr;
    struct aentry	*aptr;
    int	   UINITRET();
    int	   proc_start();

    disable(ps);

    if (isbadasid(asid) || (pid = newpid()) == SYSERR || isodd(procaddr)) {
	restore(ps);
	return(SYSERR);
    }

    saddr = (unsigned)((unsigned)(vmaddrsp.stkstrt));
    numproc++;
    aptr = &addrtab[asidindex(asid)]; 
    pptr = &proctab[pidindex(pid)];
    pptr->pstate = PRSUSP;
    for (i=0 ; i<PNMLEN && (pptr->pname[i] = name[i])!=0; i++)
	;
    pptr->pprio  = priority;
    pptr->psem  = 0;
    pptr->phasmsg  = FALSE;
    pptr->plimit = (int)vmaddrsp.minstk;
    pptr->pbase = (int)saddr;
    pptr->pstklen = ((int)saddr - (int)vmaddrsp.minstk + sizeof(int));
    pptr->pargs  = nargs;
    pptr->pnxtkin = BADPID;
    pptr->pdevs[0] = pptr->pdevs[1] = BADDEV;

    /* MACHINE DEPENDENT */
    for (i=0; i < PNREGS; i++)		/* to start out, just set them all */
	pptr->pregs[i] = INITREG;

    pptr->paddr = (WORD) procaddr;
    pptr->pregs[PC] = ((WORD)proc_start) - 8;
    pptr->pregs[PS] = INITKMPS;
    pptr->ctxt = EMPTY;			/* no context assigned */

    /* initialize in-memory proc-region page tables */
    bzero(&procpmegs[pidindex(pid)], sizeof(struct procpgtbl));
    for (i=0; i < MAXPROCPMEGS; i++) {
	procpmegs[pidindex(pid)].redpmeg[i].pmeg = VM_PMEG_INVALID;
    }

    /* END MACHINE DEPENDENT */

    pptr->asid = asid;
    if ((i = aptr->thd_id) != EMPTY) {	/* insert new thread into a list*/
	proctab[i].prev_thd = pid;	/* beginnig from addrtab    */
	pptr->next_thd = i;
    } else
	pptr->next_thd = EMPTY;
    pptr->prev_thd = EMPTY;
    aptr->thd_id = pid;
    aptr->numthd++;

    pptr->pginsem = screate(0);		/* semaphore for paging in   */
    pptr->psinformed = FALSE;           /* PS isn't informed now     */

    /* get a pmeg for the stack area */
    if (getpmeg(pid, saddr) == SYSERR) {
	restore(ps);
	return(SYSERR);
    }
    /* map user stack, kernel stack, sys call param passing  area */
    map_ustk(pid, NFRAMES(MINSTK), FALSE); /* user stack frames unlocked */

    /* MACHINE DEPENDENT */
#ifdef DEBUG_PROCCREATE
    kprintf("before adding args: saddr = 0x%08x\n", saddr);
#endif
    if (nargs < 7)			/* all args will fit in registers */
	pptr->pregs[FP] = ((WORD)saddr) - SPARCMINFRAME;
    else {				/* save space on stack for args past 6 */
	pptr->pregs[FP] = ((WORD)saddr) - SPARCMINFRAME - ((nargs-6)*4);
	pptr->pregs[FP] = STK_ALIGN_8(pptr->pregs[FP]);
    }

    pptr->pregs[SP] = pptr->pregs[FP] - SPARCMINFRAME; /* useless */
    
#ifdef DEBUG_PROCCREATE
    kprintf("pptr->pregs[FP] = 0x%08x\n", pptr->pregs[FP]);
#endif

    /* move args to the stack */
    /* move args[0 through 5] to %i[0 through 5] on stack */
    saddr = (unsigned) (pptr->pregs[FP] + IREG_0_OFFSET);
#ifdef DEBUG_PROCCREATE
    kprintf("copying arg 0 - 5 to saddr++, where saddr = 0x%08x\n", saddr);
#endif

    temp = (nargs < 6) ? nargs : 6 ;
    vbcopy(argpt, currpid, saddr, pid, (temp * sizeof(int)), TRUE);
    argpt += temp;
    /* move args[6 through (nargs-1)] to stack */
    if (nargs > 6) {
	saddr = (unsigned) (pptr->pregs[FP] + SAV_IREG_6_OFFSET);
#ifdef DEBUG_PROCCREATE
	kprintf("copying arg 6 - n to saddr++, where saddr = 0x%08x\n", saddr);
#endif
	temp = nargs - 6;
	vbcopy(argpt, currpid, saddr, pid, (temp * sizeof(int)), TRUE);
    }

    /* put procaddr on the stack in %l0		*/
    /*      - xinu convention (see proc_start.s)	*/
    saddr = (unsigned) (pptr->pregs[FP] + LREG_0_OFFSET);
#ifdef DEBUG_PROCCREATE
    kprintf("cp procaddr=0x%08x to saddr++, where saddr = 0x%08x\n",
	    procaddr, saddr);
#endif
    temp = (WORD) procaddr;		/* start address of procedure	*/
    vbcopy(&temp, currpid, saddr, pid, sizeof(int), TRUE);
    /* put UINITRET on the stack in %i7		*/
    /*      - xinu convention (see proc_start.s)	*/
    saddr = (unsigned) (pptr->pregs[FP] + HIDE_VAL_OFFSET - 4);
#ifdef DEBUG_PROCCREATE
    kprintf("copying UINITRET to saddr++, where saddr = 0x%08x\n", saddr);
#endif
    temp = (unsigned) UINITRET;		/* push on return address	*/
    vbcopy(&temp, currpid, saddr, pid, sizeof(int), TRUE);
    /* MACHINE DEPENDENT ends */

    /* commented out 2-28-92 - griff - since S/P doesn't work anyway */
#ifdef THE_SHARED_PRIVATE_AREA_NOW_WORKS
    /* S/P area pte's */
    n = roundpg((int)(vmaddrsp.maxsp - vmaddrsp.rsaio))/PGSIZ;
    /* num of S/P pages*/
    if (n > 0) copypte( currpid, pid, vmaddrsp.rsaio+1, n );
    /* copy parent's pte's  */
#endif

    if (pgiocreate(pid) != OK) {
	kprintf("Couldn't create swap space for process\n");
	restore(ps);
	kill(pid);
	return(SYSERR);
    }

#ifdef DEBUG_PROCCREATE
    kprintf("procreate: done. (%d)\n", pid);
#endif
    restore(ps);
    return(pid);
}

/*--------------------------------------------------------------------------
 *  kcreate - create a kernel process (thread) with all stack frames locked
 *--------------------------------------------------------------------------
 */
kcreate(procaddr, ssize, priority, name, nargs, va_alist)
     int	*procaddr;		/* procedure address		*/
     int	ssize;			/* stack size (locked)      	*/
     int	priority;		/* process priority > 0		*/
     char	*name;			/* name (for debugging)		*/
     int	nargs;			/* number of args that follow	*/
     va_dcl				/* arguments (treated like an	*/
	 				/* array in the code)		*/
{
    STATWORD 	ps;    
    int		pid;			/* stores new process id	*/
    struct	pentry	*pptr;		/* pointer to proc. table entry */
    struct  	aentry  *aptr;		/* pointer to addr sp tbl entry */
    WORD	*saddr;			/* stack address		*/
    int		i, j, temp, nframes;
    int		INITRET();
    int		proc_start();
    va_list 	args;

#ifdef DEBUG_KCREATE
    kprintf("kcreate: creating process:  name=%s\n", name);
#endif

    disable(ps);
    if ((pid=newpid())== SYSERR || isodd(procaddr)) {
	restore(ps);
	return(SYSERR);
    }

    /* stack address */
    saddr = (int *)((unsigned)(vmaddrsp.kernstk));
    nframes = NFRAMES(ssize);
    numproc++;
    pptr = &proctab[pidindex(pid)];
    pptr->pstate = PRSUSP;
    for (i=0 ; i<PNMLEN && (pptr->pname[i]=name[i])!=0; i++)
	;
    pptr->pprio  = priority;
    pptr->psem  = 0;
    pptr->phasmsg  = FALSE;

    /* use user stack for kernel thread */
    pptr->pstklen = roundpg(ssize);
    pptr->plimit = (int)(saddr - pptr->pstklen + sizeof(int));
    pptr->pbase = (int)saddr;
    pptr->pargs  = nargs;
    pptr->pnxtkin = BADPID;
    pptr->pdevs[0] = pptr->pdevs[1] = BADDEV;

    /* MACHINE DEPENDENT */
    for (i=0; i < PNREGS; i++)	/* to start out, just set them all */
	pptr->pregs[i] = INITREG;
    pptr->paddr = (WORD) procaddr;
    pptr->pregs[PC] = ((WORD)proc_start) - 8;
    pptr->pregs[PS] = INITKMPS;
    pptr->ctxt = EMPTY;		/* no context assigned */

    /* initialize pmegs stored in memory to invalid */
    bzero(&procpmegs[pidindex(pid)], sizeof(struct procpgtbl));
    for (i = 0; i < MAXPROCPMEGS; i++) {
	procpmegs[pidindex(pid)].redpmeg[i].pmeg = VM_PMEG_INVALID;
    }

    /* MACHINE DEPENDENT ends */

    aptr = &addrtab[asidindex(XINUASID)];
    pptr->asid = XINUASID;
    if ((i = aptr->thd_id) != EMPTY) {   /*insert new thd into a list*/
	proctab[i].prev_thd = pid;       /* beginnig from addrtab    */
	pptr->next_thd = i;
    } else
	pptr->next_thd = EMPTY;
    pptr->prev_thd = EMPTY;
    aptr->thd_id = pid;
    aptr->numthd++;

    pptr->psinformed = FALSE;    /* PS isn't informed now     */

    /* get a pmeg for the stack area */
    if (getpmeg(pid, saddr) == SYSERR) {
	restore(ps);
	return(SYSERR);
    }
    
    /* map kernel thread run-time stack and lock initial stack frames */
    map_kstk(pid, nframes);

    /* MACHINE DEPENDENT */
#ifdef DEBUG_KCREATE
    kprintf("before adding args: saddr = 0x%08x\n", saddr);
#endif
    if (nargs < 7)		/* all args will fit in registers */
	pptr->pregs[FP] = ((WORD)saddr) - SPARCMINFRAME;
    else {			/* save space on stack for args past 6 */
	pptr->pregs[FP] = ((WORD)saddr) - SPARCMINFRAME - ((nargs-6)*4);
	pptr->pregs[FP] = STK_ALIGN_8(pptr->pregs[FP]);
    }
    
    pptr->pregs[SP] = pptr->pregs[FP] - SPARCMINFRAME; /* useless */

#ifdef DEBUG_KCREATE
    kprintf("pptr->pregs[FP] = 0x%08x\n", pptr->pregs[FP]);
#endif

    /* move args to the stack */
    va_start(args);
    /* move args[0 through 5] to %i[0 through 5] on stack */
    saddr = (int *) (pptr->pregs[FP] + IREG_0_OFFSET);
#ifdef DEBUG_KCREATE
    kprintf("copying arg 0 - 5 to saddr++, where saddr = 0x%08x\n", saddr);
#endif
    for( i= 0; ((i<6) && (i<nargs)); i++) {
	temp = va_arg(args,int);
	vbcopy(&temp, currpid, saddr, pid, sizeof(int), TRUE);
	saddr++;
    }

    /* move args[6 through (nargs-1)] to stack */
    saddr = (int *) (pptr->pregs[FP] + SAV_IREG_6_OFFSET);
#ifdef DEBUG_KCREATE
    kprintf("copying arg 6 - n to saddr++, where saddr = 0x%08x\n", saddr);
#endif
    for( i= 6; i < nargs; i++) {
	temp = va_arg(args,int);
	vbcopy(&temp, currpid, saddr, pid, sizeof(int), TRUE);
	saddr++;
    }

    /* put procaddr on the stack in %l0		*/
    /*      - xinu convention (see proc_start.s)	*/
    saddr = (int *) (pptr->pregs[FP] + LREG_0_OFFSET);
#ifdef DEBUG_KCREATE
    kprintf("cp procaddr=0x%08x to saddr++, where saddr = 0x%08x\n",
	    procaddr, saddr);
#endif
    temp = (WORD) procaddr;		/* start address of procedure	*/
    vbcopy(&temp, currpid, saddr, pid, sizeof(int), TRUE);

    /* put INITRET on the stack in %i7		*/
    /*      - xinu convention (see proc_start.s)	*/
    saddr = (int *) (pptr->pregs[FP] + HIDE_VAL_OFFSET - 4);
#ifdef DEBUG_KCREATE
    kprintf("copying INITRET to saddr++, where saddr = 0x%08x\n", saddr);
#endif
    temp = (WORD) INITRET;		/* push on return address	*/
    vbcopy(&temp, currpid, saddr, pid, sizeof(int), TRUE);

    /* MACHINE DEPENDENT ends */

    /* commented out 2-28-92 - griff - since S/P doesn't work anyway */
#ifdef THE_SHARED_PRIVATE_AREA_NOW_WORKS
    /* S/P area pte's */

    j = roundpg((int)(vmaddrsp.maxsp-vmaddrsp.rsaio))/PGSIZ;
    /* num of S/P pages*/
    if (j>0)  copypte( currpid, pid, vmaddrsp.rsaio+1, j );
#endif

    Debug("kcreate-done ");

#ifdef DEBUG_KCREATE
    kprintf("kcreate: done - name = %s\n", name);
#endif

    restore(ps);
    return(pid);
}

/*---------------------------------------------------------------------------
 * vmloadall - Load all of an a.out file dynamically from the file system
 *---------------------------------------------------------------------------
 */
vmloadall(asid)
     int asid;	/* address space ID */
{
    char        *vaddr;
    int         findex, len;
    struct exec *aouthdr;
    struct aentry  *aptr;
    union pte   pte;
    int         fdnum;

    aptr = &addrtab[asidindex(asid)];
    fdnum = aptr->devload;
    aouthdr = &aptr->aout;

#ifdef DEBUG_VMLOAD
    kprintf("text size = 0x%x (%d)\n", (*aouthdr).a_text, (*aouthdr).a_text);
    kprintf("data size = 0x%x (%d)\n", (*aouthdr).a_data, (*aouthdr).a_data);
    kprintf("bss  size = 0x%x (%d)\n", (*aouthdr).a_bss, (*aouthdr).a_bss);
    kprintf("entry pt  = 0x%x\n", (*aouthdr).a_entry);
#endif

    /* Load in the Text Segment */
    vaddr = (char *)N_TXTADDR(*aouthdr);

    if (((unsigned) vaddr % PGSIZ) != 0) {
	panic("vmload: Start of text segment is not page aligned!");
	/* if this happens, you must write a better vmload() routine */
    }

    seek(fdnum, N_TXTOFF(*aouthdr));	/* move to text offset  */
    len = (*aouthdr).a_text;

    if ((len % PGSIZ) != 0) {
	panic("vmload: text segment is not multiple of page size!");
    }
#ifdef DEBUG_VMLOAD
    kprintf("text start = 0x%08x, text end = 0x%08x (size=%d)\n", vaddr,
	    vaddr + (*aouthdr).a_text, len);
#endif

    /* map in the pages we will need. Assume len is in multiple of page size */
    while (len > 0) {
	findex = getframe();
	/* directly read into other address space */
	mapframe(asid, findex, vaddr, UR_KR, FALSE, FALSE);
	pte.value = GetPageMap(VM_COPY_BEGIN);
	pte.st.pfn = toframenum(findex);
	SetPageMap(VM_COPY_BEGIN, pte.value);
#ifdef DEBUG_VMLOAD
	kprintf("loading text into address 0x%08x (left=%d)\n", vaddr, len);
#endif
	/* load image from file system */
	if (len >= PGSIZ) {
	    if (read(fdnum, VM_COPY_BEGIN, PGSIZ) < 0)
		panic("vmload: error reading from file");
	    len -= PGSIZ;
	    vaddr += PGSIZ;
	}
    }

    /* Load in the Data Segment */
    vaddr = (char *) (N_DATADDR(*aouthdr));

    if ( ((unsigned) vaddr % PGSIZ) != 0 ) {
	panic("vmload: Start of data segment is not page aligned!");
 	/* if this happens, you must write a better vmload() routine */
    }

    len = (*aouthdr).a_data;
    if ((len % PGSIZ) != 0) {
	panic("vmload: data segment is not multiple of page size!");
    }
    seek(fdnum, N_DATOFF(*aouthdr));	/* move to data offset  */

#ifdef DEBUG_VMLOAD
    kprintf("data start = 0x%08x, data end = 0x%08x, size = %d\n", vaddr,
	    vaddr + (*aouthdr).a_data, len);
#endif

    /* map in the pages we will need. Assume len is in multiple of page size */
    while (len > 0) {
	findex = getframe();
	/* directly read into other address space */
	mapframe(asid, findex, vaddr, URW_KRW, FALSE, FALSE);
	pte.value = GetPageMap(VM_COPY_BEGIN);
	pte.st.pfn = toframenum(findex);
	SetPageMap(VM_COPY_BEGIN, pte.value);

#ifdef DEBUG_VMLOAD
	kprintf("loading data into address 0x%08x (left=%d)\n", vaddr, len);
#endif
	if (len >= PGSIZ) {
	    if (read(fdnum, VM_COPY_BEGIN, PGSIZ) < 0)
		panic("vmload: error reading from file");
	    len -= PGSIZ;
	    vaddr += PGSIZ;
	}
    }
#ifdef DEBUG_VMLOAD
	kprintf("vmload done!\n");
#endif
    return(OK);
}

/*-------------------------------------------------------------------------
 * vmloadpage - load a text or data page for the current process
 *-------------------------------------------------------------------------
 */
int vmloadpage(asid, vaddr)
     int asid;
     unsigned vaddr;
{
    int             fd;
    unsigned        tsize, dsize, txtoff, loadstart;
    int             len;
    struct  ppte    ppte;
    struct  ppte    *ppteptr;
    int             pmode;
    unsigned        fpos;
    struct aentry   *aptr;
    struct exec     *paout;

    aptr = &addrtab[asidindex(asid)];
    paout = &aptr->aout;

#ifdef DEBUG_DYNAMIC
    kprintf("vmloadpage(asid:%d, vaddr:0x%08x) called\n", asid, vaddr);
#endif

    /* check for a valid paging device */
    if (isbaddev(fd = aptr->devload)) {
#ifdef PRINTERRORS
        kprintf("vmloadpage: devload is bad (%d)\n", aptr->devload);
#endif
        return(SYSERR);
    }

    /* check the bounds of this program */
    tsize     = N_TEXTSIZE(*paout);
    dsize     = N_DATASIZE(*paout);
    loadstart = N_TXTADDR(*paout);
    txtoff    = N_TXTOFF  (*paout);

    /* make sure the address requested is within our reach */
    if ((vaddr < loadstart) || (vaddr > (loadstart + tsize + dsize))) {
#ifdef DEBUG_DYNAMIC
	kprintf("vmloadpage: 0x%x not TEXT or DATA\n", vaddr);
#endif
        return(SYSERR);
    }
    
    vaddr = truncpg(vaddr);
    /* read data from file */
    fpos = txtoff + (vaddr - loadstart);
    
    /* seek to the correct location in the file */
    seek(fd, fpos);

#ifdef DEBUG_DYNAMIC
    kprintf("vmloadpage: reading file offset 0x%x into addr 0x%x\n",fpos,vaddr);
#endif
    
    if ((len = read(fd, vaddr, PGSIZ)) <= 0) {
#if defined(PRINTERRORS)
        kprintf("vmloadpage: fatal read error\n");
#endif
        return(SYSERR);
    }

    if (vaddr <= (loadstart + tsize)) {
        /* text segment */
#if defined(DEBUG_DYNAMIC)
        kprintf("load TEXT page to 0x%08x\n", vaddr);
#endif
        /* this page should be read-only UNLESS some of the     */
        /* DATA segment resides on it                    */
        if ((truncpg(N_DATADDR(*paout)) == vaddr))
            pmode = URW_KRW;            /* user read/write      */
        else
            pmode = UR_KR;              /* user read only       */
    } else if (vaddr <= (loadstart + tsize + dsize)) {
        /* data segment */
#if defined(DEBUG_DYNAMIC)
        kprintf("load DATA page to 0x%08x\n", vaddr);
#endif
        pmode = URW_KRW;                /* user read/write      */
    }

    /* set proper page protections */

    ppteptr = &ppte;

    getppte(asid, vaddr, ppteptr);
    setprot(ppteptr, pmode);
    setmod(ppteptr, TRUE);	/* so it will be paged out to swap */
    setaout(ppteptr, FALSE);
    
    /* make sure 2-hand clock daemon does not reclaim it so soon */
    setreference(ppteptr, TRUE);

#ifdef DEBUG_DYNAMIC
    kprintf("vmloadpage: return\n");
#endif
    return(OK);
}
