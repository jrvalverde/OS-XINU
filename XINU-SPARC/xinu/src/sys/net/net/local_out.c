/* local_out.c - local_out */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>
#include <nettcp.h>

/*#define PRINTERRORS*/
/*#define DEBUG*/

struct ep	*ipreass();

/*------------------------------------------------------------------------
 *  local_out  -  handle an IP datagram headed for a local process
 *------------------------------------------------------------------------
 */
int local_out(pep)
     struct	ep	*pep;
{
    struct	netif	*pni = &nif[NI_LOCAL];
    struct	ip	*pip = (struct ip *)pep->ep_data;
    int		rv;
    
#ifdef BUF_DEBUG
    if (bpwhichpool(((int *)pep) - 1 ) == SYSERR) {
	kprintf("local_out(): invalid packet 0x%08x (1)\n", pep);
	panic("local_out: bad packet (1)");
    }
#endif
    
    pep = ipreass(pep);
    if (pep == 0)
	return OK;
#ifdef BUF_DEBUG
    if (bpwhichpool(((int *)pep) - 1 ) == SYSERR) {
	kprintf("local_out(): invalid packet 0x%08x (2)\n", pep);
	panic("local_out: bad packet (2)");
    }
#endif
    pip = (struct ip *)pep->ep_data;
    ipdstopts(pni, pep);		/* do IP option processing	*/
#ifdef BUF_DEBUG
    if (bpwhichpool(((int *)pep) - 1 ) == SYSERR) {
	kprintf("local_out(): invalid packet 0x%08x (3)\n", pep);
	panic("local_out: bad packet (3)");
    }
#endif
    switch (pip->ip_proto) {
      case IPT_UDP:
#ifdef DEBUG
	kprintf("	local_out(): get an UDP packet\n");
#endif
	rv = udp_in(pni, pep);
	break;
      case IPT_ICMP:
#ifdef DEBUG
	kprintf("	local_out(): get an ICMP packet\n");
#endif
	rv = icmp_in(pni, pep);
	break;
      case IPT_TCP:
#ifdef DEBUG
	kprintf("	local_out(): get an TCP packet\n");
#endif
	rv = tcp_in(pni, pep);
	break;
      default:
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("local_out: can't receive ip type 0x%x, dropping\n",
		pip->ip_proto);
#endif
	IpInUnknownProtos++;
	icmp(ICT_DESTUR, ICC_PROTOUR, pip->ip_src, pep);
	return OK;
    }
    IpInDelivers++;
    return rv;
}
