/* ip2dot.c - ip2dot */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  ip2dot - create the dotted decimal form of ip in pdot
 *------------------------------------------------------------------------
 */
char *ip2dot(pdot, ip)
char	*pdot;
IPaddr	ip;
{
	char	*pch = pdot;
	int	i;

	for (i=0; i<IP_ALEN; ++i) {
		sprintf(pch, i==0 ? "%d" : ".%d", ip[i] & 0xff);
		pch += strlen(pch);
	}
	*pch = NULLCH;
	return pdot;
}
