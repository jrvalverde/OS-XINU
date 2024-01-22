/* isbrc.c - isbrc */

#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  isbrc  -  Is "dest" a broadcast address?
 *------------------------------------------------------------------------
 */
Bool isbrc(dest)
IPaddr	dest;
{
	int	inum;

	/* all 0's and all 1's are broadcast */

	if (IP_SAMEADDR(dest, ip_anyaddr) ||
	    IP_SAMEADDR(dest, ip_maskall))
		return TRUE;

	/* check real broadcast address and BSD-style for net & subnet 	*/

	for (inum=0; inum < Net.nif; ++inum)
		if (IP_SAMEADDR(dest, nif[inum].ni_brc) ||
		    IP_SAMEADDR(dest, nif[inum].ni_nbrc) ||
		    IP_SAMEADDR(dest, nif[inum].ni_subnet) ||
		    IP_SAMEADDR(dest, nif[inum].ni_net))
			return TRUE;

	return FALSE;
}
