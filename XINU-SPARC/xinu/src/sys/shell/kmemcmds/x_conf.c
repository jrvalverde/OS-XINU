/* x_conf.c - x_conf */

#include <conf.h>
#include <kernel.h>
#include <network.h>


/*------------------------------------------------------------------------
 *  x_conf  -  Print info about the system configuration
 *------------------------------------------------------------------------
 */
COMMAND	x_conf(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	char	str[128];
	char	name[80];
	char	*FindName();
	int	i;
	extern char vers[];

	sprintf(str,"Identification\n");
	write(stdout, str, strlen(str));

	sprintf(str,"    Xinu version:     %s\n", vers);
	write(stdout, str, strlen(str));

	sprintf(str,"    Ethernet address: %02x:%02x:%02x:%02x:%02x:%02x\n",
		eth[0].etpaddr[0]&0xff,
		eth[0].etpaddr[1]&0xff,
		eth[0].etpaddr[2]&0xff,
		eth[0].etpaddr[3]&0xff,
		eth[0].etpaddr[4]&0xff,
		eth[0].etpaddr[5]&0xff);
	write(stdout, str, strlen(str));
	
	for (i=0; i<Net.nif; ++i) {
		if (nif[i].ni_state == NIS_DOWN)
			continue;
		if (i == NI_LOCAL)
			continue;
		sprintf(str,"    IP address:       %d.%d.%d.%d\n",
			(unsigned short) nif[i].ni_ip[0]&0xff,
			(unsigned short) nif[i].ni_ip[1]&0xff,
			(unsigned short) nif[i].ni_ip[2]&0xff,
			(unsigned short) nif[i].ni_ip[3]&0xff);
		write(stdout, str, strlen(str));
		sprintf(str,"    Domain name:      %s\n",
			ip2name(nif[i].ni_ip,name) == SYSERR ? "UNKNOWN" : name);
		write(stdout, str, strlen(str));
	}


	sprintf(str,"\nTable sizes\n");
	write(stdout, str, strlen(str));

	sprintf(str,"    Number of threads/processes:   %d\n",NPROC);
	write(stdout, str, strlen(str));

	sprintf(str,"    Number of address spaces:      %d\n",NPROC);
	write(stdout, str, strlen(str));

	sprintf(str,"    Number of semaphores:          %d\n",NSEM);
	write(stdout, str, strlen(str));

	sprintf(str,"    Number of devices:             %d\n",NDEVS);
	write(stdout, str, strlen(str));

	sprintf(str,"\nNetwork information\n");
	write(stdout, str, strlen(str));

#ifdef GATEWAY_DEFAULT
	sprintf(str,"    Network gateway:    %-20s (%s)\n",
		netconf.gateway, FindName(netconf.gateway));
	write(stdout, str, strlen(str));
#endif	

#ifdef TSERVER
	sprintf(str,"    Time server:        %-20s (%s)\n",
		TSERVER, FindName(TSERVER));
	write(stdout, str, strlen(str));
#endif	

#ifdef RSERVER
	sprintf(str,"    Remote file server: %-20s (%s)\n",
		RSERVER, FindName(RSERVER));
	write(stdout, str, strlen(str));
#endif	

#ifdef NSERVER
	sprintf(str,"    Domain name server: %-20s (%s)\n",
		NSERVER, FindName(NSERVER));
	write(stdout, str, strlen(str));
#endif	

#ifdef PGSERVERADDR
	sprintf(str,"    Xinu Page server:   %-20s (%s)\n",
		PGSERVERADDR, FindName(PGSERVERADDR));
	write(stdout, str, strlen(str));
#endif	

	return(OK);
}


char *FindName(strIP)
     char *strIP;
{
    static char name[80];
    IPaddr addr;

/*    kprintf("FindName(%s) called\n", strIP);*/

    dot2ip(addr, strIP);
    
/*    kprintf("FindName(%s) ==> 0x%x\n", *((unsigned *) addr));*/

    if (ip2name(addr, name) == SYSERR)
	return("UNKNOWN");
    else
	return(name);
}
