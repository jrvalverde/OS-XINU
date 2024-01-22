/* clkinit.c - clkinit, updateleds, dog_timeout */

#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include <clock.h>
#include <interreg.h>
#include <mem.h>    
#include <vmem.h>

/*#define DEBUG*/
    
/* real-time clock variables and sleeping process queue pointers	*/
    
int	count10;		/* counts in 10ths of second 10-0	*/
int	clmutex;		/* mutual exclusion for time-of-day	*/
unsigned long	clktime;	/* current time in seconds since 1/1/70	*/
int     defclk;			/* non-zero, then deferring clock count */
int     clkdiff;		/* deferred clock ticks			*/
int     slnempty;		/* FALSE if the sleep queue is empty	*/
int     *sltop;			/* address of key part of top entry in	*/
				/* the sleep queue if slnonempty==TRUE	*/
int     clockq;			/* head of queue of sleeping processes  */
int	preempt;		/* preemption counter.	Current process */
				/* is preempted when it reaches zero;	*/
int 	count_per_tenth;
int 	usecs_per_intr;		/* interrupt occurs every usecs_per_int	*/
				/* microseconds	- set in clkinit()	*/
				/* Must be a factor of USECS_PER_SEC	*/
unsigned long	realtime;	/* 1/100 second counter since boot time	*/

#ifdef	RTCLOCK
				/* set in resched; counts in ticks	*/
int	clkruns;		/* set TRUE iff clock exists by setclkr	*/
#else
int	clkruns = FALSE;	/* no clock configured; be sure sleep	*/
#endif				/*   doesn't wait forever		*/

/*
 *------------------------------------------------------------------------
 * clkinit - initialize the clock and sleep queue (called at startup)
 *------------------------------------------------------------------------
 */
clkinit()
{
	int encoded_limit;
	int clkint();
	struct counterregs *timer_chip;
	int intr_per_sec;

#ifdef DEBUG
kprintf("Before calling set_evec\n");
#endif

	set_evec((int) C14VECTOR, clkint);

#ifdef DEBUG
kprintf("After call to set_evec\n");
#endif

	/* set the 1/100 second counter to zero */
	realtime = 0;
	
	/* setclkr(); */
	preempt = QUANTUM;		/* initial time quantum		*/
	count10 = 10;			/* 10ths of a sec. counter	*/
	clmutex = screate(1);		/* semaphore for tod clock	*/
	clktime = 0L;			/* initially a low number	*/
	slnempty = FALSE;		/* initially, no process asleep	*/
	clkdiff = 0;			/* zero deferred ticks		*/
	defclk = 0;			/* clock is not deferred	*/
	clkruns = 1;
	clockq = newqueue();

#ifdef DEBUG
kprintf("done creating semaphores, queues etc\n");
#endif

	intr_per_sec = 100;		/* interrupts per second	*/
	usecs_per_intr = USECS_PER_SEC/intr_per_sec;
					/* usecs between interrupts	*/
	count_per_tenth = intr_per_sec/10;

#ifdef DEBUG
kprintf("Now about to write to the interrupt base address 0x%08x\n",
	INTERRUPT_REG);
#endif
			

	/* now intialize the chip */
	*((unsigned char *) INTERRUPT_REG) &=
	    ~(IR_ENA_CLK14 | IR_ENA_CLK10); /* Unhang flops */

	timer_chip = (struct counterregs *)((struct intersil7170 *) CLOCK_REG);
#ifdef DEBUG
kprintf("Now about to write to the chip control register at 0x%08x\n",
	timer_chip);
#endif
#ifdef SPARC_SLC
	encoded_limit = (usecs_per_intr << CTR_USEC_SHIFT) | CTR_LIMIT_BIT;
#endif
#ifdef SPARC_ELC
	encoded_limit = ((usecs_per_intr) << CTR_USEC_SHIFT) & CTR_USEC_MASK;
#endif
	timer_chip->limit14 = encoded_limit; /* set the countdown	*/
	timer_chip->limit10 = encoded_limit; /* set the countdown	*/
	
	/* enable level 14 interrupts */
        *((unsigned char *) INTERRUPT_REG) |=  IR_ENA_CLK14;
	/* enable level 10 interrupts */
/*        *((unsigned char *) INTERRUPT_REG) |=  IR_ENA_CLK10;*/
	
	
}



/*
 * dog_timeout -- called when the watchdog timer determines that
 * interrupts have been disabled for too long
 */
dog_timeout()
{
    STATWORD ps;

    disable(ps);
    kprintf("\n\nWatchdog timeout!! -- interrupts disabled too long.\n");
    ret_mon();
    restore(ps);
    return;
}
