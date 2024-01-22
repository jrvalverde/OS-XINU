/* tcptimer.c - tcptimer */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>
#include <clock.h>
#include <tcptimer.h>

int	tqmutex;
int	tqpid;
struct	tqent	*tqhead = (struct tqent *)NULL;

/*------------------------------------------------------------------------
 *  tcptimer -  TCP timer process
 *------------------------------------------------------------------------
 */
PROCESS tcptimer()
{
    long	now, lastrun;	/* times from system clock	*/
    int	        delta;		/* time since last iter. (1/100 sec)*/
    struct	tqent	*tq;	/* temporary delta list ptr	*/
    
    lastrun = realtime;		/* initialize to "now"		*/
    tqmutex = screate(1);	/* mutual exclusion semaphore	*/
    tqpid = getpid();		/* record timer process id	*/
    signal(Net.sema);		/* start other network processes*/
    
    while (TRUE) {
	sleep10(TIMERGRAN);	/* real-time delay		*/
	/* block timer process if delta	list is empty		*/
	if (tqhead == (struct tqent *)NULL) 
	    suspend(tqpid);
	
	wait(tqmutex);
	now = realtime;
	delta = now - lastrun;	/* compute elapsed time		*/
	
	/* Note: check for possible clock reset (time moved	*/
	/* backward or delay was over an order of magnitude too	*/
	/* long)						*/
	
	if (delta < 0 || delta > TIMERGRAN*1000)
	    delta = 0;		/* estimate the delay	*/
	
	lastrun = now;
	while (tqhead != 0 && tqhead->tq_timeleft <= delta) {
	    delta -= tqhead->tq_timeleft;
	    if (pcount(tqhead->tq_port) <= tqhead->tq_portlen)
		psend(tqhead->tq_port, tqhead->tq_msg);
	    tq = tqhead;
	    tqhead = tqhead->tq_next;
	    freemem(tq, sizeof(struct tqent));
	}
	
	if (tqhead)
	    tqhead->tq_timeleft -= delta;
	signal(tqmutex);
    }
}
