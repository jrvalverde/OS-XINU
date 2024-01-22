/* clkint.c - handle clock interrupts */

/*#define DEBUG*/
/*#define FIXED_RUN_TIME	(10 * 6000)*/

#ifdef FIXED_RUN_TIME			/* only let xinu run for a 	*/
int griffcnt = 0;			/* fixed amount of time		*/
#endif

#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include <clock.h>
#include <interreg.h>
#include <mem.h>
#include <vmem.h>

extern unsigned int realtime;	/* 1/100 second counter */

clkint()
{
    STATWORD ps;
    struct counterregs *timer_chip;
    int encoded_limit;
    
    /* read the timer chip to reset it */
    timer_chip = (struct counterregs *)(CLOCK_REG);
    ps[0] = timer_chip->limit10;
    ps[0] = timer_chip->limit14;
    /* If I didn't assign it to a variable that gets used later on,	*/
    /* then it might get optimized away by the compiler		*/
    
#ifdef DEBUG
    kprintf("got a clock interrupt - processing it\n");
    kprintf("read timer chip - limit14 = 0x%08x\n", ps[0]);
#endif
#ifdef FIXED_RUN_TIME
    griffcnt++;
    if (griffcnt >= FIXED_RUN_TIME) {
	kprintf("clkint: Fixed Run Time exceeded - Terminating\n");
	griffcnt=0; /* reset it in case we reboot without reloading */
	halt();
    }
#endif
    /* inc realtime (it keeps track of the number of ticks since boot) */
    realtime++;
    
    count_per_tenth--;
    if (count_per_tenth > 0)
	return;
    else
	count_per_tenth = TICKS_PER_TENTH;
    
    /* 1/10 th of a second has elapsed - process the tick */
    
    count10--;
    if (count10 == 0) {
	clktime++;		/* inc time-of-day clock	*/
	count10 = 10;		/* reset count10		*/
    }
#ifdef DEBUG
    kprintf("clktime = 0x%08x\n", clktime);
#endif
    
    if (defclk) {			/* defer clock ticks		*/
	clkdiff++;
	return;			/* return quickly		*/
    }
    
#ifdef DEBUG
    kprintf("clkint before disable\n");
#endif
    disable(ps);
#ifdef DEBUG
    kprintf("clkint after disable\n");
#endif
    
    if (slnempty) {		/* sleep queue is not empty	*/
	*sltop = *sltop - 1;	/* decrement delta key		*/
	/* of first process */
	if (*sltop <= 0)
	    wakeup();		/* wake up process on sleep queue */
    }
    
    preempt--;			/* decrement preemption counter	*/
#ifdef DEBUG
    kprintf("preempt = %d\n", preempt);
#endif
    if (preempt == 0) {		/* and call resched if it 	*/
#ifdef DEBUG
	kprintf("\nGOING TO CALL ------- resched()\n\n");
#endif
	resched();		/* reaches 0			*/
    }
    
    restore(ps);
}
