/* stlookup.c - stlookup */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  stlookup  -  get station struct for a given <ifn,address>
 *------------------------------------------------------------------------
 */
struct station *
stlookup(ifnum, ea)
int	ifnum;
Eaddr	ea;
{
	struct station	*pst;

	return 0;
}
