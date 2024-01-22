/* uqinit.c - uqinit */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>

/*------------------------------------------------------------------------
 *  uqinit -  initialize TCP urgent queue data
 *------------------------------------------------------------------------
 */
int uqinit()
{
	int	i;

	uqidone = TRUE;
	uqmutex = screate(0);
	for (i=0; i<UQTSIZE; ++i)
		uqtab[i].uq_state = UQS_FREE;
	signal(uqmutex);
	return OK;
}
