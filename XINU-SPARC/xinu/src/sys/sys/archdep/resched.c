/* resched.c  -  resched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <vmem.h>

#define	MEM_OUTPUT_DEBUGGING
/*#define DEBUG_RESCHED*/
/*#define MIN_DEBUG_RESCHED*/
/*#define MIN_MIN_DEBUG_RESCHED*/
/*#define DEBUG_CTXSW*/
/*#define PRINT_SAVED_CONTEXT*/

#ifdef MIN_MIN_DEBUG_RESCHED
int num_calls_to_resched = 0;
#endif

#ifdef IF_THIS_IS_NEEDED_FOR_SOME_STRANGE_REASON_THEN_DEFINE_IT
unsigned long currSP;	/* REAL sp of current process */
#endif

/* Global variable set/used in context switching */
int	chctxt;			/* reload the mmu context with new pg maps? */

/*------------------------------------------------------------------------
 * resched  --  reschedule processor to highest priority ready process
 *
 * Notes:	Upon entry, currpid gives current process id.
 *		Proctab[currpid].pstate gives correct NEXT state for
 *			current process if other than PRREADY.
 *------------------------------------------------------------------------
 */
int resched()
{
    register struct	pentry	*optr;	/* pointer to old process entry */
    register struct	pentry	*nptr;	/* pointer to new process entry */
    
#ifdef DEBUG_RESCHED
    kprintf("now in resched\n");
#endif

#ifdef	RTCLOCK
    preempt = QUANTUM;			/* reset preemption counter	*/
#endif

    /* no switch needed if current process priority higher than next */
    if (((optr= &proctab[currpid])->pstate == PRCURR) &&
	(lastkey(rdytail)<optr->pprio)) {
	return(OK);
    }

    /* force context switch */
    if (optr->pstate == PRCURR) {
	optr->pstate = PRREADY;
	insert(currpid, rdyhead, optr->pprio);
    }

#ifdef MEM_OUTPUT_DEBUGGING
    Debug("[");
    Debug(proctab[currpid].pname);
    Debug("->");
#endif
#if defined (DEBUG_RESCHED) || defined (MIN_DEBUG_RESCHED)
    kprintf("[%s(%d)->", proctab[currpid].pname, currpid);
#endif
/*    kprintf("[%s->", proctab[currpid].pname);*/
#ifdef MIN_MIN_DEBUG_RESCHED
    kprintf("{p%d", currpid);
#endif
    
    /* remove highest priority process at end of ready list */
    nptr = &proctab[(currpid = getlast(rdytail))];
    nptr->pstate = PRCURR;		/* mark it currently running	*/

#if defined (DEBUG_RESCHED) || defined (MIN_DEBUG_RESCHED)
    kprintf("%s(%d)]\n", proctab[currpid].pname, currpid);
#endif
#ifdef MIN_MIN_DEBUG_RESCHED
    kprintf("p%d}", currpid);
    if ((++num_calls_to_resched) > 8) {
	kprintf("\n");
	num_calls_to_resched = 0;
    }
#endif

#ifdef MEM_OUTPUT_DEBUGGING
    Debug(proctab[currpid].pname);
    Debug("]");
#endif
/*kprintf("%s]", proctab[currpid].pname);*/
#ifdef	DEBUG_RESCHED
    PrintSaved(nptr);
#endif

    if ((curcontext = nptr->ctxt) != EMPTY) {
	chctxt = FALSE;			/* don't need to fill in seg tbl */
    }
    else {
	chctxt = TRUE;			/* yes! fill in seg tbl for new ctxt */
	curcontext = newctxt(currpid);
/*    kprintf("<L%d>", curcontext);*/
    }
    mru(&ctxtbl[curcontext]);	/* make it most recently used*/

#ifdef DEBUG_RESCHED
    kprintf("now calling ctxsw()\n");
    kprintf("new process '%s' (pid=%d) should use context %d\n",
	    nptr->pname, currpid, curcontext);
    kprintf("optr->pregs=0x%08x\n",optr->pregs);
    kprintf("nptr->pregs=0x%08x\n",nptr->pregs);
#endif
    
#ifdef MEM_OUTPUT_DEBUGGING
    Debug("+");
#endif
    
#ifdef	DEBUG_RESCHED
    PrintSaved(optr);
#endif
    ctxsw(optr->pregs, nptr->pregs);
    /* The OLD process returns here when resumed. */
    
#ifdef MEM_OUTPUT_DEBUGGING    
    Debug("-");
#endif
/*kprintf("-\n");    */
#ifdef DEBUG_RESCHED
    kprintf("we are back from context switch, and we are\n");
    kprintf("in process %d in context %d\n", currpid, curcontext);
    kprintf("returning from resched\n");
#endif

    return(OK);
}

