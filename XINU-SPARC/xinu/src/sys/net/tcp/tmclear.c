/* tmclear.c - tmclear */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>
#include <clock.h>
#include <tcptimer.h>

/*------------------------------------------------------------------------
 *  tmclear -  clear the indicated timer, return time elapsed (1/100 sec)
 *------------------------------------------------------------------------
 */
int tmclear(port, msg)
     int	port, msg;
{
    struct	tqent	*prev, *ptq;
    int		timespent;
    
    wait(tqmutex);
    prev = 0;
    for (ptq = tqhead; ptq != (struct tqent *)NULL; ptq = ptq->tq_next) {
	if (ptq->tq_port == port && ptq->tq_msg == msg) {
	    timespent = realtime - ptq->tq_time;
	    if (prev)
		prev->tq_next = ptq->tq_next;
	    else
		tqhead = ptq->tq_next;
	    if (ptq->tq_next)
		ptq->tq_next->tq_timeleft += ptq->tq_timeleft;
	    signal(tqmutex);
	    freemem(ptq, sizeof(struct tqent));
	    return timespent;
	}
	prev = ptq;
    }
    signal(tqmutex);
    return SYSERR;
}
