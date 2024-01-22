/* udpcksum.c - udpcksum */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*#define DEBUG*/

#define	UDP_ALEN	IP_ALEN		/* length of src+dst, in shorts	*/

/*------------------------------------------------------------------------
 *  udpcksum -  compute a UDP pseudo-header checksum
 *------------------------------------------------------------------------
 */
unsigned short udpcksum(pip)
struct	ip	*pip;
{
	struct	udp	*pudp = (struct udp *)pip->ip_data;
	unsigned	short	*psh;
	unsigned	long	sum;
	short		len = net2hs(pudp->u_len);
	int		i;

	sum = 0;

	psh = (unsigned short *) pip->ip_src;
	for (i=0; i<UDP_ALEN; ++i)
		sum += *psh++;

	psh = (unsigned short *)pudp;
	sum += hs2net(pip->ip_proto + len);
	if (len & 0x1) {
		((char *)pudp)[len] = 0;	/* pad */
		len += 1;	/* for the following division */
	}

	len /= 2;		/* convert to length in shorts */

	/* unrolled loop */
	while (len >= 10) {
		sum += psh[0];
		sum += psh[1];
		sum += psh[2];
		sum += psh[3];
		sum += psh[4];
		sum += psh[5];
		sum += psh[6];
		sum += psh[7];
		sum += psh[8];
		sum += psh[9];
		psh += 10;
		len -= 10;
	}

	/* slow loop to clean up */
	while (len > 0) {
		sum += *psh++;
		--len;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);

#ifdef DEBUG
	kprintf("udpcksum of packet:\n");
	kprintf("IP-UDP: src %s:%d  dst %s:%d  len %d  cksum 0x%x\n",
		ip2dot("                    ",pip->ip_src), pudp->u_src,
		ip2dot("                    ",pip->ip_dst), pudp->u_dst,
		pudp->u_len, pudp->u_cksum);
	kprintf("yields cksum: 0x%04x\n", (unsigned short) (~sum & 0xffff));
#endif

	return (short)(~sum & 0xffff);
}
