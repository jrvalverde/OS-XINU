/* icmp.c - icmp */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <mark.h>

/*#define DEBUG*/

struct	ep	*icsetbuf();

/*
 * ICT_REDIRECT	- pa2 == gateway address
 * ICT_PARAMP	- pa2 == (packet) pointer to parameter error
 * ICT_MASKRP	- pa2 == mask address
 * ICT_ECHORQ	- pa1 == seq, pa2 == data size
 */

/*------------------------------------------------------------------------
 *  icmp -  send an ICMP message
 *------------------------------------------------------------------------
 */
icmp(type, code, dst, pa1, pa2)
     int	type, code;
     IPaddr	dst;
     char	*pa1, *pa2;
{
    struct	ep	*pep;
    struct	ip	*pip;
    struct	icmp	*pic;
    Bool		isresp, iserr;
    IPaddr		tdst;
    int		datalen;

#ifdef DEBUG
    kprintf("icmp() called\n");
#endif
    
    IcmpOutMsgs++;
    IP_COPYADDR(tdst, dst);
    
    pep = icsetbuf(type, pa1, &isresp, &iserr);
    if (pep == (struct ep *) SYSERR) {
#ifdef DEBUG
	kprintf("!!! icmp: bad e-packet\n");
#endif
	IcmpOutErrors++;
	return SYSERR;
    }
    pip = (struct ip *) pep->ep_data;
    pic = (struct icmp *) pip->ip_data;
    
    datalen = IC_HLEN;
    
    /* we fill in the source here, so routing won't break it */
    if (isresp) {
	if (iserr) {
	    if (!icerrok(pep)) {
		freebuf(pep);
		return OK;
	    }
	    blkcopy(pic->ic_data, pip, IP_HLEN(pip)+8);
	    datalen += IP_HLEN(pip)+8;
	}
	icsetsrc(pip);
    } else
	IP_COPYADDR(pip->ip_src, ip_anyaddr);
    IP_COPYADDR(pip->ip_dst, tdst);
    
    pic->ic_type = (char) type;
    pic->ic_code = (char) code;
    if (!isresp) {
	if (type == ICT_ECHORQ)
	    pic->ic_seq = (int) pa1;
	else
	    pic->ic_seq = 0;
	pic->ic_id = icgetid();
    }
    datalen += icsetdata(type, pip, pa2);
    
    pic->ic_cksum = 0;
    pic->ic_cksum = cksum(pic, (datalen+1)>>1);
    
    pip->ip_proto = IPT_ICMP;	/* for generated packets */
    ipsend(tdst, pep, datalen);
#ifdef DEBUG
    kprintf("icmp() exit (type = %d)\n", pic->ic_type);
#endif
    return OK;
}


static MARKER icmarker;
static unsigned int icpids[NPROC];
static int icmutex;


/*
 * ====================================================================
 * icgetid - return a unique icmp id and register the current process
 *           as being interested in it.
 * ====================================================================
 */
icgetid()
{
    static unsigned int lastid = 0;
    int i;
    
    if (!marked(icmarker)) {
	for (i=0; i < NPROC; ++i)
	    icpids[i] = 0;
	icmutex = screate(1);
	mark(icmarker);
    }
    
    wait(icmutex);
    while (1) {
	if (++lastid == 0)
	    lastid = 1;
	for (i=0; i < NPROC; ++i)
	    if (icpids[i] == lastid)
		continue;
	/* this one is unique */
	icpids[getpid()] = lastid;
	signal(icmutex);
	return(lastid);
    }
}

/*
 * ====================================================================
 * icsendproc - send the message 'msg' to the process who was
 *              interested in icmp message 'id' if there is one.
 *              Returns OK if the message was sent, and SYSERR otherwise.     
 * ====================================================================
 */
icsendproc(id,msg)
     unsigned int id;
     int msg;
{
    int i;
    int ret;
    
    wait(icmutex);
    
    /* see if anyone is waiting for it */
    for (i=0; i < NPROC; ++i)
	if (icpids[i] == id) {
	    icpids[i] = 0;
	    ret = send(i,msg);
	    signal(icmutex);
	    return(ret);
	}
    
    signal(icmutex);
    return(SYSERR);
}
