/* nqalloc.c - nqalloc */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  nqalloc  -  allocate a network demultiplexing queue
 *------------------------------------------------------------------------
 */
nqalloc()
{
	STATWORD ps;    
	int	i;
	struct	upq	*nqptr;

	disable(ps);
	for (i=0 ; i<UPPS ; i++) {
		nqptr = &upqs[i];
		if (!nqptr->up_valid) {
			nqptr->up_valid = TRUE;
			nqptr->up_port = -1;
			nqptr->up_pid = BADPID;
			restore(ps);
			return(i);
		}
	}
	restore(ps);
	return(SYSERR);
}
