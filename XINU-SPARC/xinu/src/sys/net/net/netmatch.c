/* netmatch.c - netmatch */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  netmatch  -  Is "dst" on "net"?
 *------------------------------------------------------------------------
 */
Bool netmatch(dst, net, mask, islocal)
IPaddr	dst, net, mask;
Bool	islocal;
{
	int	i;

	if (((*(int *) mask) & (*(int *) dst)) != (*(int *) net))
	    return FALSE;

	/*
	 * local srcs should not match broadcast addresses (host routes)
	 */
	if (islocal && isbrc(dst))
	    return !IP_SAMEADDR(mask, ip_maskall);
	return TRUE;
}
