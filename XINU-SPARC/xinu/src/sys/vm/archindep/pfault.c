/*----------------------------------------------------------------------
 * pfault.c  - page fault handler routine called by assembly level
 *             dispatcher which takes care of saving and restoring
 *             registers.
 *----------------------------------------------------------------------
 */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <vmem.h>
#include <frame.h>

/*#define DEBUG*/
#define PRINTERRORS

/*-------------------------------------------------------------------------
 * pfault - page fault handler. 
 *-------------------------------------------------------------------------
 */
void pfault(vaddr, write_access)
     int	vaddr;		/* address which resulted in the fault 	*/
     Bool	write_access;	/* was it a write access?		*/
{
    PStype  ps;
    struct  ppte *ppteptr;
    struct  ppte ppte;
    struct  fte	 *fteptr;
    int	    findex;
    int	    id, ret, prot, zero_filled;
    
#ifdef DEBUG
    kprintf("<PF%8x>", vaddr);
#endif
    if (vaddr < (unsigned)vmaddrsp.minaddr) {
	pfkill(currpid, "Illegal reference to low memory (below 0x%x)",
	       (unsigned)vmaddrsp.minaddr);
	return;
    }
    
    ppteptr = &ppte;
    
    /* get in-memory page table entry ptr for this addr */
    if (getppte(currpid, vaddr, ppteptr) == SYSERR) {
	pfkill(currpid, "Invalid virtual address (0x%08x)", (unsigned)vaddr);
	return;
    }

    /* check if it is properly initialied */
    if (!gettvalid(ppteptr)) { 
	pg.addrviolation++;
	pfkill(currpid, "Illegal reference to invalid memory (0x%08x)",
	       (unsigned)vaddr);
	return;
    }
    
    disable(ps);

    if (write_access)
	setmod(ppteptr, TRUE);    	/* set mod bit */
    
    /* not in memory, need to be paged in */
    if (!getinmem(ppteptr)) {
	if (vaddr <= (unsigned)(vmaddrsp.maxheap))
	    id = proctab[currpid].asid;
	else
	    id = currpid;

	prot = getprot(ppteptr);	/* get the original protection mode */
	zero_filled = getzero(ppteptr);
	
	/* get a free frame. N.B. Put frame on the locked list */
	findex = getframe();

	init_frame(id, findex, vaddr);

	/* map frame */
	setpfn(ppteptr, toframenum(findex)); /* set frame number */
	setprot(ppteptr, URW_KRW);      /* set it to URW_KRW for now */
	sethvalid(ppteptr, TRUE);	/* set valid bit */
	/* set in-memory copy */
	setinmem(ppteptr, TRUE);	/* set in-memory bit */
	settvalid(ppteptr, TRUE);	/* set truly valid bit */

	/* load from a.out */
	if (getaout(ppteptr)) {
#ifdef NFS_PAGING
	    /*
	     * Inform NFS page_in thread to fetch the image.
	     * NFS page_in thread has higher priority than
	     * user threads.
	     * N.B. wait on pginsem semaphore -- ctxsw()
	     */
	    load_image(id, findex, vaddr);
#else
	    vmloadpage(id, vaddr);
#endif
	    mvtoalist(findex);		/* move it to active list */
	    restore(ps);
	    return;
	}
	
	if (zero_filled) {		/* see if zero-filled */
#ifdef DEBUG
	    kprintf("pfault:zero-filled: getframe = %d\n", findex);
#endif
	    zerofill(truncpg(vaddr));	/* fill frame with zeroes */
	    pg.zodfaults++;

	    setzero(ppteptr, FALSE);
	}
	else {				/* not zero-filled, page in from swap space */
	    pg.diskfaults++;
	    
	    /* a request is built and will be put on pgin request queue */
	    /* N.B. wait on pginsem semaphore.				*/
	    pgiogetpage(id, findex, vaddr);
	    
	    /* set access protection bits */
	    setprot(ppteptr, prot);
	}
	/* 
	 * mark it referenced so it is not reclaimed immediately
	 * by the frame manager.
	 */
	setreference(ppteptr, TRUE);
	mvtoalist(findex);		/* move it to active list */
	restore(ps);
	return;	
    }

    /* page in memory */
    
    /* check if it is a protection fault */
    /* there is probably a more correct way to do this, but this*/
    /* should suffice for the moment 				*/
    if (gethvalid(ppteptr)) {
	/* MMU would have caught a protection violation in 	*/
	/* SPARC_low_level_page_fault_handler().  		*/
	/* Because SPARC_low_level_page_fault_handler does not 	*/
	/* handle pipeline faults correctly we sometimes call	*/
	/* pfault() when we shouldn't - in this case we end up	*/
	/* in this if stmt.  Since it is a false alarm, return	*/
	/* We get here because I don't fix pgfaults that 	*/
	/* occur somewhere in the pipeline architecture		*/
	restore(ps);
	return;
    }

    /* page needed is on memory list: R-list, M-list or N-list */
    findex = frameindex(getpfn(ppteptr)); /* note: pte contains physical 
					     frame number instead of index */
    sethvalid(ppteptr, TRUE);
    pg.memfaults++;

    if (ftonalist(findex)) {
#ifdef PRINTERRORS
	kprintf("pfault: id=%d, vaddr=%x, frame %d on a-list!\n",
		id, vaddr, findex);
#endif
	restore(ps);
	return;
    }
#ifdef DEBUG
    kprintf("<PFL%d,%d>\n", findex, ftlist(findex));
#endif
    fteptr = &ft[findex];
    if (fteptr->bits.st.pgout) {	/* in process of paging out */
	fteptr->bits.st.wanted = TRUE;	/* set wanted bit  */
    }
    
    if (ftonrlist(findex)) {		/* frame on reclaim list */
	semaph[ftfreesem].semcnt--;	/* decrease semaphore cnt  */
    }
    
    if (!ftlocked(findex))
	ftrm(findex);			/* remove it from list if not on N-list */
    mvtoalist(findex);			/* move it to active list */

    restore(ps);
    return;
}


