/* netstart.c - netstart */

#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include <network.h>
#include <nettcp.h>
#include <tcptimer.h>
#include <q.h>
#include <snmp.h>
#include <frame.h>

#define PRINTROUTES
#define PRINTNIFINFO	
/*#define DEBUG*/
#define STARTUP_INFO

extern	int	bsdbrc;
struct	netif		nif[NIF];
struct	netinfo		Net;
int geticmpmask();

/*------------------------------------------------------------------------
 *  netstart - initialize net
 *------------------------------------------------------------------------
 */
PROCESS netstart(userpid)
     int	userpid;		/* user process to resume	*/
{
    char str[128];
    int	i;
    IPaddr ipgw;

    if (clkruns == FALSE)
	panic("net: no clock");
    
    /* initialize ports */
    
    for (i=0 ; i<UPPS ; i++)
	upqs[i].up_valid = FALSE;
    udpmutex = screate(1);
    
    /* these need a command to set/clear them. FIX ME!!! */
    gateway = FALSE;
    
    IpForwarding = 2;	/* == 2 if not a gateway, 1 otherwise */
    IfNumber = Net.nif - 1;
    
    bsdbrc = 1;
    
    if (gateway) {
	initgate();
	rtadd(RT_DEFAULT, RT_DEFAULT, "\200\12\3\160", RTM_INF-1, 1, RT_INF);
    } else
	inithost();
    
    rtadd(RT_LOOPBACK, ip_maskall, RT_LOOPBACK, 0, NI_LOCAL, RT_INF);
    
    for (i=0; i<Net.nif; ++i) {
	nif[i].ni_ipinq = newq(NI_INQSZ, QF_NOWAIT);
	nif[i].ni_hwtype = AR_HARDWARE;		/* for ARP */
	nif[i].ni_state = NIS_UP;
    }
    
#ifdef DEBUG
    kprintf("netstart: calling netconfig to find out where we are.\n");
#endif
    netconfig();
    
    /* add a default route through the netconfig gateway */
    dot2ip(ipgw, netconf.gateway);
    
#ifdef DEBUG
    kprintf("netstart: gateway is %s\n", netconf.gateway);
    kprintf("netstart: adding default route thru '%s' (%x)\n",
	    netconf.gateway, ip2i(ipgw));
#endif
    rtadd(RT_DEFAULT, RT_DEFAULT, ipgw, RTM_INF-1, 1, RT_INF);
    
    arpinit();
    
    ipfinit();	/* init the IP frag queue */
    
    /*
     * wait()'s synchronize to insure initialization is done
     * before proceeding.
     */
    resume(kcreate(slowtimer, STSTK, STPRI, STNAM, STARGC));
    wait(Net.sema);

#if defined(DEBUG) || defined(STARTUP_INFO)
    kprintf("netstart: starting IP\n");
#endif	
    resume(kcreate(ipproc, IPSTK, IPPRI, IPNAM, IPARGC));
    wait(Net.sema);
    
#if defined(DEBUG) || defined(STARTUP_INFO)
    kprintf("netstart: starting TCP\n");
#endif	
    resume(kcreate(tcptimer, TMSTK, TMPRI, TMNAM, TMARGC));
    wait(Net.sema);

    resume(kcreate(tcpinp, TCPISTK, TCPIPRI, TCPINAM, TCPIARGC));
    wait(Net.sema);

    resume(kcreate(tcpout, TCPOSTK, TCPOPRI, TCPONAM, TCPOARGC));
    wait(Net.sema);
    
    /* get addrs & names */
    for (i=0; i<Net.nif; ++i) {
	IPaddr junk;
	char junk2[64];
	
	if (i == NI_LOCAL)
	    continue;
	
	if (nif[i].ni_state != NIS_UP)
	    continue;
	
	if (!gateway && (i == NI_PRIMARY)) {
	    IPaddr mask;
	    struct	netif	*pni;
#ifdef DEBUG
	    kprintf("netstart: setting IP address to '%s'\n",netconf.myIPaddr);
#endif
	    pni = &nif[i];
	    /* use the IP address in the netconf structure */
	    dot2ip(pni->ni_ip, netconf.myIPaddr);
	    netnum(pni->ni_net, pni->ni_ip);
	    pni->ni_ivalid = TRUE;
	    netmask(mask, pni->ni_ip);
	    setmask(i, mask);
	    /* host route */
	    rtadd(pni->ni_ip, ip_maskall, pni->ni_ip, 0, NI_LOCAL, RT_INF);
	    
	    /* non-subnetted route */
	    rtadd(pni->ni_net, ip_maskall, pni->ni_ip, 0, NI_LOCAL, RT_INF);
	}
	
	getiaddr(i, junk);
	
#ifdef DEBUG
	kprintf("netstart: sending ICMP subnet mask request\n");
#endif
	resume(kcreate(geticmpmask, 1000, getprio(getpid()), "geticmpmask",
		       2, i, getpid()));
	receive();
	
	getiname(i, junk2);
	
#ifdef DEBUG
	kprintf("netstart: DONE getting the name of this interface\n");
#endif
	
#ifdef DEBUG
	kprintf("netstart:\nnetwork interface %d\n", i);
	nifdump(i);
#endif
    }	/* end for() */
    
#if defined(DEBUG) || defined(STARTUP_INFO)
    kprintf("netstart: starting RIP\n");
#endif	
    resume(kcreate(rip, RIPSTK, RIPPRI, RIPNAM, RIPARGC));
    
#ifdef DO_SNMP
#ifdef DEBUG
    kprintf("netstart: starting snmpd\n");
#endif
    resume(kcreate(snmpd, SNMPSTK, SNMPPRI, SNMPDNAM, 0));
#endif DO_SNMP
    
#if defined(DEBUG) || defined(STARTUP_INFO)
    kprintf("netstart: starting fingerd\n");
#endif	
    resume(kcreate(FINGERD, FNGDSTK, FNGDPRI, FNGDNAM, FNGDARGC));
    
#if defined(DEBUG) || defined(STARTUP_INFO)
    kprintf("netstart: starting tcp echo\n");
#endif	
    resume(kcreate(TCPECHOD, TCPECHOSTK, TCPECHOPRI, TCPECHODNAM, 0));

#if defined(DEBUG) || defined(STARTUP_INFO)
    kprintf("netstart: starting udp echo\n");
#endif	
    resume(kcreate(UDPECHOD, UDPECHODSTK, UDPECHODPRI, UDPECHODNAM, 0));
    
#if defined(DEBUG) || defined(STARTUP_INFO)
    kprintf("netstart: starting udp discard\n");
#endif	
    resume(kcreate(UDPDISCARD, 1000, UDPDISCARDPRI, UDPDISCARDNAM,0));

#if defined(DEBUG) || defined(STARTUP_INFO)    
    kprintf("netstart: starting xlogin\n");
#endif
    resume(kcreate(XLOGIND, XLOGINSTK, XLOGINPRI, XLOGINDNAM, 0));
    
    /* start the page server process(es) */
    pgiostart();

    resume(userpid);
}

