/* ethmatch.c - ethmatch */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 * ethmatch - determine if an Ethernet address is one of ours
 *------------------------------------------------------------------------
 */
Bool
ethmatch(pni, pea)
struct netif	*pni;
Eaddr		pea;
{
	int	i;

	if (pni->ni_state != NIS_UP)
		return FALSE;
	if (blkequ(pni->ni_hwa.ha_addr, pea, EP_ALEN))
		return TRUE;
	if (blkequ(pni->ni_hwb.ha_addr, pea, EP_ALEN))
		return TRUE;
#ifdef	MULTICAST
/* check multicast addresses here */
#endif	/* MULTICAST */
	return FALSE;
}
