/* 
 * kprintip.c - print the ip address
 * 
 * 		Dept. of Computer Science
 * 		Purdue University
 * Date:	Sat Apr  3 16:13:58 1993
 */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*-------------------------------------------------------------------------
 * kprintip - kprintf input IPaddr
 *-------------------------------------------------------------------------
 */
int kprintip(ip)
     IPaddr ip;
{
    kprintf("%d.%d.%d.%d", ip[0] & 0xff, ip[1] & 0xff, ip[2] & 0xff,
	    ip[3] & 0xff);
}

