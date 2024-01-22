/* icmp_in.c - icmp_in */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <proc.h>

/*------------------------------------------------------------------------
 *  icmp_in  -  handle ICMP packet coming in from the network
 *------------------------------------------------------------------------
 */
int icmp_in(pni, pep)
     struct	netif	*pni;		/* not used */
     struct	ep	*pep;
{
    struct	ip	*pip;
    struct	icmp	*pic;
    int		i, len;
    
    pip = (struct ip *)pep->ep_data;
    pic = (struct icmp *) pip->ip_data;
    
#ifdef DEBUG
    kprintf("icmp_in(pni, 0x%08x) called, type: %d\n",
	    pep, (unsigned int) pic->ic_type);
#endif
    len = pip->ip_len - IP_HLEN(pip);
    if (cksum(pic, len>>1)) {
	IcmpInErrors++;
	freebuf(pep);
	return SYSERR;
    }
    IcmpInMsgs++;
    switch(pic->ic_type) {
      case ICT_ECHORQ:
	IcmpInEchos++;
	return icmp(ICT_ECHORP, 0, pip->ip_src, pep, 0);
      case ICT_MASKRQ:
	IcmpInAddrMasks++;
	if (!gateway) {
	    freebuf(pep);
	    return OK;
	}
	pic->ic_type = (char) ICT_MASKRP;
	netmask(pic->ic_data, pip->ip_dst);
	break;
      case ICT_MASKRP:
	IcmpInAddrMaskReps++;
	for (i=0; i<Net.nif; ++i)
	    if (IP_SAMEADDR(nif[i].ni_ip, pip->ip_dst))
		break;
	if (i != Net.nif && (nif[i].ni_svalid != 2)) {
	    setmask(i, pic->ic_data);
	    nif[i].ni_svalid = 2;	/* only the first reply counts */
#ifdef DEBUG
	    kprintf("icmp_in: sending message to proc %d (%s)\n",
		    pic->ic_id, proctab[pic->ic_id].pname);
#endif
	    icsendproc(pic->ic_id, ICT_MASKRP);
	}
	freebuf(pep);
	return OK;
      case ICT_ECHORP:
	IcmpInEchoReps++;
	if (icsendproc(pic->ic_id, pep) != OK)
	    freebuf(pep);
	return OK;
      case ICT_REDIRECT:
	IcmpInRedirects++;
	icredirect(pep);
	return OK;
      default:
	switch(pic->ic_type) {
	  case ICT_DESTUR:	IcmpInDestUnreachs++;	break;
	  case ICT_SRCQ:	IcmpInSrcQuenchs++;	break;
	  case ICT_TIMEX:	IcmpInTimeExcds++;	break;
	  case ICT_PARAMP:	IcmpInParmProbs++;	break;
	  case ICT_TIMERQ:	IcmpInTimestamps++;	break;
	  case ICT_TIMERP:	IcmpInTimestampReps++;	break;
	  default:		IcmpInErrors++;		break;
	}
	freebuf(pep);
	return OK;
    }
    icsetsrc(pip);
    
    len = pip->ip_len - IP_HLEN(pip);
    
    pic->ic_cksum = 0;
    pic->ic_cksum = cksum(pic, len>>1);
    
    IcmpOutMsgs++;
    ipsend(pip->ip_dst, pep, len);
    return OK;
}
