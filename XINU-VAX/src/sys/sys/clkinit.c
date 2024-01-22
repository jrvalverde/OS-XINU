/* clkinit.c - clkinit */

#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include <procreg.h>

/* real-time clock variables and sleeping process queue pointers	*/

#ifdef	RTCLOCK
int	clmutex;		/* mutual exclusion for time-of-day	*/
long	clktime;		/* current time in seconds since 1/1/70	*/
int	tickctr;		/* counts 10ths of a second 10-0	*/
int     defclk;			/* non-zero, then deferring clock count */
int     clkdiff;		/* deferred clock ticks			*/
int     slnempty;		/* FALSE if the sleep queue is empty	*/
short   *sltop;			/* address of key part of top entry in	*/
				/* the sleep queue if slnonempty==TRUE	*/
int     clockq;			/* head of queue of sleeping processes  */
int	preempt;		/* preemption counter.	Current process */
				/* is preempted when it reaches zero;	*/
				/* set in resched; counts in ticks	*/
int	clkruns = TRUE;		/* set TRUE iff clock exists 		*/
#else
int	clkruns = FALSE;	/* no clock configured; be sure sleep	*/
#endif				/*   doesn't wait forever		*/
#if defined(UVAX)
int	count10;		/* counts 100ths of a second 10-0	*/
#endif

/*
 *------------------------------------------------------------------------
 * clkinit - initialize the clock and sleep queue (called at startup)
 *------------------------------------------------------------------------
 */
clkinit()
{
	*CVECTOR = (int) clkint;	/* call clkint on interrupt	*/
	clmutex = screate(1);		/* semaphore for tod clock	*/
	clktime = 0L;			/* initially a low number	*/
	preempt = QUANTUM;		/* initial time quantum		*/
	slnempty = FALSE;		/* initially, no process asleep	*/
	clkdiff = 0;			/* zero deferred ticks		*/
	defclk = 0;			/* clock is not deferred	*/
	clockq = newqueue();		/* allocate clock queue in q	*/
	tickctr = 10;			/* 10ths of a second counter	*/
#if defined(UVAX)
	count10 = 10;			/* 100ths of a sec. counter	*/
	mtpr(STRTUCLK, ICCS);		/* start real-time clock	*/
#else
	mtpr(-CLKINTVL, NICR);		/* set interval to 0.1 sec	*/
	mtpr(STRTCLK, ICCS);		/* start interval timer,	*/
					/* enable clock to interrupt	*/
#endif
}
