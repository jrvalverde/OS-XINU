/* nm_in.c - nm_in */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  nm_in - network monitor input function
 *------------------------------------------------------------------------
 */
int nm_in(pni, pep, len)
struct	netif	*pni;		/* the interface	*/
struct	ep	*pep;		/* the packet		*/
int		len;		/* length, in octets	*/
{
	int	ifnum = pni - &nif[0];
	int	rv;

	ipdump(pep->ep_data, 0);
	return OK;
}
