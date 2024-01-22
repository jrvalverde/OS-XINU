#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <proc.h>
#include <bootp.h>

/*#define DEBUG*/

#define MAXTRIES 5	/* five chances to contact server */

static char ip_str[] = "%u.%u.%u.%u";
static char ip_port_str[] = "%u.%u.%u.%u:%u";

int bootp_pid;
struct BootInfo Bootrecord;

/*-------------------------------------------------------------------------
 * bootp - 
 *-------------------------------------------------------------------------
 */
bootp()
{
    int tries, status, ret;
    
    /* ALL needed addresses must come from the server */
    *Bootrecord.myip = NULLCH;
    *Bootrecord.defaultroute = NULLCH;
    *Bootrecord.timeserver = NULLCH;
    *Bootrecord.rfserver = NULLCH;
    *Bootrecord.dnsserver = NULLCH;
#ifdef WIRELESS
    *Bootrecord.wirelessip = NULL;
#endif
    
    /*
     * create a receiver process
     */
    resume(create(BOOTP_RECV_PROC, BOOTP_RECV_STK, BOOTP_RECV_PRIO,
		   BOOTP_RECV_NAM, BOOTP_RECV_ARGS));
#ifdef DEBUG
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
	    kprintf("bootp TIMEOUT.  send again.\n");
	    ret = SYSERR;
	}
	else {
	    /* bootp completed */
	    ret = status;
	    break;
	}
    }
    
    if (tries >= MAXTRIES)
	kprintf("BOOTP: no response from server.\n");
    
    bootp_pid = BADPID;
    return(ret);
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
    blkcopy(bptr->chaddr, (char *)&(ee[0].ed_paddr), EP_ALEN);
    
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
	
	pip = (struct ip *)pep->ep_data;
	
	pup = (struct udp *)pip->ip_data;
	
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
	pip->ip_dst = ip_maskall;
	pip->ip_cksum = 0;
	pip->ip_cksum = cksum(pip, IP_HLEN(pip)>>1);
	
	/* now set the ethernet info */
	blkcopy(pep->ep_eh.eh_dst, EP_BRC, EP_ALEN);
	pep->ep_eh.eh_type = EPT_IP;
	
	if (SYSERR != write(ETHER, pep, EP_HLEN + U_HLEN
			     + IP_HLEN(pip) + BOOTP_SIZE)) {
	    return OK;
	}
    }
    
    kprintf("** failed writing BOOTP request packet.\n");
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
    
#ifdef DEBUG
	kprintf("bootp_recv: started.\n");
#endif
    
    while (read(ETHER, &pep, sizeof(struct ep)) > 0) {
	
#ifdef DEBUG
	kprintf("bootp_recv: got a packet.\n");
#endif
	pip  = (struct ip *) pep->ep_data;
	pup  = (struct udp *) pip->ip_data;
	bptr = (struct bootp_msg *) pup->u_data;
	if ((EPT_IP == pep->ep_type) && (IPT_UDP == pip->ip_proto) && 
	    (NBOOTPC == net2hs(pup->u_dst)) && (BOOTREPLY == bptr->op)) {
#ifdef DEBUG
	    kprintf("bootp_recv: got our reply\n");
#endif
	    if (bootp_parse(pep) == OK) {
		pid = bootp_pid;
		if (!isbadpid(bootp_pid)) {
		    bootp_pid = BADPID;
		    send(pid, OK);
		}
		freebuf(pep);
		return;		/* exit */
	    }
	    freebuf(pep);
	}
    }
}

/*-------------------------------------------------------------------------
 * bootp_parse - 
 *-------------------------------------------------------------------------
 */
bootp_parse(pep)
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
    ip2dot(Bootrecord.myip, bptr->yiaddr);
    /* boot server IP */
    ip2dot(Bootrecord.tftpserver, bptr->siaddr);
    /* boot file name */
    strcpy(Bootrecord.bootfile, bptr->file);
    
    /*
     * parse vendor fields
     */
    while (ptr - vendata < SZVENDOR) {
	switch (*ptr++) {
	case END:
#ifdef DEBUG
	{ char buf[32];
	kprintf("my IP addr is %s.\n", Bootrecord.myip);
	kprintf("Gateway is %s.\n", Bootrecord.defaultroute);
	kprintf("Name server is %s.\n", Bootrecord.dnsserver);
	kprintf("RFS server is %s.\n", Bootrecord.rfserver);
	kprintf("Time server is %s.\n", Bootrecord.timeserver);
	kprintf("TFTP server is %s.\n", Bootrecord.tftpserver);
	kprintf("Subnetmask is %s.\n", ip2dot(buf, Bootrecord.subnetmask));
	kprintf("Boot file name is %s.\n", Bootrecord.bootfile);
#ifdef WIRELESS
	kprintf("Wireless IP addr. is %s.\n", Bootrecord.wirelessip);
#endif
	}
#endif
	return OK;

	case PAD:
	    break;

	case SUBNETMASK:
	    /*
	     * get subnet mask
	     */
	    ptr++;
	    blkcopy(&Bootrecord.subnetmask, ptr, 4);
	    ptr += 4;
	    break;
	    
	case TIME_SERVER:
	    sprintf(Bootrecord.timeserver, ip_port_str, ptr[1],
		    ptr[2], ptr[3], ptr[4], TIME_PORT);
	    ptr = ptr + *ptr + 1;
	    break;
	    
	case DOMAIN_NAMESERVER:
	    sprintf(Bootrecord.dnsserver, ip_port_str, ptr[1],
		    ptr[2], ptr[3], ptr[4], NAME_PORT);
	    ptr = ptr + *ptr + 1;
	    break;
	    
	case LPR_SERVER:
#ifdef WIRELESS
	    sprintf(Bootrecord.wirelessip, ip_str, ptr[1],
		    ptr[2], ptr[3], ptr[4]);
#endif
	    ptr = ptr + *ptr + 1;
	    break;
	    
	case RLP_SERVER:
	    sprintf(Bootrecord.rfserver, ip_port_str, ptr[1],
		    ptr[2], ptr[3], ptr[4], RFS_PORT);
	    ptr = ptr + *ptr + 1;
	    break;
	    
	case GATEWAY:
	    sprintf(Bootrecord.defaultroute, ip_str, ptr[1],
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
