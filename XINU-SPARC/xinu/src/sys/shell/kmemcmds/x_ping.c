/* x_ping.c - x_ping */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  x_ping  -  do ICMP echo to a given destination
 *------------------------------------------------------------------------
 */
COMMAND
x_ping(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
    char	str[80];
    IPaddr	dst;
    int	got, i;
    int	np, ds;				/* # packets to send, data size */

    ds = 56;		/* default data size */
    np = 10;		/* default # of packets to send */
    if (nargs == 3) {
	ds = atoi(args[2]);
    } else if (nargs = 4) {
        ds = atoi(args[2]);
	np = atoi(args[3]);
    } else if (nargs != 2) {
	sprintf(str, "usage: ping host [size] [# packets]\n");
	kprintf("x_ping:[%s]\n", str);
	write(stderr, str, strlen(str));
	return OK;
    }
    name2ip(dst, args[1]);

    got = 0;
    i = 0;
    while(TRUE) {
	struct ep *packet;

	packet = (struct ep *)recvtim(10); /* wait up to 1 sec */
	if (packet != (struct ep *)TIMEOUT) {
	    struct ip *pip = (struct ip *)packet->ep_data;
	    struct icmp *pic = (struct icmp *)pip->ip_data;

	    sprintf(str, "%d octets from %u.%u.%u.%u: icmp_seq %d\n",
		    pip->ip_len - IP_HLEN(pip),
		    pip->ip_src[0]&0xff, pip->ip_src[1]&0xff,
		    pip->ip_src[2]&0xff, pip->ip_src[3]&0xff,
		    pic->ic_seq);
	    write(stdout, str, strlen(str));
	    freebuf(packet);
	    got++;
	} else {
	    if (i >= np)
		break;
	    icmp(ICT_ECHORQ, 0, dst, i, ds);
	    i++;
	}
    }
    sprintf(str, "received %d/%d packets (%d %% loss)\n", got, np,
	    (100 * (np-got))/np);
    write(stdout, str, strlen(str));
    return(OK);
}