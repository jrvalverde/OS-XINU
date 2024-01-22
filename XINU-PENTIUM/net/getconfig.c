/* 
 * getconfig.c - Get the test configuration from console
 * 
 * Author:	Raj
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Tue Nov  1 14:12:59 1988
 */
#include <conf.h>
#include <kernel.h>
#include <network.h>

#define ALL0	"\000\000\000\000\000\000\000\000\000"

/*
 * ====================================================================
 * getconfig - prompts the user to find out whether this machine is a
 * host or gateway. In the case of a host, finds out which interface
 * to use and also asks for the address of the gatewway on that
 * virtual net. You should use ICMP mask request to get the gateway
 * But, for initial testing this is fine.
 * ====================================================================
 */
getconfig(piface)
int *piface;
{
	int iface;
	int len;
	char buff[128];
	IPaddr gateway;
	int addr1, addr2, addr3, addr4;

	while ( 1 ) {
 		kprintf("Configure machine as host(h) or gateway(g)?: ");
		buff[ 0 ] = NULLCH;
		if ( read(CONSOLE, buff, sizeof(buff)) <= 0 ) {
			continue;
		}
		if ( (buff[0] == 'h') || (buff[0]=='H') ) {
			break;
		}
		if ((buff[0] == 'g') || (buff[0]=='G') ) {
			kprintf("Configuring machine as gateway\n");
			return(TRUE);
		}
	}
	
	/* host config */
	while ( 1 ) {
		kprintf("Enter the interface to be used [0-2]: ");
		if ( (len = read(CONSOLE, buff, sizeof(buff))) <= 0 ) {
			continue;
		}
		buff[len] = NULLCH;
		sscanf(buff, "%d",&iface);
		if ( iface >= 0 && iface <= 2 ) {
			break;
		}
	}
	
	while ( 1 ) {
		kprintf("Enter gateway: ");
		len = read(CONSOLE, buff, sizeof(buff) - 1);
		buff[len] = NULLCH;
		if ( sscanf(buff, "%d.%d.%d.%d", & addr1, & addr2, & addr3,
			    & addr4 ) == 4 ) {
			dot2ip( gateway, addr1, addr2, addr3, addr4 );
			break;
		}
	}
	
	kprintf("\nConfiguring machine as host on interface %d, with gateway ",
		iface);
	kprintf("%u.%u.%u.%u\n", gateway[0]&0xff, gateway[1]&0xff,
		gateway[2]&0xff, gateway[3]&0xff);

	addroute(ALL0, ALL0, iface, TRUE, gateway);
	*piface = iface;
	return(FALSE);
}