/* global variable used to avoid accessing the stack while in ctxsw */
int *ctxsw_tmp_npregs;

/*-------------------------------------------------------------------------
 * ctxsw - 
 *-------------------------------------------------------------------------
 */
ctxsw(opregs, npregs)
     int *opregs, *npregs;
{
    /* Both save_context and restore_context return to the	*/
    /* instruction after the save_context call.  The only	*/
    /* difference is the return value.  If we are really	*/
    /* returning from save_context, the return value will	*/
    /* be 0.  If we are in the new context and returning 	*/
    /* from restore_context, the return value will be 1.	*/


    /* Save npregs into a global variable so we can get at	*/
    /* it later without touching the stack.			*/
    /* If the optimizer is used, this statement can usually	*/
    /* be omitted, because any reference to npregs will not	*/
    /* reference the stack, just a register.  But if the	*/
    /* optimizer is not turned on, you never know what dump	*/
    /* (useless) references to memory might be generated.	*/
    ctxsw_tmp_npregs = npregs;

#ifdef DEBUG_CTXSW
    kprintf("about to save_context\n");
#endif
    if (save_context(opregs) == 0) {
#ifdef DEBUG_CTXSW
	/* use a temporary stack if we want to print msgs */
	asm("set 0x0fd03ffc, %fp");
	asm("set 0x0fd03f00, %sp");
	kprintf("Just before SetContext(%d)\n", curcontext);
#endif
	SetContext(curcontext);
	/* From this point to the end of this "if" stmt	*/
	/* we must be sure not to access the stk region	*/
	/* because we no longer have a stack!		*/
#ifdef DEBUG_CTXSW
	kprintf("Just before loadsegs(), chctxt = %d\n", chctxt);
#endif
	if (chctxt) {
	    /* Since this context used to be occupied by */
	    /* another process, we should flush the VAC	 */
#ifdef USE_THE_VAC		/* only flush if VAC is being used */
	    vac_flushContext();	/* flush the VAC */
#endif
	    loadsegs();
	}
#ifdef DEBUG_CTXSW
	kprintf("Just before restore_context()\n");
#endif
	/* Use ctxsw_tmp_npregs rather than npregs to	*/
	/* avoid touching the stack			*/
	restore_context(ctxsw_tmp_npregs);
	/* After restore_context, the stack is ok again	*/
    }
#ifdef DEBUG_CTXSW
    kprintf("just done with restore_context\n");
#endif
}


#ifdef PRINT_SAVED_CONTEXT
/* passed the pointer to the regs in the process entry */
PrintSaved(ptr)
     struct pentry *ptr;
{
    unsigned int i;

    kprintf("\nSaved context listing for process '%s' (%d)\n",ptr->pname, ptr - proctab);
    kprintf("\tPC: 0x%lx",(unsigned long) ptr->pregs[PC]);
    kprintf("  FP: 0x%lx",(unsigned long) ptr->pregs[FP]);
    kprintf("  SP: 0x%lx",(unsigned long) ptr->pregs[SP]);
    kprintf("  PS: 0x%lx\n",(unsigned long) ptr->pregs[PS]);
    for (i=0; i<7; ++i) {
	kprintf("\tg[%d]: 0x%08lx\n",i+1,(unsigned long) ptr->pregs[i]);
    }
    kprintf("\n");
}
#endif
