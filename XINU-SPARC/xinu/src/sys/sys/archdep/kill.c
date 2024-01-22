/* 
 * kill.c - Kill a process
 * 
 */
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <vmem.h>
#include <xpppgio.h>
#include <q.h>
#include <io.h>

/*--------------------------------------------------------------------------
 *  kill - terminate a process or address space
 *--------------------------------------------------------------------------
 */
SYSCALL kill(id)
     int  id;
{
    PStype ps;
    
    disable(ps);
    if (ispid(id))
	killproc(id);
    else if (isasid(id))
	killas(id);
    else{
	restore(ps);
	return(SYSERR);
    }    
    
    restore(ps);
    return(OK);
}


/*--------------------------------------------------------------------------
 *  killas - kill an address space
 *--------------------------------------------------------------------------
 */
killas(asid)
     int  asid;
{
    struct aentry  *aptr;
    int    pid, pid1, i, pmeg;
    int    *buf1, *buf2, *getbuf();
    struct message *mptr;
    struct rqnode  *rqp;
    
#ifdef DEBUG
    kprintf("in killas, killing asid %d\n", asid);
#endif	
    
    /* check the arguments */
    if (isbadasid(asid) ||
	((aptr = &addrtab[asidindex(asid)])->valid == FALSE))
	return(SYSERR);
    
    /* close the device attached to the executable file */
    close(aptr->devload);

    /*
     * if this is NOT a kernel process, tell the page server that
     * it has died 
     */
    if (asid != XINUASID)
	pgioterminate(asid);

    /* kill remaining threads */
    pid = aptr->thd_id;
    while(pid != EMPTY) {
	pid1 = proctab[pid].next_thd;
	killproc(pid);
	pid = pid1;
    }
    
    /* modify the addres space table */
    aptr->valid = FALSE;
    aptr->tstamp++;
    numaddr--;
    
    for (i = 0; i < MAXADDRPMEGS; i++) {
	pmeg = addrpmegs[asidindex(asid)].redpmeg[i].pmeg;
	if (pmeg < VM_PMEG_INVALID)
	    freepmeg(pmeg);
    }
    
    return(OK);	
}

/*--------------------------------------------------------------------------
 *  killproc - terminate a process/thread
 *--------------------------------------------------------------------------
 */
killproc(pid)
     int  pid;
{
    struct pentry  *pptr;
    struct aentry  *aptr;
    struct message *mptr;
    struct rqnode  *rqp;
    int    asid, i, pmeg;
    int    thdid, k, dev;
    int    *buf1, *buf2, *getbuf();
    char str[80];			/* for debug messge		*/
    
#ifdef DEBUG
    kprintf("in killproc, currpid %d, killing pid %d\n", currpid, pid);
#endif	
    
    /* check arguments */
    if (isbadpid(pid) || (pptr = &proctab[pidindex(pid)])->pstate == PRFREE)
	return(SYSERR);
    
    
    /* if this is NOT a kernel process, tell the page server that   */
    /* it has died                                                  */
    if (proctab[pid].asid != XINUASID)
	pgioterminate(pid);

    if (--numproc == 0)
	xdone();			/* print: all u-proc done	*/
    
    dev = pptr->pdevs[0];
    if (!isbaddev(dev))
	close(dev);
    
    dev = pptr->pdevs[1];
    if (!isbaddev(dev))
	close(dev);
    
    send(pptr->pnxtkin, pid);		/* this causes a context switch */
    
    /* do not modify the process table before this point */
    pptr->tstamp++;			/* increment time stamp		*/
    sdelete(pptr->pginsem);		/* delete paging in semaphore	*/
    
    asid = pptr->asid;
    aptr = &addrtab[asidindex(asid)];
    dropthd(aptr,pptr);		/* drop from as's thread list	*/
    
    if (!aptr->numthd) 
	killas(asid);

    if (pptr->ctxt != EMPTY)
	freectxt(pptr->ctxt);
    
    pptr->ctxt = EMPTY;
    
    for (i = 0; i < MAXPROCPMEGS; i++) {
	pmeg = procpmegs[pidindex(pid)].redpmeg[i].pmeg;
	if (pmeg < VM_PMEG_INVALID)
	    freepmeg(pmeg);
    }

    /* clean up S/P pte's after the initial experiment */
    /* release frames belong to S/P pte's if any */
    
    freestk(pid);			/* use trick to release stack	*/
    
    switch (pptr->pstate) {
      case PRCURR:			/* suicide */
	pptr->pstate = PRFREE;
	resched();
      case PRWAIT:
	semaph[pptr->psem].semcnt++;
	/* fall through */
      case PRREADY:
	dequeue(pid);
	pptr->pstate = PRFREE;
	break;
      case PRSLEEP:
      case PRTRECV:
	unsleep(pid);
	/* fall through */
      default:
	pptr->pstate = PRFREE;
    }
    return(OK);
}


/*-------------------------------------------------------------------------
 * dropthd - drop a thread from its as's thread list  
 *-------------------------------------------------------------------------
 */
LOCAL dropthd(aptr, pptr)	
     struct aentry *aptr;
     struct pentry *pptr;
{
    int k;
    
    if(pptr->prev_thd==EMPTY)		/* it's first thd on list  */
	aptr->thd_id = k = pptr->next_thd;
    else 
	proctab[pptr->prev_thd].next_thd = k = pptr->next_thd;
	
    if (k != EMPTY)
	proctab[k].prev_thd = pptr->prev_thd;

    --aptr->numthd;
}
