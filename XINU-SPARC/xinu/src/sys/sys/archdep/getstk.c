/* getstk.c - map_ustk, map_kstk, stkreserve, ustkreserve, kstkreserve */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <vmem.h>

/*#define DEBUG*/
    
/*------------------------------------------------------------------------
 * map_ustk -- map user stack memory.
 *	       This should only be called with process ids (pids not asids)
 *------------------------------------------------------------------------
 */
map_ustk(pid, nframes, locked)
     unsigned int pid;			/* process id			*/
     int     nframes;			/* # of frmames needed		*/
     int     locked;			/* locked frame or not		*/
{
    unsigned	virtaddr, i, findex, numstkpgs;
    struct	ppte	ppte;		/* struct containing ptr to pte	*/
    struct	ppte	*ppteptr;	/* pointer to ppte		*/
    
    /* initialize user stack */
    virtaddr = (unsigned)vmaddrsp.stkstrt;

    /* map user stack region */
    for (i = 0; i < nframes; i++) {
	findex = getframe();
#ifdef DEBUG
	kprintf("in map_ustk: got frame %d\n", findex);
	kprintf("in map_ustk: calling mapframe: virtaddr = 0x%08x\n", virtaddr);
#endif
	/* make user stack frames non-shared */
	mapframe(pid, findex, virtaddr, URW_KRW, locked, FALSE);
	virtaddr -= PGSIZ;
    }
    
#ifdef DEBUG
    kprintf("in map_ustk after get & map frame for user stack \n");
#endif	
    
    numstkpgs = VM_USER_STACK_SIZE >> LOG2_PGSIZ;

#ifdef DEBUG
    kprintf("number of stack pages = %d\n", numstkpgs);
#endif
    
    ppteptr = &ppte;

    /* make all other stack pages zero-on-demand */
    getppte(pid, virtaddr, ppteptr);
    ppteptr->loc.pte->value = PTEZEROMASK;

    i++;
    for (; i < numstkpgs; i++) {	/* make other pgs zero-on-demand */
	prevppte(pid, &virtaddr, ppteptr);
	ppteptr->loc.pte->value = PTEZEROMASK;
    }

    /* make kernel stack area */
    map_kstk(pid, NFRAMES(VM_KSTK_SIZE));
    
    /* map parameter passing area (in RSA) */
    map_rsa(pid, NFRAMES(VM_RSA_SIZE));
    
#ifdef DEBUG
    kprintf("map_ustk finishes !! \n");
#endif	
}

/*------------------------------------------------------------------------
 * map_kstk -- map kernel stack and RSA area memory.
 *	       This should only be called pids not asids
 *------------------------------------------------------------------------
 */
map_kstk(pid, nframes)
     int pid;				/* process id			*/
     int nframes;			/* # of frames needed 		*/
{
    unsigned virtaddr, findex, i;
    
    /* map kernel stack -- locked */
    virtaddr = (unsigned)vmaddrsp.kernstk;
    for (i = 0; i < nframes; i++) {
	findex = getframe();
	/* lock, non-shared frames */
#ifdef DEBUG
	kprintf("map_kstk:pid %d, frame %d (0x%8x)\n", pid, findex, virtaddr);
#endif
	mapframe(pid, findex, virtaddr, URW_KRW, TRUE, FALSE);
	virtaddr -= PGSIZ;
    }
}


/*-------------------------------------------------------------------------
 * map_rsa - map the RSA (for system call parameter passing)
 *-------------------------------------------------------------------------
 */
int map_rsa(pid, nframes)
     int pid;				/* process id			*/
     int nframes;			/* # of frames needed 		*/
{
    unsigned virtaddr, findex, i;
    
    /* map the I/O area (in RSA) -- locked */
    virtaddr = (unsigned)vmaddrsp.rsaio;
    for (i = 0; i < nframes; i++) {
	findex = getframe();
#ifdef DEBUG
	kprintf("map_rsa: map frame %d (0x%8x)\n", findex, virtaddr);
#endif
	/* map it locked */
	mapframe(pid, findex, virtaddr, URW_KRW, TRUE, FALSE);
	virtaddr -= PGSIZ;
    }
    
#ifdef DEBUG
    kprintf("map_rsa finishes !! \n");
#endif	
}
