/* ipdump.c - ipdump */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>

#define SHORTFORM

/* assumes ALL fields are in host byte order */
ipdump(pep)
     struct	ep *pep;
{
	char buf1[50], buf2[50];
	struct ip *pip = (struct ip *)pep->ep_data;
	char c[16], *dptr;
	int i, dlen, ct;

#ifdef	SHORTFORM
	if (pip->ip_proto == IPT_TCP) {
		struct tcp *ptcp = (struct tcp *)pip->ip_data;

		dlen = pip->ip_len;
		dlen -= (pip->ip_verlen & 0xf) << 2;
		dlen -= ((ptcp->tcp_offset&0xf0)>>2);
		kprintf("IP-TCP: s %s:%d d %s:%d len %d w %d seq 0x%X ack 0x%X cod 0x%x\n",
			ip2dot(buf1,pip->ip_src), (ptcp->tcp_sport),
			ip2dot(buf2,pip->ip_dst), (ptcp->tcp_dport),
			dlen,
			(ptcp->tcp_window),
			(ptcp->tcp_seq),
			(ptcp->tcp_ack),
			ptcp->tcp_code);
	} else if (pip->ip_proto == IPT_UDP) {
		struct udp *pudp = (struct udp *)pip->ip_data;

		kprintf("IP-UDP: src %s:%d  dst %s:%d  len %d  cksum 0x%x\n",
			ip2dot(buf1,pip->ip_src), pudp->u_src,
			ip2dot(buf2,pip->ip_dst), pudp->u_dst,
			pudp->u_len, pudp->u_cksum);
	} else if (pip->ip_proto == IPT_ICMP) {
		struct icmp	*pic = (struct icmp *)pip->ip_data;

		kprintf("IP-ICMP: src %s  dst %s  ",
			ip2dot(buf1,pip->ip_src),
			ip2dot(buf2,pip->ip_dst));
		kprintf("  type %d code %d cksum 0x%x\n",
			pic->ic_type,
			pic->ic_code,
			pic->ic_cksum);
		switch (pic->ic_type) {
		      case ICT_ECHORP:
		      case ICT_ECHORQ:
			kprintf("   id %d seq %d\n",
				pic->ic_id,
				pic->ic_seq);
			break;
		      case ICT_REDIRECT:
			kprintf("   gw %s\n", ip2dot(buf1,pic->ic_gw));
			break;
		}
	} else
	    kprintf("IP: src %s dst %s proto %d id 0x%X fragoff 0x%x\n",
		    ip2dot(buf1,pip->ip_src),
		    ip2dot(buf2,pip->ip_dst),
		    pip->ip_proto,
		    pip->ip_id,
		    pip->ip_fragoff);
#else					/* SHORTFORM */
	kprintf("ETHER:\tsrc ");
	for (i=0; i<EP_ALEN; ++i)
	    kprintf("%x:", pep->ep_src[i] & 0xff);
	kprintf("\tdst ");
	for (i=0; i<EP_ALEN; ++i)
	    kprintf("%x:", pep->ep_dst[i] & 0xff);
	kprintf("\nETHER:\ttype %x\n", pep->ep_type);
	kprintf("IP:\tver %d len %d tos %x\n", pip->ip_verlen>>4,
		pip->ip_verlen & 0xf, pip->ip_tos);
	kprintf("IP:\tlen %d ident %u fragoff %x\n", pip->ip_len,
		pip->ip_id, pip->ip_fragoff);
	kprintf("IP:\tproto %d\n", pip->ip_proto);
	kprintf("IP:\tttl %d cksum %x\n", pip->ip_ttl, pip->ip_cksum);
	kprintf("IP:\tsrc %X dst %X\n", *((int *)pip->ip_src),
		*((int *)pip->ip_dst));
	dlen = pip->ip_len - ((pip->ip_verlen & 0xf)<<2);
	dptr = pip->ip_data;
	if (pip->ip_fragoff & IP_FRAGOFF)
	    /* empty */;
	else if (pip->ip_proto == IPT_UDP) {
		struct udp *pudp = (struct udp *)pip->ip_data;
		kprintf("UDP:\tsrc %d\tdst %d\tlen %d\n", pudp->u_src,
			pudp->u_dst, pudp->u_len);
		dlen -= 8;
		dptr += 8;
	} else if (pip->ip_proto == IPT_TCP) {
		struct tcp *ptcp = (struct tcp *)pip->ip_data;
		kprintf("TCP:\tsrc %d\tdst %d\tseq %X\tack %X\n", ptcp->tcp_sport,
			ptcp->tcp_dport, ptcp->tcp_seq, ptcp->tcp_ack);
		kprintf("TCP: offset %x code %x window %x\n", ptcp->tcp_offset,
			ptcp->tcp_code, ptcp->tcp_window);
		kprintf("TCP: cksum %x urg %d\n", ptcp->tcp_cksum,
			ptcp->tcp_urgptr);
		dlen -= IP_HLEN(pip);
		dptr += IP_HLEN(pip);
	} else if (pip->ip_proto == IPT_ICMP) {
		struct icmp	*pic = (struct icmp *)pip->ip_data;
		kprintf("ICMP:\ttype %d code %d cksum %x\n", pic->ic_type,
			pic->ic_code, pic->ic_cksum);
		switch (pic->ic_type) {
		      case ICT_ECHORP:
		      case ICT_ECHORQ:
			kprintf("ICMP:\tid %d seq %d\n", pic->ic_id,
				pic->ic_seq);
			break;
		      case ICT_REDIRECT:
			kprintf("ICMP:\tgw %X\n", *((int *)pic->ic_gw));
			break;
		}
		dlen -= sizeof(struct icmp);
		dptr += sizeof(struct icmp);
	}
	if (dlen < 0) {
		kprintf("WARNING: computed dlen %d\n", dlen);
		dlen = 0;
	}
	if (dlen > 48)
	    dlen = 48;
	for (i=0; i<dlen; ++i) {

		if ((i % 16) == 0) {
			c[16] = '\0';
			if (i == 0)
			    kprintf("DATA: ", c);
			else
			    kprintf("\t\"%s\"\nDATA: ", c);
		}
		ct = dptr[i] & 0xff;
		c[i%16] = (ct>=' ' && ct <= '~') ? ct : '.';
		kprintf("%02x ", ct);
	}
	c[i%16] = '\0';
	for (; i%16; ++i)
	    kprintf("   ");
	kprintf("\t\"%s\"\n\n", c);
#endif					/* SHORTFORM */
}

