/* x_ifstat.c - x_ifstat */

#include <conf.h>
#include <kernel.h>
#include <network.h>

static	char *sn[] = { "DOWN", "UP", "UNUSED" };

/*------------------------------------------------------------------------
 *  x_ifstat  -  print interface status information
 *------------------------------------------------------------------------
 */
COMMAND	x_ifstat(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	char	*str[80];
	char	*dn;
	struct	netif	*pni;
	int	iface;

	if (nargs != 2) {
		sprintf(str, "usage: ifstat <intf>\n");
		write(stderr, str, strlen(str));
		return OK;
	}
	iface = atoi(args[1]);
	if (iface < 0 || iface >= Net.nif) {
		sprintf(str, "ifstat: illegal interface (0-%d)\n", Net.nif-1);
		write(stderr, str, strlen(str));
		return OK;
	}
	if (iface == NI_LOCAL) {
		sprintf(str, "Local interface state: %s\n",
			sn[nif[NI_LOCAL].ni_state]);
		write(stdout, str, strlen(str));
		return OK;
	}
	pni = &nif[iface];
	switch(pni->ni_dev) {
	case ETHER:	dn = "le0";
			break;
#if	Noth > 0
	case OTHER1:	dn = "on0";
			break;
	case OTHER2:	dn = "on1";
			break;
#endif	/* Noth > 0 */
	default:
		dn = "??";
	}
	sprintf(str,"%s: state=%x<%s>\n",dn,pni->ni_state,sn[pni->ni_state]);
	write(stdout, str, strlen(str));
	if (pni->ni_ivalid)
		sprintf(str, "IP %u.%u.%u.%u NAME ", pni->ni_ip[0]&0xff,
			pni->ni_ip[1]&0xff, pni->ni_ip[2]&0xff,
			pni->ni_ip[3]&0xff);
	else
		sprintf(str, "IP <invalid> NAME ");
	write(stdout, str, strlen(str));
	if (pni->ni_nvalid)
		sprintf(str, "\"%s\"\n", pni->ni_name);
	else
		sprintf(str, "<invalid>\n");
	write(stdout, str, strlen(str));
	if (pni->ni_svalid)
		sprintf(str, "MASK %X BROADCAST %u.%u.%u.%u\n",
			ip2i(pni->ni_mask),
			pni->ni_brc[0]&0xff,
			pni->ni_brc[1]&0xff,
			pni->ni_brc[2]&0xff,
			pni->ni_brc[3]&0xff);
	else
		sprintf(str, "MASK <invalid> BROADCAST <invalid>\n");
	write(stdout, str, strlen(str));
	sprintf(str,"MTU %d HADDR %x:%x:%x:%x:%x:%x HBCAST %x:%x:%x:%x:%x:%x\n",
		pni->ni_mtu, pni->ni_hwa.ha_addr[0]&0xff,
		pni->ni_hwa.ha_addr[1]&0xff, pni->ni_hwa.ha_addr[2]&0xff,
		pni->ni_hwa.ha_addr[3]&0xff, pni->ni_hwa.ha_addr[4]&0xff,
		pni->ni_hwa.ha_addr[5]&0xff, pni->ni_hwb.ha_addr[0]&0xff,
		pni->ni_hwb.ha_addr[1]&0xff, pni->ni_hwb.ha_addr[2]&0xff,
		pni->ni_hwb.ha_addr[3]&0xff, pni->ni_hwb.ha_addr[4]&0xff,
		pni->ni_hwb.ha_addr[5]&0xff);
	write(stdout, str, strlen(str));
	sprintf(str, "inq %d\n", pni->ni_ipinq);
	write(stdout, str, strlen(str));
	return(OK);
}
