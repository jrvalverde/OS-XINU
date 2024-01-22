/* ipgetp.c - ipgetp */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*#define CHECK_FOR_ERRORS*/
/*#define BUF_DEBUG*/

static	int	ifnext = NI_LOCAL;

/*------------------------------------------------------------------------
 * ipgetp  --  choose next IP input queue and extract a packet
 *------------------------------------------------------------------------
 */
struct ep *ipgetp(pifnum)
     int	*pifnum;
{
    struct	ep	*pep;
    int		i;
    
    recvclr();	/* make sure no old messages are waiting */
    while (TRUE) {
	for (i=0; i < Net.nif; ++i, ++ifnext) {
	    if (ifnext >= Net.nif)
		ifnext = 0;
	    if (nif[ifnext].ni_state == NIS_DOWN)
		continue;
	    if (pep = NIGET(ifnext)) {
		*pifnum = ifnext;
#ifdef CHECK_FOR_ERRORS
		if ((ifnext < 0) || (ifnext >= Net.nif)) {
		    kprintf("ipgetp: *pifnum is illegal: %d\n",
			    ifnext);
		    panic("IPGETP: bad ifnum");
		}
#endif
#ifdef BUF_DEBUG
		if (bpwhichpool(((int *)pep) - 1 ) == SYSERR) {
		    kprintf("PROCESS ipgetp(%d): invalid packet 0x%08x\n",
			    ifnext,pep);
		    panic("IPGETP: bad packet");
		}
#endif
		return pep;
	    }
	}
	ifnext = receive();	
#ifdef CHECK_FOR_ERRORS
	if ((ifnext < 0) || (ifnext >= Net.nif)) {
	    kprintf("ipgetp: received illegal nif: %d\n", ifnext);
	    ifnext = NI_LOCAL;
	}
#endif
    }
    /* can't reach here */
}
