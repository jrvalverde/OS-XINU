/* hgarpdel.c - hgarpdel */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <igmp.h>

/*------------------------------------------------------------------------
 *  hgarpdel  -  remove an ARP table entry for a multicast address
 *------------------------------------------------------------------------
 */
int hgarpdel(ifnum, ipa)
int	ifnum;
IPaddr	ipa;
{
	struct netif	*pni = &nif[ifnum];
	struct arpentry	*pae, *arpfind();
	int		ifdev = nif[ifnum].ni_dev;
	STATWORD	ps;

	disable(ps);
	if (pae = arpfind(&ipa, EPT_IP, pni))
		pae->ae_state = AS_FREE;
	if (pni->ni_mcast)
		(pni->ni_mcast)(NI_MDEL, ifdev, pae->ae_hwa, ipa);
	restore(ps);
	return OK;
}
