/* snfreebl.c - snfreebl */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <snmp.h>

#define DEBUG

/*------------------------------------------------------------------------
 * snfreebl - free memory used for ASN.1 strings and snbentry nodes
 *------------------------------------------------------------------------
 */
snfreebl(bl)
struct snbentry	*bl;
{
	register struct snbentry	*nbl;

	if (bl == 0)
		return;
	for (; bl; bl = nbl) {
	    	nbl = bl->sb_next;
		if ( bl->sb_a1slen != 0 ) {
		    freemem(bl->sb_a1str, bl->sb_a1slen);
		}
		freemem(bl, sizeof(struct snbentry));
	}
}
