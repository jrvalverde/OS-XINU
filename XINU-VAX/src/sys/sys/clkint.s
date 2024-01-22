/* clkint.s -  clkint */

#include <procreg.h>
#include <conf.h>

/*------------------------------------------------------------------------
 * clkint  --  real-time clock interrupt service routine 
 *------------------------------------------------------------------------
*/

					/* define constants		*/
#define	CLEARINT 0xc1			/* value to clear ICCS INT bit	*/

	.text
	.align	2			/* align on longword boundary	*/
	.globl	_clkint
_clkint:
	mtpr	$DISABLE, $IPL		/* disable interrupts		*/
#if !defined(UVAX)
	mtpr	$CLEARINT, $ICCS	/* clr interrupt bit in interval*/
					/* clock control reg 		*/
#else 					/* Microvax has a 10ms rt clock	*/
	decl	_count10		/* Is this the 10th interrupt?	*/
	jnequ	clret			/*  no => return		*/
	movl	$10, _count10		/*  yes=> reset counter&continue*/
#endif
	decl	_tickctr		/* Is this the 10th tick?	*/
	jnequ	clckdef			/*  no => process tick		*/
	movl	$10, _tickctr		/*  yes=> reset counter&continue*/
	incl	_clktime		/* increment time-of-day clock	*/
clckdef:
	tstl	_defclk			/* Are clock ticks deferred?	*/
	jeqlu	notdef			/*  no => go process this tick	*/
	incl	_clkdiff		/*  yes=> count in clkdiff and	*/
	rei				/*	  return quickly	*/
notdef:
	tstl	_slnempty		/* Is sleep queue nonempty?	*/
	jeqlu	clpreem			/* no => go process preemption	*/
	decw	*_sltop			/* yes=> decrement delta key	*/
	jnequ	clpreem			/* 	 on first process,	*/
					/* 	 calling wakeup if 0	*/
	pushr	$LOWREGS		/* save low registers		*/
	calls	$0, _wakeup		/* wakeup sleeping process	*/
	popr	$LOWREGS		/* restore low registers	*/
clpreem:
	decl	_preempt		/* decrement preemption counter	*/
	jnequ	clret			/* and call resched if it is 0	*/
	pushr	$LOWREGS		/* save low registers		*/
	calls	$0, _resched		/* reschedule			*/
	popr	$LOWREGS		/* restore low registers	*/
clret:
	rei				/* restore pre-interrupt IPL &	*/
					/* return from interrupt	*/
