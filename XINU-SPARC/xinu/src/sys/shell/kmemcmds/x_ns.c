/* x_ns.c - x_ns */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  x_ns  -  (command ns) interact with the host name server cache
 *------------------------------------------------------------------------
 */
COMMAND	x_ns(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	char	str[128], ipstr[64], name[100];
	IPaddr	ip;
	int	ac;

	for (ac=1; ac < nargs; ++ac) {
		/* get the IP address */
		if (name2ip(ip, args[ac]) == SYSERR) {
			strcpy(ipstr, "not resolved");
			strcpy(name, args[ac]);
		} else {
			ip2dot(ipstr,ip);
			/* get the (domain) name */
			if (ip2name(ip,name) == SYSERR)
			    strcpy(name,"unknown domain name");
		}

		sprintf(str, "%s: %s\n", name, ipstr);
		write(stdout, str, strlen(str));
	}
	return OK;
}
