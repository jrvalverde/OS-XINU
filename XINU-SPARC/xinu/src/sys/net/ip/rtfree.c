/* rtfree.c - rtfree */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  rtfree  -  remove one reference to a route
 *------------------------------------------------------------------------
 */
int rtfree(prt)
struct	route	*prt;
{
	wait(Route.ri_mutex);
	RTFREE(prt);
	signal(Route.ri_mutex);
	return OK;
}
