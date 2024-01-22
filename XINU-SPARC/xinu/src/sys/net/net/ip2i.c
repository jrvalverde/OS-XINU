/* ip2i.c - ip2i */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  ip2i - return an integer representation of an IP address
 *------------------------------------------------------------------------
 */
ip2i(addr)
     IPaddr addr;
{
	unsigned i;

	i = ((unsigned) addr[0] << 24) |
	    ((unsigned) addr[1] << 16) |
	    ((unsigned) addr[2] <<  8) |
	    ((unsigned) addr[3]);

	return((int) i);
}
