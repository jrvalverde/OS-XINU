/* rtdel.c - rtdel */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  rtdel  -  delete the route with the given net, mask
 *------------------------------------------------------------------------
 */
int rtdel(net, mask)
IPaddr	net, mask;		/* destination network and mask		*/
{
	struct	route	*prt, *prev;
	int		hv;

	wait(Route.ri_mutex);
	if (Route.ri_default &&
	    IP_SAMEADDR(net, Route.ri_default->rt_net)) {
		RTFREE(Route.ri_default);
		Route.ri_default = 0;
		signal(Route.ri_mutex);
		return OK;
	}
	hv = rthash(net);

	prev = (struct route *)NULL;
	for (prt = rttable[hv]; prt; prt = prt->rt_next) {
		if (IP_SAMEADDR(net, prt->rt_net) &&
		    IP_SAMEADDR(mask, prt->rt_mask))
			break;
		prev = prt;
	}
	if (prt == (struct route *)NULL) {
		signal(Route.ri_mutex);
		return SYSERR;
	}
	if (prev)
		prev->rt_next = prt->rt_next;
	else
		rttable[hv] = prt->rt_next;
	RTFREE(prt);
	signal(Route.ri_mutex);
	return OK;
}