/*-------------------------------------------------------------------------
 * init_frame - init frame table frame entry
 *-------------------------------------------------------------------------
 */
LOCAL init_frame(id, findex, vaddr)
     int id;		/* asid or pid */
     int findex;	/* frame index */
     int vaddr;		/* virtual address */
{
    struct	fte	*fteptr;
    
    fteptr = &ft[findex];
    fteptr->id = id;
    fteptr->bits.st.used = TRUE;
    fteptr->bits.st.shared = FALSE;
    fteptr->bits.st.wanted = FALSE;
    fteptr->bits.st.pgout = FALSE;
    fteptr->pageno = topgnum(vaddr);
    
    if (ispid(id))			/* stack page - use process id */
	fteptr->tstamp = proctab[pidindex(id)].tstamp;
    else				/* use address space id */
	fteptr->tstamp = addrtab[asidindex(id)].tstamp;
}

/*---------------------------------------------------------------------------
 * zerofill - zero fill a page (frame)
 *---------------------------------------------------------------------------
 */
LOCAL zerofill(vaddr)
     register int *vaddr;
{
    register int i;
    register int j;

    /* do 8 at a time to speed it up */
    j = PGSIZ / (sizeof(int)*8);
    for(i=0; i < j; i++) {
	*vaddr = (int)0;
	*(vaddr+1) = (int) 0;
	*(vaddr+2) = (int) 0;
	*(vaddr+3) = (int) 0;
	*(vaddr+4) = (int) 0;
	*(vaddr+5) = (int) 0;
	*(vaddr+6) = (int) 0;
	*(vaddr+7) = (int) 0;
	vaddr += 8;
    }
    return(OK);
}
    
/*---------------------------------------------------------------------------
 * pfkill - illegal memory reference by a process, kill it
 *---------------------------------------------------------------------------
 */
pfkill(pid, msg, a1, a2, a3, a4)
int pid;
char *msg;
int a1,a2,a3,a4;
{
    kprintf("Memory violation by process %d (%s):\n", pid, proctab[pid].pname);
    kprintf(msg,a1,a2,a3,a4);
    kprintf("\nKilling process %d\n", pid);
    PrintFaultType();
    kill(pid);
}
