/* tqdump.c - tqdump, tqwrite */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <tcptimer.h>

/*------------------------------------------------------------------------
 *  tqdump -  dump a timer queue (for debugging)
 *------------------------------------------------------------------------
 */
int tqdump()
{
	struct	tqent	*tq;

	for (tq=tqhead; tq; tq=tq->tq_next) {
		kprintf("tq <D %d,T %d, (%d, %d)>\n", tq->tq_timeleft,
			tq->tq_time, TCB(tq->tq_msg), EVENT(tq->tq_msg));
	}
}

/*------------------------------------------------------------------------
 *  tqwrite - write the timer queue to the given device
 *------------------------------------------------------------------------
 */
tqwrite(stdout)
int	stdout;
{
	struct	tqent	*tq;
	int		timeleft;
	char		str[80];

	if (tqhead == 0) {
		write(stdout, "no entries\n", 11);
		return OK;
	}
	write(stdout, "timeleft\ttime\tport\tportlen\tmsg\n", 31);
	timeleft = 0;
	for (tq=tqhead; tq; tq=tq->tq_next) {
		timeleft += tq->tq_timeleft;
		sprintf(str, "%-8d\t%-5d\t%-4d\t%-7d\t%-3d\n",
			timeleft, tq->tq_time, tq->tq_port,
			tq->tq_portlen, tq->tq_msg);
		write(stdout, str, strlen(str));
	}
	return OK;
}
