/* tmleft.c - tmleft */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>
#include <tcptimer.h>

/*------------------------------------------------------------------------
 *  tmleft -  how much time left for this timer? (in 1/100 sec)
 *------------------------------------------------------------------------
 */
int tmleft(port, msg)
     int	port, msg;
{
    struct	tqent	*tq;
    int		timeleft = 0;
    
    if (tqhead == (struct tqent *)NULL)
	return 0;
    
    wait(tqmutex);
    for (tq = tqhead; tq != (struct tqent *)NULL; tq = tq->tq_next) {
	timeleft += tq->tq_timeleft;
	if (tq->tq_port == port && tq->tq_msg == msg) {
	    signal(tqmutex);
	    return timeleft;
	}
    }
    signal(tqmutex);
    return 0;
}
