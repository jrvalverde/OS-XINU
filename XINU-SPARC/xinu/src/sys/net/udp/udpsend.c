/* udpsend.c - udpsend */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*#define DEBUG*/

/*------------------------------------------------------------------------
 *  udpsend  -  send one UDP datagram to a given IP address
 *------------------------------------------------------------------------
 */
int udpsend(fip, fport, lport, pep, datalen, docksum)
IPaddr		fip;
unsigned short	fport, lport;
struct	ep	*pep;
int		datalen;
Bool		docksum;
{
    struct	ip	*pip = (struct ip *) pep->ep_data;
    struct	udp	*pudp = (struct udp *) pip->ip_data;
    struct	route	*prt, *rtget();
    
#ifdef DEBUG
    kprintf("udpsend(lport = %d) called\n", lport);
#endif
    pip->ip_proto = IPT_UDP;
    pudp->u_src = lport;
    pudp->u_dst = fport;
    pudp->u_len = U_HLEN+datalen;
    pudp->u_cksum = 0;
    udph2net(pudp);
    if (docksum) {
	prt = rtget(fip, RTF_LOCAL);
	if (prt == (struct route *) NULL) {
	    IpOutNoRoutes++;
	    freebuf(pep);
	    return SYSERR;
	}
	IP_COPYADDR(pip->ip_dst, fip);
	if (prt->rt_ifnum == NI_LOCAL)
	    IP_COPYADDR(pip->ip_src, pip->ip_dst);
	else
	    IP_COPYADDR(pip->ip_src, nif[prt->rt_ifnum].ni_ip);
	rtfree(prt);
	pudp->u_cksum = udpcksum(pip);
	if (pudp->u_cksum == 0)
	    pudp->u_cksum = ~0;
    }
    UdpOutDatagrams++;
#ifdef DEBUG
    kprintf("udpsend: calling ipsend\n");
#endif
    return ipsend(fip, pep, U_HLEN+datalen);
}
