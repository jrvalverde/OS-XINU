/* sae_set.c - sae_set */

#include <conf.h>
#include <kernel.h>
#include <network.h>

#ifdef	SNMP

#include <snmp.h>

/*------------------------------------------------------------------------
 * sae_set - return error: the IP Address Entry Table is read-only
 *------------------------------------------------------------------------
 */
sae_set()
{
	return SERR_NO_SUCH;
}
#endif	/* SNMP */
