/* rthash.c - rthash */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  rthash  -  compute the hash for "net"
 *------------------------------------------------------------------------
 */
int rthash(net)
IPaddr	net;
{
	int	bc;	/* # bytes to count	*/
	int	hv;	/* hash value		*/

	hv = 0;
	if (IP_CLASSA(net)) bc = 1;
	else if (IP_CLASSB(net)) bc = 2;
	else if (IP_CLASSC(net)) bc = 3;
	else if (IP_CLASSD(net)) bc = 4;
	else if (IP_CLASSE(net)) bc = 4;
	while (bc--)
		hv += net[bc] & 0xff;
	return hv % RT_TSIZE;
}
