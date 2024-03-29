/* ripsend.c - ripsend */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 * ripsend - send a RIP update
 *------------------------------------------------------------------------
 */
int ripsend(gw, port)
     IPaddr		gw;	/* remote gateway (FFFFFFFF => all)	*/
     unsigned int	port;	/* remote port				*/
{
    struct	rq	*prq, rqinfo[NIF];	/* here, so reentrant	*/
    struct	route	*prt;
    int		i, pn;
    
    if (ripifset(rqinfo, gw, port) != OK)
	return SYSERR;
    
    wait(Route.ri_mutex);
    for (i=0; i<RT_TSIZE; ++i)
	for (prt=rttable[i]; prt; prt=prt->rt_next)
	    ripadd(rqinfo, prt);
    if (Route.ri_default)
	ripadd(rqinfo, Route.ri_default);
    signal(Route.ri_mutex);
    
    for (i=0; i<Net.nif; ++i)
	if (rqinfo[i].rq_active) {
	    prq = &rqinfo[i];
	    for (pn=0; pn<=prq->rq_cur; ++pn)
		udpsend(prq->rq_ip, prq->rq_port, UP_RIP, prq->rq_pep[pn],
			prq->rq_len[pn], TRUE);
	}
    return OK;
}
