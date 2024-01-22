/* ipfjoin.c - ipfjoin */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>


#define PRINTERRORS
/*#define DEBUG*/


struct	ep	*ipfcons();

/*------------------------------------------------------------------------
 *  ipfjoin  -  join fragments, if all collected
 *------------------------------------------------------------------------
 */
struct ep *ipfjoin(iq)
struct	ipfq	*iq;
{
	struct	ep	*pep;
	struct	ip 	*pip;
	int		off, packoff;

#ifdef DEBUG
	kprintf("ipfjoin() called\n");
#endif

	if (iq->ipf_state == IPFF_BOGUS)
		return 0;
	/* see if we have the whole datagram */

	off = 0;
	while (pep=(struct ep *)seeq(iq->ipf_q)) {
		pip = (struct ip *)pep->ep_data;
		packoff =  (pip->ip_fragoff & IP_FRAGOFF)<<3;
		if (off < packoff) {
			while(seeq(iq->ipf_q))
				/*empty*/;
			return 0;
		}
		off = packoff + pip->ip_len - IP_HLEN(pip);
	}
	if (off > MAXLRGBUF) {		/* too big for us to handle */
#if defined(PRINTERRORS) || defined(DEBUG)
		kprintf("ipfjoin: packet too large\n");
#endif
		while (pep = (struct ep *)deq(iq->ipf_q))
			freebuf(pep);
		freeq(iq->ipf_q);
		iq->ipf_state = IPFF_FREE;
		return 0;
	}
	if ((pip->ip_fragoff & IP_MF) == 0)
		return ipfcons(iq);

	return 0;
}
