/* netstart.c - netstart */

#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include <network.h>
#include <snmp.h>
#include <tcptimer.h>
#include <bootp.h>

#ifdef OSPF
#include <ospf.h>
#endif /* OSPF */

#include <q.h>

struct	netif	nif[NIF];
struct	netinfo	Net;
extern	int	bsdbrc;

int udpecho();
int gateway_question();

/*#define DEBUG*/

/*------------------------------------------------------------------------
 *  netstart - initialize net
 *------------------------------------------------------------------------
 */
netstart(userpid)
int	userpid;
{
    char str[128];
    char *str1 = "sleeping 30 secs to get routes...";
    char *str2 = "\ndone.\n";
    struct	upq	*pup;
    IPaddr 	gw;
    int	i;
    struct netif   *pni;

    if (clkruns == FALSE)
	panic("clock is not running");

    /* initialize ports */
    for (i=0 ; i<UPPS ; i++)
	upqs[i].up_valid = FALSE;
    udpmutex = screate(1);

    /* these need a command to set/clear them. FIX ME!!! */
/*    gateway = gateway_question();*/
    gateway = 1;
    
    /* bsdbrc = 1;*/ 	/* uncomment to use 0's broadcast */

    if (gateway) {
	IpForwarding = 1;
	initgate();
    }
    else {
	IpForwarding = 2;
	inithost();
    }
    
    for (i = 0; i < Net.nif; ++i) {
	nif[i].ni_ipinq = newq(NI_INQSZ, QF_NOWAIT);
	nif[i].ni_hwtype = AR_HARDWARE;		/* for ARP */
	/* no OTHER's for now */
	nif[i].ni_state = NIS_UP;
    }

    arpinit();
    ipfinit();	/* init the IP frag queue */

    /*
     * get parameters from BOOTP or use default values
     */
    netconfig();

    /*
     * wait()'s synchronize to insure initialization is done
     * before proceeding.
     * N.B. slowtimer() depends on timer server; it must be invoked
     *      after netconfig().
     */
    resume(create(slowtimer, STSTK, STPRI, STNAM, STARGC));
    wait(Net.sema);
    resume(create(ipproc, IPSTK, IPPRI, IPNAM, IPARGC));
    wait(Net.sema);

#ifdef DEBUG
    kprintf("netstart: gateway is %s\n", Bootrecord.defaultroute);
#endif
    
    /* get addrs & names */
    for (i=0; i<Net.nif; ++i) {

	if (i == NI_LOCAL)
	    continue;
	
	pni = &nif[i];
	
	if (pni->ni_state != NIS_UP)
	    continue;
	
	if (pni->ni_ivalid == FALSE) {
	    /*
	     * set IP address, mask, etc.
	     */
	    if (i == NI_PRIMARY) {
#ifdef DEBUG
		kprintf("netstart: setting IP address to '%s'\n",
			Bootrecord.myip);
#endif
		/* use the IP address in the netconf structure */
		pni->ni_ip = dot2ip(Bootrecord.myip);
		/*
		 * add default route and loopback route
		 */
		gw = dot2ip(Bootrecord.defaultroute);
		rtadd(RT_DEFAULT, RT_DEFAULT, gw, RTM_INF-1, 1, RT_INF);
		rtadd(RT_LOOPBACK, ip_maskall, RT_LOOPBACK, 0, NI_LOCAL, RT_INF);
	    }

#ifdef WIRELESS
	    /*
	     * set the wireless interface's IP address
	     */
	    if (pni->ni_speed < 0) {
		pni->ni_speed = -pni->ni_speed;
		pni->ni_ip = dot2ip(Bootrecord.wirelessip);
		kprintf("Wireless IP: %s\n", Bootrecord.wirelessip);
	    }
#endif
	    pni->ni_net = netnum(pni->ni_ip);
            pni->ni_ivalid = TRUE;

	    /*
	     * set subnetmask
	     */
	    if (Bootrecord.subnetmask) {
		setmask(i, Bootrecord.subnetmask);
	    }
	    else {
#ifdef DEBUG
		kprintf("get subnet mask using ICMP...\n");
#endif
		/*
		 * get subnet mask using ICMP
		 */
		icmp(ICT_MASKRQ, 0, nif[i].ni_brc);
		sleep10(1);	/* wait for an answer */
	    }

	    /* host route */
	    rtadd(pni->ni_ip, ip_maskall, pni->ni_ip, 0, NI_LOCAL, RT_INF);

	    /* non-subnetted route */
	    rtadd(pni->ni_net, ip_maskall, pni->ni_ip, 0, NI_LOCAL, RT_INF);
	}

	/*
	 * get the interface's host name using name server
	 */
	getiname(i, str);
#ifdef DEBUG
/*	nifdump(i);*/
#endif
    }

    /*
     * wait()'s synchronize to insure initialization is done
     * before proceeding.
     */
    resume(create(tcptimer, TMSTK, TMPRI, TMNAM, TMARGC));
    wait(Net.sema);
    resume(create(tcpinp, TCPISTK, TCPIPRI, TCPINAM, TCPIARGC));
    wait(Net.sema);
    resume(create(tcpout, TCPOSTK, TCPOPRI, TCPONAM, TCPOARGC));
    wait(Net.sema);

    resume(create(FINGERD, FNGDSTK, FNGDPRI, FNGDNAM, FNGDARGC));
    resume(create(ECHOD, ECHOSTK, ECHOPRI, ECHODNAM, 0));
    resume(create(udpecho, 1000, 50, "udpecho", 0));
    
#ifdef	MULTICAST
    {  	/* ANOTHER ETHER EXPRESS HACK  TO ENSURE MULTICASTING IS SET. */
	struct etdev *ped;

	kprintf("*** use MULTICAST\n");
	ped = &ee[0];
	ped->ed_mcset = 1;
	ee_wstrt(ped);
    }
    /* hginit();  */
#endif	/* MULTICAST */
#ifdef RIP
    resume(create(rip, RIPISTK, RIPPRI, RIPNAM, RIPARGC));
#endif	/* RIP */
#ifdef	OSPF
/*	resume(create(ospf, OSPFSTK, OSPFPRI, OSPFNAM, 0)); */
#endif /* OSPF */
#ifdef	SNMP
    resume(create(snmpd, SNMPSTK, SNMPPRI, SNMPDNAM, 0));
#endif	/* SNMP */


    if (!gateway) {
	write(CONSOLE, str1, strlen(str1));
	sleep(30);
	write(CONSOLE, str2, strlen(str2));
    }

    resume(userpid);
}

