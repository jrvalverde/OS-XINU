/* tcpsndlen.c - tcpsndlen */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <q.h>

struct	uqe	*uqalloc();

/*------------------------------------------------------------------------
 *  tcpsndlen - compute the packet length and offset in sndbuf
 *------------------------------------------------------------------------
 */
int tcpsndlen(ptcb, rexmt, poff)
struct	tcb	*ptcb;
Bool	rexmt;
int	*poff;
{
	struct 	uqe	*puqe, *puqe2;
	int		datalen;

	if (rexmt || (ptcb->tcb_code & TCPF_SYN))
		*poff = 0;
	else
		*poff = ptcb->tcb_snext - ptcb->tcb_suna;
	datalen = ptcb->tcb_sbcount - *poff;
	datalen = min(datalen, ptcb->tcb_swindow);
	return min(datalen, ptcb->tcb_smss);
}
