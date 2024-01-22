/* nm_init.c - nm_init */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <netmon.h>

/*------------------------------------------------------------------------
 * nm_init - initialize network monitor data structures
 *------------------------------------------------------------------------
 */
int
nm_init()
{
	Netmon.nm_mutex = screate(1);
	Netmon.nm_pool = mkpool(sizeof(struct station), ST_BPSIZE);
}

struct netmon Netmon;
