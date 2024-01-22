/* netmask.c - netmask */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  netmask  -  set the default mask for the given net
 *------------------------------------------------------------------------
 */
int netmask(mask, net)
     IPaddr	mask;
     IPaddr	net;
{
	IPaddr	netpart;
	Bool	isdefault = TRUE;
	int	i, bc;
	
	for (i=0; i<IP_ALEN; ++i) {
		mask[i] = ~0;
		isdefault &= net[i] == 0;
	}
	if (isdefault) {
		IP_COPYADDR(mask, net);
		return OK;
	}
	/* check for net match (for subnets) */
	
	netnum(netpart, net);
	for (i=0; i<Net.nif; ++i) {
		if (nif[i].ni_svalid && nif[i].ni_ivalid &&
		    IP_SAMEADDR(nif[i].ni_net, netpart)) {
			IP_COPYADDR(mask, nif[i].ni_mask);
			return OK;
		}
	}
	if (IP_CLASSA(net)) bc = 1;
	if (IP_CLASSB(net)) bc = 2;
	if (IP_CLASSC(net)) bc = 3;
	if (IP_CLASSD(net)) bc = 4;
	if (IP_CLASSE(net)) bc = 4;
	for (; bc < IP_ALEN; ++bc)
	    mask[bc] = 0;
	return OK;
}
