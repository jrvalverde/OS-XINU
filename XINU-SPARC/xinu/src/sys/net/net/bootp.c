#include <netconf.default.h>
#include <kernel.h>
#include <network.h>
#include <proc.h>
#include <bootp.h>

/*#define DEBUG*/

#define MAXTRIES 5	/* five chances to contact server */

static int bootp_recv_die;

static char ip_str[] = "%u.%u.%u.%u";
static char ip_port_str[] = "%u.%u.%u.%u:%u";


/*-------------------------------------------------------------------------
 * bootp_parse - 
 *-------------------------------------------------------------------------
 */
bootp_parse(netdata, pep)
     struct netconf *netdata;
     struct ep *pep;
{
    
    unsigned char *ptr;
    unsigned long cookie;
    struct ip *pip;
    struct udp *pup;
    struct bootp_msg *bptr;
    unsigned char *vendata;
    
    pip = (struct ip *) pep->ep_data;
    pup = (struct udp *) pip->ip_data;
    bptr = (struct bootp_msg *)pup->u_data;
    vendata = (unsigned char *)bptr->vend;
    
    ptr = vendata;
    cookie = *(unsigned long *)ptr;
    ptr += 4;
    if (net2hl(cookie) != RFC1084) {
	kprintf("bootp_parse: Incorrect RFC 1084 magic cookie in BOOTP reply.\n");
	return SYSERR;
    }
    
    /* grab my IP addr */
    ip2dot(netdata->myIPaddr,&bptr->yiaddr);
    
    while (ptr - vendata < SZVENDOR) {
	switch (*ptr++) {
	  case END:
	    netdata->init = TRUE;
#ifdef DEBUG
	    kprintf("my IP addr is %s.\n", netdata->myIPaddr);
	    kprintf("Gateway is %s.\n", netdata->gateway);
	    kprintf("Name server is %s.\n", netdata->nserver);
	    kprintf("Page server is %s.\n", netdata->pgserver);
	    kprintf("RFS server is %s.\n", netdata->rserver);
	    kprintf("Time server is %s.\n", netdata->tserver);
#endif
	    bootp_recv_die = TRUE;
	    return OK;
	  case PAD:
	    break;
	    
	  case TIME_SERVER:
	    sprintf(netdata->tserver, ip_port_str, ptr[1],
		    ptr[2], ptr[3], ptr[4], TIME_PORT);
	    ptr = ptr + *ptr + 1;
	    break;
	  case DOMAIN_NAMESERVER:
	    sprintf(netdata->nserver, ip_port_str, ptr[1],
		    ptr[2], ptr[3], ptr[4], NAME_PORT);
	    ptr = ptr + *ptr + 1;
	    break;
	  case LPR_SERVER:	/* used for pageserver */
	    sprintf(netdata->pgserver, ip_port_str, ptr[1],
		    ptr[2], ptr[3], ptr[4], PAGE_PORT);
	    ptr = ptr + *ptr + 1;
	    break;
	  case RLP_SERVER:
	    sprintf(netdata->rserver, ip_port_str, ptr[1],
		    ptr[2], ptr[3], ptr[4], RFS_PORT);
	    ptr = ptr + *ptr + 1;
	    break;
	  case GATEWAY:
	    sprintf(netdata->gateway, ip_str, ptr[1],
		    ptr[2], ptr[3], ptr[4]);
	    ptr = ptr + *ptr + 1;
	    break;
	    
	    
	  default:
	    /* skip over by using the length field */
#ifdef DEBUG
	    /*
	      kprintf("skipping field with key %d, length %d\n",ptr[-1], ptr[0]);
	      */
#endif
	    ptr = ptr + *ptr + 1;
	    break;
	}
    }
    return(OK);
}


int bootp_pid;


/*-------------------------------------------------------------------------
 * bootp - 
 *-------------------------------------------------------------------------
 */
bootp()
{
    int tries;
    int status;
    int ret;
    
    /* ALL needed addresses must come from the server */
    *netconf.gateway  = NULLCH;
    *netconf.nserver  = NULLCH;
    *netconf.pgserver = NULLCH;
    *netconf.rserver  = NULLCH;
    *netconf.tserver  = NULLCH;
    *netconf.myIPaddr = NULLCH;
    
    bootp_recv_die = FALSE;
    resume(kcreate(BOOTP_RECV_PROC, BOOTP_RECV_STK, BOOTP_RECV_PRIO,
		   BOOTP_RECV_NAM, BOOTP_RECV_ARGS));
#ifdef DEBUG
    kprintf("bootp: started the receiver process.\n");
    kprintf("bootp: Contacting bootp server.\n");
#endif
    
    bootp_pid = getpid();
    for (tries = 0; tries < BOOTP_MAXRETRY; ++tries) {
	recvclr();
	if (SYSERR == write_bootp_packet()) {
	    ret = SYSERR;
	    break;
	}
	status = recvtim((10 * BOOTP_RESEND) << tries);
	if (SYSERR == status) {
	    ret = SYSERR;
	    break;
	}
	else if (TIMEOUT == status) {
	    kprintf("bootp TIMEOUT.  sending again.\n");
	    ret = SYSERR;
	}
	else {
	    /* bootp complete */
	    ret = status;
	    break;
	}
    }
    if (tries >= MAXTRIES)
	kprintf("Bootp: no response from server.\n");
    bootp_pid = BADPID;
    bootp_recv_die = TRUE;
    return(ret);
}


