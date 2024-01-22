/* ipproc.c - ipproc */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*#define DEBUG*/
/*#define PRINTERRORS*/
/*#define CHECK_FOR_ERRORS*/

int	ippid, gateway, bsdbrc;
struct	ep	*ipgetp();
struct	route	*rtget();

/*------------------------------------------------------------------------
 *  ipproc  -  handle an IP datagram coming in from the network
 *------------------------------------------------------------------------
 */
PROCESS ipproc()
{
    struct	ep	*pep;
    struct	ip	*pip;
    struct	route	*prt;
    Bool		nonlocal;
    int		ifnum;
    
    ippid = getpid();	/* so others can find us	*/
    
#ifdef DEBUG
    kprintf("PROCESS ip: started (pid: %d)\n", ippid);
#endif		
    signal(Net.sema);	/* signal initialization done	*/
    
    while (TRUE) {
	pep = ipgetp(&ifnum);
#ifdef CHECK_FOR_ERRORS
	if ((ifnum < 0) || (ifnum >= Net.nif)) {
	    kprintf("ipproc: received illegal nif: %d\n", ifnum);
	    ifnum = NI_LOCAL;
	}
#endif
	pip = (struct ip *)pep->ep_data;
#ifdef DEBUG
	kprintf("PROCESS ip(%d): got a packet\n",ifnum);
#endif		
#ifdef SJCDEBUG
	rec_tcp_seg(pep, ifnum==NI_LOCAL?0:1, "IP Proc");
#endif
#ifdef SHOWINPACKETS
	if (ifnum != NI_LOCAL) {
	    char buf[100];
	    kprintf("PROCESS ip(%d): got network packet from %s\n",
		    ifnum, ip2dot(buf,pip->ip_src));
	    ipdump(pep);
	}
#endif
	
	if ((pip->ip_verlen>>4) != IP_VERSION) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("PROCESS ip(%d): bad IP version, dropping\n", ifnum);
#endif
	    IpInHdrErrors++;
	    freebuf(pep);
	    continue;
	}
	if (IP_CLASSD(pip->ip_dst) || IP_CLASSE(pip->ip_dst)) {
	    IpInAddrErrors++;
	    freebuf(pep);
	    continue;
	}
	if (ifnum != NI_LOCAL) {
	    if (cksum(pip, IP_HLEN(pip)>>1)) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("PROCESS ip(%d): bad cksum, dropping\n", ifnum);
#endif
		IpInHdrErrors++;
		freebuf(pep);
		continue;
	    }
	    ipnet2h(pip);
	}
	prt = rtget(pip->ip_dst, (ifnum == NI_LOCAL));
	
	if (prt == (struct route *)NULL) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    char buf[100];
	    
	    kprintf("PROCESS ip(%d): no route to '%s', dropping\n",
		    ifnum, ip2dot(buf,pip->ip_dst));
#endif
	    if (gateway) {
		iph2net(pip);
		icmp(ICT_DESTUR, ICC_NETUR,
		     pip->ip_src, pep);
	    } else {
		IpOutNoRoutes++;
		freebuf(pep);
	    }
	    continue;
	}
	nonlocal = ifnum != NI_LOCAL && prt->rt_ifnum != NI_LOCAL;
	if (!gateway && nonlocal) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    char buf[100];
	    
	    kprintf("PROCESS ip(%d): '%s' not local, dropping\n",
		    ifnum, ip2dot(buf,pip->ip_dst));
	    kprintf("   ");
	    ipdump(pep);
#endif
	    IpInAddrErrors++;
	    freebuf(pep);
	    rtfree(prt);
	    continue;
	}
	if (nonlocal)
	    IpForwDatagrams++;
	/* fill in src IP, if we're the sender */
	
	if (ifnum == NI_LOCAL)
	    if (IP_SAMEADDR(pip->ip_src, ip_anyaddr))
		if (prt->rt_ifnum == NI_LOCAL)
		    blkcopy(pip->ip_src, pip->ip_dst,
			    IP_ALEN);
		else
		    blkcopy(pip->ip_src,
			    nif[prt->rt_ifnum].ni_ip,
			    IP_ALEN);
	if (--(pip->ip_ttl) == 0 && prt->rt_ifnum != NI_LOCAL) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("PROCESS ip(%d): packet ttl == 0,  dropping\n",
		    ifnum);
	    kprintf("   ");
	    ipdump(pep);
#endif
	    IpInHdrErrors++;
	    iph2net(pip);
	    icmp(ICT_TIMEX, ICC_TIMEX, pip->ip_src, pep);
	    rtfree(prt);
	    continue;
	}
	ipdbc(ifnum, pep, prt);	/* handle directed broadcasts	*/
	ipredirect(pep, ifnum, prt); /* do redirect, if needed	*/
#ifdef DEBUG
	kprintf("PROCESS ip(%d): enquing packet\n",ifnum);
#endif		
#ifdef BUF_DEBUG
	if (bpwhichpool(((int *)pep) - 1 ) == SYSERR) {
	    kprintf("PROCESS ip(%d): invalid packet 0x%08x (2)\n",
		    ifnum,pep);
	    panic("IPPROC: bad packet (2)");
	}
#endif
	if (prt->rt_metric != 0)
	    ipputp(prt->rt_ifnum, prt->rt_gw, pep);
	else
	    ipputp(prt->rt_ifnum, pip->ip_dst, pep);
	rtfree(prt);
    }
}

