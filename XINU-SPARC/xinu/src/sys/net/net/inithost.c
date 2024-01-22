/* inithost.c - inithost */

#include <conf.h>
#include <kernel.h>
#include <network.h>


static	int	itod[] = { -1, ETHER };

/*------------------------------------------------------------------------
 * inithost  --  initialize a host's interface structures
 *------------------------------------------------------------------------
 */
int inithost()
{
    int	i;
    
    Net.sema = screate(1);
    Net.nif = NIF;
    
    for (i=0; i<Net.nif; ++i) {
	/* start with everything 0 */
	bzero(&nif[i], sizeof(nif[i]));
	nif[i].ni_state = NIS_DOWN;
	nif[i].ni_admstate = NIS_UP;
	nif[i].ni_ivalid = nif[i].ni_nvalid = FALSE;
	nif[i].ni_svalid = FALSE;
	nif[i].ni_dev = itod[i];
	if (i == NI_LOCAL) {
	    /* maxbuf - ether header - CRC - nexthop */
	    nif[i].ni_mtu = MAXLRGBUF-EP_HLEN-EP_CRC-IP_ALEN;
	    IP_COPYADDR(nif[i].ni_ip, ip_anyaddr);
	    continue;
	}
	switch(nif[i].ni_dev) {
	  case ETHER:
	    efaceinit(i);
	    nif[i].ni_state = NIS_UP;
	    break;
	};
    }
    
    return OK;
}