/*-------------------------------------------------------------------------
 * bootp_eth_addr - 
 *-------------------------------------------------------------------------
 */
char *bootp_eth_addr()
{
    struct etblk *etptr;
    
    etptr = &eth[0];
    return etptr->etpaddr;
}


/*-------------------------------------------------------------------------
 * make_bootp_packet - 
 *-------------------------------------------------------------------------
 */
make_bootp_packet(bptr)
     struct bootp_msg *bptr;
{
    bzero(bptr, BOOTP_SIZE);
    bptr->op = BOOTREQUEST;
    bptr->htype = ETHERNET;
    bptr->hlen = ELEN;
    bptr->xid = 47;		/* just a random number that's nonzero */
    blkcopy(bptr->chaddr, bootp_eth_addr(), EP_ALEN);
    
#ifdef DEBUG
    kprintf("Using ethernet address %02x:%02x:%02x:%02x:%02x:%02x\n",
	    bptr->chaddr[0], bptr->chaddr[1], bptr->chaddr[2],
	    bptr->chaddr[3], bptr->chaddr[4], bptr->chaddr[5]);
#endif
}



/*-------------------------------------------------------------------------
 * write_bootp_packet - 
 *-------------------------------------------------------------------------
 */
write_bootp_packet()
{
    int tries;			/* we'll try THREE times to write data */
    struct bootp_msg bppacket;
    struct ep *pep;
    struct ip *pip;
    struct udp *pup;
    
    make_bootp_packet(&bppacket);
    for (tries = 0; tries < 3; ++tries) {
	/* now get an ethernet buffer and copy the info. */
	/* it's not the best way to do this, but we want */
	/* to change the code as little as possible now. */
	pep = (struct ep *) getbuf(Net.netpool);
	if (pep == (struct ep *)SYSERR)
	    return SYSERR;
	
	pip = (struct ip *)  pep->ep_data;
	
	pup = (struct udp *) pip->ip_data;
	
	/* set the BOOTP data */
	blkcopy(pup->u_data, &bppacket, BOOTP_SIZE);
	
	/* now set the UDP header info */
	pup->u_src = hs2net(NBOOTPC);
	pup->u_dst = hs2net(NBOOTPS);
	pup->u_len = hs2net(U_HLEN + BOOTP_SIZE);
	pup->u_cksum = 0;
	
	/* now set the IP header info */
	pip->ip_proto = IPT_UDP;
	pip->ip_verlen = (IP_VERSION<<4) | IP_MINHLEN;
	pip->ip_tos = 0;
	pip->ip_len = hs2net(IP_HLEN(pip) + U_HLEN + BOOTP_SIZE);
	pip->ip_id = 0;
	pip->ip_fragoff = 0;
	pip->ip_ttl = IP_TTL;
	bzero(pip->ip_src, IP_ALEN);
	IP_COPYADDR(pip->ip_dst, ip_maskall);
	pip->ip_cksum = 0;
	pip->ip_cksum = cksum(pip, IP_HLEN(pip)>>1);
	
	/* now set the ethernet info */
	blkcopy(pep->ep_eh.eh_dst, EP_BRC, EP_ALEN);
	pep->ep_eh.eh_type = EPT_IP;
	
	if (SYSERR != write (ETHER, pep, EP_HLEN + U_HLEN
			     + IP_HLEN(pip) + BOOTP_SIZE)) {
	    return OK;
	}
    }
    
    kprintf("failed writing bootp request packet.\n");
    return SYSERR;
}


/*-------------------------------------------------------------------------
 * bootp_recv - 
 *-------------------------------------------------------------------------
 */
bootp_recv()
{
    struct ip *pip;
    struct udp *pup;
    struct bootp_msg *bptr;
    struct ep *pep;
    int pid;
    
    while (read(ETHER, &pep, sizeof(struct ep)) > 0) {
	if (bootp_recv_die)
	    return;
#ifdef DEBUG
	kprintf("bootp_recv: got a packet.\n");
#endif
	pip  = (struct ip *) pep->ep_data;
	pup  = (struct udp *) pip->ip_data;
	bptr = (struct bootp_msg *) pup->u_data;
	if ((EPT_IP == pep->ep_type) &&
	    (IPT_UDP == pip->ip_proto) && 
	    (NBOOTPC == net2hs(pup->u_dst)) &&
	    (BOOTREPLY == bptr->op)) {
#ifdef DEBUG
	    kprintf("bootp_recv: got our reply\n");
#endif
	    if (bootp_parse(&netconf,pep) == OK) {
		pid = bootp_pid;
		if (!isbadpid(bootp_pid)) {
		    bootp_pid = BADPID;
		    send(pid, OK);
		}
		freebuf(pep);
		kill(getpid());
	    } else {
		freebuf(pep);
	    }
	}
    }
}