/*-------------------------------------------------------------------------
 * gateway_question - 
 *-------------------------------------------------------------------------
 */
gateway_question()
{
    char buff[128];
    while (1) {
	kprintf("Configure machine as host(h) or gateway(g)?: ");
	buff[0] = NULLCH;
	if (read (CONSOLE, buff, sizeof(buff)) <= 0) {
	    continue;
	}
	if ((buff[0] == 'h') || (buff[0] == 'H') ) {
	    kprintf("Configuring as host.\n");
	    return 0;
	}
	if ( (buff[0] == 'g') || (buff[0] == 'G') ) {
	    kprintf("Configuring as gateway.\n");
	    return 1;
	}
    }	
}

/*------------------------------------------------------------------------
 *  netconfig - set network configuration parms using BOOTP
 *------------------------------------------------------------------------
 */
int netconfig()
{
    kprintf("Contacting BOOTP server....\n");
    /*
     * try BOOTP first
     */
    if (bootp() == SYSERR) {
	kprintf("** BOOTP failed; use default values.\n");
	
	strcpy(Bootrecord.myip, DEFAULT_IPADDR1);
	strcpy(Bootrecord.defaultroute, DEFAULT_ROUTER);
	strcpy(Bootrecord.timeserver, DEFAULT_TSERVER);
	strcpy(Bootrecord.rfserver, DEFAULT_RSERVER);
	strcpy(Bootrecord.dnsserver, DEFAULT_NSERVER);
	*Bootrecord.bootfile = 0;
#ifdef WIRELESS
	strcpy(Bootrecord.wirelessip, DEFAULT_WIRELESSIP);
#endif
	return;
    }

    kprintf("BOOTP ok.\n");
}

/*-------------------------------------------------------------------------
 * nifdump -
 *-------------------------------------------------------------------------
 */
nifdump(i)
     int i;
{
    char buf[32];

    kprintf("  network interface %d\n", i);
    kprintf("    state: %s\n", nif[i].ni_state?"UP":"DOWN");
    kprintf("    name: %s\n",
            nif[i].ni_nvalid?nif[i].ni_name:"INVALID");
    kprintf("    ip: %s\n",
            nif[i].ni_ivalid?(char *)ip2dot(buf, nif[i].ni_ip):"INVALID");
    kprintf("    net: %s\n", ip2dot(buf, nif[i].ni_net));
    kprintf("    subnet: %s\n", ip2dot(buf, nif[i].ni_subnet));
    kprintf("    mask: %s\n", ip2dot(buf, nif[i].ni_mask));
    kprintf("    brc: %s\n", ip2dot(buf, nif[i].ni_brc));
    kprintf("    nbrc: %s\n", ip2dot(buf, nif[i].ni_nbrc));
    kprintf("    mtu: %d\n", nif[i].ni_mtu);
}
