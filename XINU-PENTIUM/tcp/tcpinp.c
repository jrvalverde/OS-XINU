/* tcpinp.c - tcpinp */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  tcpinp  -  handle TCP segment coming in from IP
 *------------------------------------------------------------------------
 */
PROCESS tcpinp()
{
	struct	ep	*pep;
	struct	ip	*pip;
	struct	tcp	*ptcp;
	struct	tcb	*ptcb, *tcpdemux();

	tcps_iport = pcreate(TCPQLEN);
	signal(Net.sema);
	while (TRUE) {
		pep = (struct ep *)preceive(tcps_iport);
		if ((int)pep == SYSERR)
			break;
		pip = (struct ip *)pep->ep_data;
		if (tcpcksum(pep, pip->ip_len - IP_HLEN(pip))) {
#ifdef notnow
kprintf("checksum failed: %d\n", tcpcksum(pep, pip->ip_len - IP_HLEN(pip)));
#endif
			freebuf(pep);
			continue;
		}
		ptcp = (struct tcp *)pip->ip_data;
		tcpnet2h(ptcp); /* convert all fields to host order */
		pep->ep_order |= EPO_TCP;
		ptcb = tcpdemux(pep);
		if (ptcb == 0) {
			tcpreset(pep);
			freebuf(pep);
			continue;
		}
		if (!tcpok(ptcb, pep))
{
kprintf("tcpinp: !OK, acking\n");
			tcpackit(ptcb, pep);
}
		else {
			tcpopts(ptcb, pep);
			tcpswitch[ptcb->tcb_state](ptcb, pep);
		}
		if (ptcb->tcb_state != TCPS_FREE)
			signal(ptcb->tcb_mutex);
		freebuf(pep);
	}
}

int tcps_oport, tcps_iport, tcps_lqsize, tcps_tmutex;
