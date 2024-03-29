/* rfalloc.c - rfalloc */

#include <conf.h>
#include <kernel.h>
#include <fserver.h>
#include <rfile.h>

/*------------------------------------------------------------------------
 *  rfalloc  --  allocate pseudo device for a remote file; return id
 *------------------------------------------------------------------------
 */
rfalloc()
{
	int	i;
	PStype	ps;

	disable(ps);
	for (i=0 ; i<Nrf ; i++)
		if (Rf.rftab[i].rf_state == RFREE) {
			Rf.rftab[i].rf_state = RUSED;
			restore(ps);
			return(i);
		}
	restore(ps);
	return(SYSERR);
}
