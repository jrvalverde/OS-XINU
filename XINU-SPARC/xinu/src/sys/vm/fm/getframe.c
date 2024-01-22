/* getframe.c */

#include <conf.h>
#include <q.h>
#include <kernel.h>
#include <proc.h>
#include <vmem.h>
#include <frame.h>

/*#define DEBUG */

/*
 *-------------------------------------------------------------------------
 * getframe
 *
 *    Locates frame at head of free list. Unlinks it from free list.
 * Returns index in to frame table of free frame.  If the frame was
 * still in use by some process, invalidate page table entries.
 * NOTE: THIS ROUTINE MAY BLOCK AND CALL RESCHED
 *-------------------------------------------------------------------------
*/
int getframe()
{
    PStype ps;				/* save processor status	*/
    int findex;				/* index into frame table	*/
    struct ppte *ppteptr;		/* ptr to page table entry	*/
    struct ppte ppte;
    struct fte *fptr; 
	    
#ifdef DEBUG
    kprintf("getframe: just called\n");
#endif

    /* Make sure there IS a free frame, and disable interupts - sdo */
    /* There's a rather subtle race condition here.  Between the    */
    /* wait() and the disable(), it's possible that the owner of    */
    /* of a newly free'd frame demands it back.  Pfault() will      */
    /* remove it from the reclaim (free) list and it's won't be     */
    /* there when we look at it.  This is extrememly rare.          */
    while (1) {
	wait(ftfreesem);
	
	disable(ps);
	/* Unlink the next frame from the head of the reclaimed list */
	if ((findex = ftrmhd(FTRHD)) != SYSERR)
	    break;
	restore(ps);
    }
    
    fptr = &ft[findex];
    /* Prepare the frame to be released */
    if (!ftobsolete(findex)) {
	ppteptr = &ppte;
	ftgetppte(findex, ppteptr);
	setinmem(ppteptr, FALSE);
	sethvalid(ppteptr, FALSE);
    }
    fteclear(fptr);
    
#ifdef DEBUG
    kprintf("getframe: got frame %d\n", findex);
#endif
    restore(ps);

    if (ftinfo.rlen < FM_LOW)
	send(fmgr, OK);

    return(findex);
}


