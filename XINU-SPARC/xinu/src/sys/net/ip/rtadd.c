/* rtadd.c - rtadd */

#include <conf.h>
#include <kernel.h>
#include <network.h>

struct	route *rtnew();

/*#define DEBUG*/

/*------------------------------------------------------------------------
 *  rtadd  -  add a route to the routing table
 *------------------------------------------------------------------------
 */
int rtadd(net, mask, gw, metric, intf, ttl)
IPaddr	net, mask, gw;
int	metric, intf, ttl;
{
	struct	route	*prt, *srt, *prev;
	Bool		isdup;
	int		hv, i, j;

#ifdef DEBUG
	char buf1[100],buf2[100],buf3[100];
	kprintf("rtadd(net:%s, mask:%s, gw:%s, metric:%d, intf:%d, ttl:%d)\n",
		ip2dot(buf1,net),
		ip2dot(buf2,mask),
		ip2dot(buf3,gw),
		metric,intf,ttl);
#endif

	prt = rtnew(net, mask, gw, metric, intf, ttl);
	if (prt == (struct route *)SYSERR)
		return SYSERR;

	/* compute the queue sort key for this route */
	for (prt->rt_key = 0, i=0; i<IP_ALEN; ++i)
		for (j=0; j<8; ++j)
			prt->rt_key += (mask[i] >> j) & 1;
	wait(Route.ri_mutex);

	/* special case for default routes */
	if (IP_SAMEADDR(net, RT_DEFAULT)) {
		if (Route.ri_default &&
		    metric > Route.ri_default->rt_metric) {
			/* metric is greater so ignore it. */
			RTFREE(prt);
			signal(Route.ri_mutex);
			return OK;
		}
		if (Route.ri_default)
		    RTFREE(Route.ri_default);
		Route.ri_default = prt;
		signal(Route.ri_mutex);
		return OK;
	}
	prev = (struct route *)NULL;
	hv = rthash(net);
	isdup = FALSE;
	for (srt=rttable[hv]; srt; srt = srt->rt_next) {
		if (prt->rt_key > srt->rt_key)
			break;
		if (IP_SAMEADDR(srt->rt_net, prt->rt_net) &&
	    	    IP_SAMEADDR(srt->rt_mask, prt->rt_mask)) {
			isdup = TRUE;
			break;
		}
		prev = srt;
	}
	if (isdup) {
		struct	route	*tmprt;

		if (IP_SAMEADDR(srt->rt_gw, prt->rt_gw)) {
			/* just update the existing route */
			if (dorip) {
				srt->rt_ttl = ttl;
				if (srt->rt_metric != metric) {
					if (metric == RTM_INF)
						srt->rt_ttl = RIPZTIME;
					send(rippid, 0);
				}
			}
			srt->rt_metric = metric;
			RTFREE(prt);
			signal(Route.ri_mutex);
			return OK;
		}
		/* else, someone else has a route there... */
		if (srt->rt_metric <= prt->rt_metric) {
			/* no better off to change; drop the new one */

			RTFREE(prt);
			signal(Route.ri_mutex);
			return OK;
		} else if (dorip)
			send(rippid, 0);
		tmprt = srt;
		srt = srt->rt_next;
		RTFREE(tmprt);
	} else if (dorip)
		send(rippid, 0);
	prt->rt_next = srt;
	if (prev)
		prev->rt_next = prt;
	else
		rttable[hv] = prt;
	signal(Route.ri_mutex);
	return OK;
}
