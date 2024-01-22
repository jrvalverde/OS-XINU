/* arpfind.c - arpfind */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 * arpfind - find an ARP entry given a protocol address and interface
 *------------------------------------------------------------------------
 */
struct arpentry *arpfind(pra, prtype, pni)
char		*pra;
int		prtype;
struct netif	*pni;
{
	struct arpentry	*pae;
	int		i;

	for (i=0; i<ARP_TSIZE; ++i) {
		pae = &arptable[i];
		if (pae->ae_state == AS_FREE)
			continue;
		if (pae->ae_prtype == prtype &&
		    pae->ae_pni == pni &&
		    blkequ(pae->ae_pra, pra, pae->ae_prlen))
			return pae;
	}
	return 0;
}
