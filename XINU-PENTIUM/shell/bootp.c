#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <bootp.h>

/* -------------------------------------------------------------------
 * Bootp code. Calls the bootp server and stores the information in
 * the Bootrecord.
 *
 * TODO 
 * -----
 * check Magic cookie.   99.130.83.99
 * -------------------------------------------------------------------
 */
struct xgram bootpackage;

struct BootInfo Bootrecord;

bootp()
{
	int	i;
 	int 	count;
	int	dev; 	
	char str[80];
	struct boot_type *tmp;

	if ( (dev = open(UDP, "128.10.255.255:67", 68)) == SYSERR)
	{ 

		kprintf( "BOOTP: open fails\n");
		return (SYSERR);
	}
	control(dev, DG_SETMODE, DG_TMODE | DG_NMODE);
	tmp = (struct boot_type *)bootpackage.xg_data;
	bootpackage.xg_fip = 0xffffffff;
	bootpackage.xg_fport = 67;
	bootpackage.xg_lport = 68;
	tmp->boot_op = 1;
	tmp->boot_htype = 1;  /* ETHER */
	tmp->boot_hlen = 6;
	tmp->boot_hops = 0;
	tmp->boot_id = 17;
	tmp->boot_sec = 0;
	tmp->boot_clientip = 0;
	tmp->boot_yourip = 0;
	tmp->boot_serverip = 0;
	tmp->boot_gatewayip = 0;	
	/* HARDWARE ADDRESS FOR MACHINE */
	{
	extern struct netif	nif[];
	extern struct netinfo   Net;

	int i;
	for (i=0;i<=Net.nif;i++)
		if ((nif[i].ni_state == NIS_UP) && (i!= NI_LOCAL))	
			break;
	tmp->boot_hardware[0] = nif[i].ni_hwa.ha_addr[0]&0xff;
	tmp->boot_hardware[1] = nif[i].ni_hwa.ha_addr[1]&0xff;
	tmp->boot_hardware[2] = nif[i].ni_hwa.ha_addr[2]&0xff;
	tmp->boot_hardware[3] = nif[i].ni_hwa.ha_addr[3]&0xff;
	tmp->boot_hardware[4] = nif[i].ni_hwa.ha_addr[4]&0xff;
	tmp->boot_hardware[5] = nif[i].ni_hwa.ha_addr[5]&0xff;
	}
	tmp->boot_servername[0] = 0;
	tmp->boot_file[0] = 0;
	if (debug_level("bootp")>=2)
		printbootp(tmp);
	write(dev, &bootpackage, sizeof(struct boot_type) + 8);

	count = 5;
        i = read(dev, &bootpackage, sizeof(struct boot_type) + 8);
	while((i == TIMEOUT) && (count --))
	   i = read(dev, &bootpackage, sizeof(struct boot_type) + 8);

	if (count < 0)
		{
		kprintf("TIMEOUT on Boot Protocol.\n");
		close(dev);
		return(SYSERR);
		}
	if (debug_level("bootp")>=2)
		printbootp(tmp);
	close(dev);
/* ---- Extract info ------- */
	strcpy(Bootrecord.bootfile, tmp->boot_file);
	ip2dot(Bootrecord.myip, tmp->boot_yourip);
	ip2dot(Bootrecord.rfserver, tmp->boot_serverip);
	strcat(Bootrecord.rfserver, ":2001");
	ip2dot(Bootrecord.tftpserver, tmp->boot_serverip);
	/* Parse Vendor Field */
	return parse_vendor(tmp->boot_vendor);
}
int parse_bootp(struct boot_type *tmp)
{
	strcpy(Bootrecord.bootfile, tmp->boot_file);
	ip2dot(Bootrecord.myip, tmp->boot_yourip);
	ip2dot(Bootrecord.rfserver, tmp->boot_serverip);
	strcat(Bootrecord.rfserver, ":2001");
	ip2dot(Bootrecord.tftpserver, tmp->boot_serverip);
	/* Parse Vendor Field */
	return parse_vendor(tmp->boot_vendor);
}
int parse_vendor(char vendor[])
{
    unsigned char code = 0, length;
	IPaddr address;
    int index = 4;
 
    while ((code != BV_END) && (index < 64))
	{
		code = (unsigned char)vendor[index++];
		if (debug_level("bootp")>=4)
			kprintf("BOOTP: Code = %d\n", code);
		switch (code) {
			case BV_PAD: 
			case BV_END: 
						break;
			case BV_SUBMASK:
						  index ++;
						  bcopy(&vendor[index],&Bootrecord.subnetmask, 4);
						  index += 4;
						  break;
			case BV_TOD: 
						index ++;
						index += 4;
					  	break;
			case BV_IEN116:
			case BV_QUOTE:
			case BV_LPR:
			case BV_IMPRESS:
			case BV_RLP:
			case BV_HOSTNAME: 	length = (unsigned char) vendor[index++];
							  	index += length;
								break;	
			case BV_GATE:	 	length = (unsigned char) vendor[index++];
								bcopy(&vendor[index], &address, 4);
								ip2dot(Bootrecord.defaultroute, address);
							  	index += length;
								break;	
			case BV_TS:		 	length = (unsigned char) vendor[index++];
								bcopy(&vendor[index], &address, 4);
								ip2dot(Bootrecord.timeserver, address);
								strcat(Bootrecord.timeserver, ":37");
							  	index += length;
								break;	
			case BV_DNS:	 	length = (unsigned char) vendor[index++];
								bcopy(&vendor[index], &address, 4);
								ip2dot(Bootrecord.dnsserver, address);
								strcat(Bootrecord.dnsserver, ":53");
							  	index += length;
								break;	
			case BV_LOG:	 	length = (unsigned char) vendor[index++];
								bcopy(&vendor[index], &address, 4);
								ip2dot(Bootrecord.loghost, address);
								strcat(Bootrecord.loghost, ":514");
							  	index += length;
								break;	
			case BV_BOOTSIZE:	index += 3;
								break;
			default:	
								return SYSERR;
			} /* end case */
	} /* while */
	return OK;
}


printbootp(struct boot_type *tmp)
{
	char str[80];
	printf("BOOTP:OP = %3d    Htype = %3d    HLEN = %3d   HOPS = %3d \n", 
		tmp->boot_op, tmp->boot_htype, tmp->boot_hlen, tmp->boot_hops);
	printf("BOOTP:Client IP = %15s   ", ip2dot(str, tmp->boot_clientip));
	printf("BOOTP:Your IP = %20s \n", ip2dot(str, tmp->boot_yourip));
	printf("BOOTP:Server IP = %15s  ", ip2dot(str, tmp->boot_serverip));
	printf("BOOTP:GateWay IP = %20s \n",ip2dot(str, tmp->boot_gatewayip));
	printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
		tmp->boot_hardware[0] & 0xff,
		tmp->boot_hardware[1] & 0xff,
		tmp->boot_hardware[2] & 0xff,
		tmp->boot_hardware[3] & 0xff,
		tmp->boot_hardware[4] & 0xff,
		tmp->boot_hardware[5] & 0xff);
	printf("BOOTP:ServerName is ->%s<-  Bootfile = ->%s<-\n",
			tmp->boot_servername, tmp->boot_file);
	return OK;
}
