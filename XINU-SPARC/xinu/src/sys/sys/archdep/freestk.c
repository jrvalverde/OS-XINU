/* freestk.c	*/

#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <vmem.h>
#include <frame.h>
#include <proc.h>

#define SHOWFATALERRS

/*------------------------------------------------------------------------
 * freestk - free locked stack frames for a process
 *------------------------------------------------------------------------
*/

SYSCALL freestk(pid)
int	pid;
{
    PStype	ps;
    struct	ppte	ppte;		/* pg tbl entry pointer struct	*/
    struct	ppte	*ppteptr;	/* ptr to pte entry struct	*/
    int		stkpages;	        /* number of pages in stack	*/
    int		i, j;			/* counter variable & frame No.	*/
    unsigned long virtaddr;		/* a virtual address		*/

    if (isbadpid(pid))
	return(SYSERR);

    disable(ps);
    ppteptr = &ppte;

    /* kernel thread only has user stack */
    if (iskernproc(pid)) {
	/* free locked stack frames */
	virtaddr = (unsigned)vmaddrsp.kernstk;
	getppte(pid, virtaddr, ppteptr);
	stkpages = proctab[pid].pstklen / PGSIZ;

	for (i = 0; i < stkpages; i++) {
	    j = frameindex(getpfn(ppteptr));
#ifdef SHOWFATALERRS
	    if (!ftlocked(j)) {
		kprintf("freestk: kthread ustack frame not locked!\n");
		kprintf("freestk: frame = %d, list = %d\n", j, ftlist(j));
	    }
#endif
	    /* kernel thread stack frames are not on any list */
	    mvtoalist(j);  /* let frame manager collect them when it dies */	
	    /* we're currently running on this stack, */
	    /* so we can't invalidate it */
	    prevppte(pid, &virtaddr, ppteptr);
	    /* do not invalidata pte - still using it */
	}
	restore(ps);
	return (OK);
    }

    /* user thread stack frames are freed by the frame manager  */
    /* (clock daemon) using implicit time-out algorithm.	*/
    
    /* free frames used by the RSA + kernel stack */
    stkpages = NFRAMES(VM_RSA_SIZE + VM_KSTK_SIZE);
    virtaddr = (unsigned)vmaddrsp.rsaio;
    getppte(pid, virtaddr, ppteptr);
    for (i = 0; i < stkpages; i++) {
	j = frameindex(getpfn(ppteptr));
#ifdef SHOWFATALERRS
	if (!ftlocked(j)) {
	    kprintf("freestk: uthread kstack frame not locked!\n");
	    kprintf("freestk: frame = %d, list = %d\n", j, ftlist(j));
	}
#endif
	/* these frames are not on any list */
	mvtoalist(j);	/* let frame manager collect them when it dies */
	prevppte(pid, &virtaddr, ppteptr);
	/* do not invalidata pte - still using it */
    }
	
    restore(ps);
    return(OK);
}

