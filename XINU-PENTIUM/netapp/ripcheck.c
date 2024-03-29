/* ripcheck.c - ripcheck */

#include <conf.h>
#include <kernel.h>
#include <network.h>

#ifdef	RIP
/*------------------------------------------------------------------------
 *  ripcheck  -  check a RIP packet for proper format
 *------------------------------------------------------------------------
 */
int ripcheck(prip, len)
struct	rip	*prip;
int		len;
{
	int	i, j, nrts;

	switch (prip->rip_vers) {
	case	0:			/* never OK		*/
		return SYSERR;
	case	1:			/* more checks below	*/
		break;
	default:			/* >1 always ok		*/
		return OK;
	}
	/* check all "must be zero" fields */

	if (prip->rip_mbz)
		return SYSERR;
	nrts = (len - RIPHSIZE)/sizeof(struct riprt);
	for (i=0; i<nrts; ++i) {
		struct	riprt	*prr = &prip->rip_rts[i];

		if (prr->rr_mbz)
			return SYSERR;
		for (j=IP_ALEN; j<sizeof(prr->rr_addr); ++j)
			if (prr->rr_addr[j])
				return SYSERR;
	}
	return OK;	/* this one's ok in my book... */
}
#endif	/* RIP */
