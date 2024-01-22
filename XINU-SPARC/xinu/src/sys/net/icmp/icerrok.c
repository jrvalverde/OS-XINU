/* icerrok.c - icerrok */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*#define DEBUG*/

/*------------------------------------------------------------------------
 *  icerrok -  is it ok to send an error response?
 *------------------------------------------------------------------------
 */
Bool icerrok(pep)
     struct	ep	*pep;
{
    struct	ip	*pip = (struct ip *)pep->ep_data;
    struct	icmp	*pic = (struct icmp *)pip->ip_data;
    
    /* don't send errors about error packets... */
    
    if (pip->ip_proto == IPT_ICMP)
	switch(pic->ic_type) {
	  case ICT_DESTUR:
	  case ICT_REDIRECT:
	  case ICT_SRCQ:
	  case ICT_TIMEX:
	  case ICT_PARAMP:
#ifdef DEBUG
	    kprintf("icerrok: can't send ICMP for ICMP packet\n");
#endif
	    return FALSE;
	  default:
	    break;
	}
    /* ...or other than the first of a fragment */
    
    if (pip->ip_fragoff & IP_FRAGOFF) {
#ifdef DEBUG
	kprintf("icerrok: can't send ICMP for a fragment packet\n");
#endif
	return FALSE;
    }
    /* ...or broadcast packets */
    
    if (isbrc(pip->ip_dst)) {
#ifdef DEBUG
	{
	    char junk[80];
	    
	    kprintf("icerrok: dst ip addr (%s) is a bcast\n",
		    ip2dot(junk, pip->ip_dst));
	}
#endif
	return FALSE;
    }
    return TRUE;
}