/*-------------------------------------------------------------------------
 * nifdump - 
 *-------------------------------------------------------------------------
 */
nifdump(i)
     int i;
{
    char buf[100];
    
    kprintf("  network interface %d\n", i);
    kprintf("    state: %s\n", nif[i].ni_state?"UP":"DOWN");
    kprintf("    name: %s\n",
	    nif[i].ni_nvalid?nif[i].ni_name:"INVALID");
    kprintf("    ip: %s\n",
	    nif[i].ni_ivalid?(char *)ip2dot(buf,nif[i].ni_ip):"INVALID");
    kprintf("    net: %s\n", ip2dot(buf,nif[i].ni_net));
    kprintf("    subnet: %s\n", ip2dot(buf,nif[i].ni_subnet));
    kprintf("    mask: %08x\n", ip2i(nif[i].ni_mask));
    kprintf("    brc: %s\n", ip2dot(buf,nif[i].ni_brc));
    kprintf("    nbrc: %s\n", ip2dot(buf,nif[i].ni_nbrc));
    kprintf("    mtu: %d\n", nif[i].ni_mtu);
}

/*-------------------------------------------------------------------------
 * geticmpmask - 
 *-------------------------------------------------------------------------
 */
PROCESS geticmpmask(ifnum, ppid)
     int ifnum;
     int ppid;
{
#ifdef DEBUG
    kprintf("process geticmpmask (%d,%d) called\n", ifnum, ppid);
    kprintf("geticmpmask: sending ICMP subnet mask request\n");
#endif	
    icmp(ICT_MASKRQ, 0, nif[ifnum].ni_brc);
    recvtim(30);	/* wait for an answer */
#ifdef DEBUG
    kprintf("geticmpmask: waking parent\n");
#endif	
    send(ppid, 1);
    kill(getpid());
}
