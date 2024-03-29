/* ripadd.c - ripadd */

#include <conf.h>
#include <kernel.h>
#include <network.h>

#ifdef	RIP
/*------------------------------------------------------------------------
 *  ripadd  -  add the given route to the RIP packets yet to send
 *------------------------------------------------------------------------
 */
int ripadd(rqinfo, prt)
struct	rq	rqinfo[];
struct	route	*prt;
{
	IPaddr	net;
	int	i, metric, pn, rn;

	for (i=0; i<Net.nif; ++i) {
		struct	rq	*prq = &rqinfo[i];
		struct	riprt	*rp;

		if (!rqinfo[i].rq_active || nif[i].ni_state != NIS_UP)
			continue;
		metric = ripmetric(prt, i);
		if (metric == SYSERR)
			continue;
		if (prq->rq_nrts >= MAXRIPROUTES &&
		    ripstart(prq) != OK)
			continue;
		pn = prq->rq_cur;
		rn = prq->rq_nrts++;
		rp = &prq->rq_prip->rip_rts[rn];
		rp->rr_family = hs2net(AF_INET);
		rp->rr_mbz = 0;
		net = netnum(prt->rt_net);
		bzero(rp->rr_addr, sizeof(rp->rr_addr));
		if (nif[i].ni_net == net ||
		    prt->rt_mask == ip_maskall) {
			blkcopy(rp->rr_addr, &prt->rt_net, IP_ALEN);
		} else	/* send the net part only (esp. for subnets)	*/
			blkcopy(rp->rr_addr, &net, IP_ALEN);
		rp->rr_metric = hl2net(metric);
		prq->rq_len[pn] += sizeof(struct riprt);
	}
	return OK;
}
#endif	/* RIP */
