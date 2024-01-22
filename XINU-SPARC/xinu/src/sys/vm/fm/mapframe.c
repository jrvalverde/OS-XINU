/* mapframe.c - mapframe */

#include <conf.h>
#include <kernel.h>
#include <vmem.h>
#include <frame.h>
#include <proc.h>

/*#define DEBUG*/
#define SHOWFATALERRS

/*---------------------------------------------------------------------------
 * mapframe - set frame table entry and init in-memory pte (not MMU)
 *---------------------------------------------------------------------------
*/
mapframe(id, ftidx, virtaddr, access_prot, locked, shared)
int	id;				/* process id or asid		*/
int	ftidx;				/* frame table index		*/
int	virtaddr;			/* virtual address		*/
int 	access_prot;			/* access protection		*/
int	locked;				/* frame is locked(not pageable)*/
int	shared;				/* frame is shared		*/
{
    PStype	ps;
    struct	fte	*fteptr;
    struct  ppte	ppte, *ppteptr;

#ifdef DEBUG
    kprintf("in mapframe: id=%d, ftidx=0x%x, virtaddr=0x%08x, locked=%d, shared=%d\n",
	    id, ftidx, virtaddr, locked, shared);
#endif

    if (isbadframe(ftidx) || (!ftlocked(ftidx))) {
#ifdef SHOWFATALERRS
	kprintf("mapframe: illegal frame %d\n", ftidx);
	panic("mapframe: list = %d\n", ftlist(ftidx));
#endif
	return(SYSERR);
    }

    if (!(ispid(id) || isasid(id))) {
#ifdef SHOWFATALERRS
	panic("mapframe: illegal id %d\n", id);
#endif
	return(SYSERR);
    }
    
    if (isasid(id) && (virtaddr > (long)vmaddrsp.maxheap)) {
#ifdef SHOWFATALERRS
	panic("mapframe: asid %d and vaddr %08x mismatch\n", id, virtaddr);
#endif
	return(SYSERR);
    }
    
    if (ispid(id) && (virtaddr < (long)vmaddrsp.minstk)) {
#ifdef SHOWFATALERRS
	panic("mapframe: pid %d and vaddr %08x mismatch\n", id, virtaddr);
#endif
	return(SYSERR);
    }
    
    disable(ps);

    fteptr = &ft[ftidx];
    fteptr->id = id;
    fteptr->bits.st.used = TRUE;
    fteptr->bits.st.shared = shared;
    fteptr->bits.st.wanted = FALSE;
    fteptr->bits.st.pgout = FALSE;
    fteptr->pageno = topgnum(virtaddr);
    
    if (ispid(id))			/* stack page - use process id */
	fteptr->tstamp = proctab[pidindex(id)].tstamp;
    else				/* use address space id */
	fteptr->tstamp = addrtab[asidindex(id)].tstamp;
    
    if (!locked)
	mvtoalist(ftidx);	/* if not locked, move frame to active list */

    ppteptr = &ppte;
    getppte(id, virtaddr, ppteptr);
#ifdef DEBUG
    kprintf("mapframe: just called getppte(%d, %08x, %08x)\n",
	    id, virtaddr, ppteptr);
    kprintf("mapframe: the ppteptr says inmmu=%d, pmeg=%d, ptenum=%d\n",
	    ppteptr->inmmu, ppteptr->loc.mmuindex.pmeg,
	    ppteptr->loc.mmuindex.ptenum);
#endif

    /* Note: if inmmu bit is set, set the MMU pte.			*/
    /*       Otherwise, set the in-memory copy.				*/
    setpfn(ppteptr, toframenum(ftidx));	/* set frame number 	 	*/
    setmod(ppteptr, FALSE);		/* set page not modified	*/
    setprot(ppteptr, access_prot);      /* set access protection bits	*/
    sethvalid(ppteptr, TRUE);		/* set valid bit		*/
    /* set in-memory copy */
    setinmem(ppteptr, TRUE);		/* set in-memory bit		*/
    settvalid(ppteptr, TRUE);		/* set truly valid bit		*/

#ifdef DEBUG
    kprintf("set access_prot=%x\n", access_prot);
#endif

    restore(ps);
    return(OK);
}
