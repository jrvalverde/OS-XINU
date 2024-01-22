/* ip2name.c - ip2name */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <mark.h>
#include <gcache.h>

#define PRINTERRORS
#define MAX_RETRIES 3

/*------------------------------------------------------------------------
 *  ip2name - return DARPA Domain name for a host given its IP address
 *------------------------------------------------------------------------
 */
SYSCALL	ip2name(ip, nam)
     IPaddr	ip;
     char	*nam;
{
    char	tmpstr[20];		/* temporary string buffer	*/
    char	*buf;			/* buffer to hold domain query	*/
    int	dg, i, ret;
    register char	*p;
    register struct	dn_mesg *dnptr;
    
    dnptr = (struct dn_mesg *) (buf = (char *) getmem(DN_MLEN));
    *nam = NULLCH;
    dnptr->dn_id = 0;
    dnptr->dn_opparm = hs2net(DN_RD);
    dnptr->dn_qcount = hs2net(1);
    dnptr->dn_acount = dnptr->dn_ncount = dnptr->dn_dcount = 0;
    p = dnptr->dn_qaaa;
    
    /* Fill in question with  ip[3].ip[2].ip[1].ip[0].in-addr.arpa  */
    
    for (i=3 ; i >= 0 ; i--) {
	sprintf(tmpstr, "%d", ip[i] & LOWBYTE);
	dn_cat(p, tmpstr);
    }
    dn_cat(p, "in-addr");
    dn_cat(p, "arpa");
    *p++ = NULLCH;	/* terminate name */
    
    /* Add query type and query class fields to name */
#if defined(SPARC) || defined(RISC)
    {	/* to avoid data alignment problem */
	struct dn_qsuf dn_qsuf;
	
	dn_qsuf.dn_type = hs2net(DN_QTPR);
	dn_qsuf.dn_clas = hs2net(DN_QCIN);
	blkcopy(p, &dn_qsuf, sizeof(struct dn_qsuf));
    }
#else
    ((struct dn_qsuf *)p )->dn_type = hs2net(DN_QTPR);
    ((struct dn_qsuf *)p )->dn_clas = hs2net(DN_QCIN);
#endif
    
    p += sizeof(struct dn_qsuf);
    
    /* Broadcast query */
    if ((dg = open(UDP, NSERVER, ANYLPORT)) == SYSERR) {
	freemem(dnptr, DN_MLEN);
	return(SYSERR);
    }
    control(dg, DG_SETMODE, DG_DMODE | DG_TMODE);
    
    /* send query - up to MAX_RETRIES times */
    for (i = 0; i < MAX_RETRIES; ++i) {
	if ((ret = write(dg, buf, p - buf)) == SYSERR) {
#ifdef PRINTERRORS
	    kprintf("ip2name: SYSERR on write\n");
#endif
	    break;
	}
	
	if ((ret = read(dg, buf, DN_MLEN)) == SYSERR) {
#ifdef PRINTERRORS
	    kprintf("ip2name: SYSERR on read\n");
#endif
	    break;
	}

        if (ret > 0)	/* got a response */
	    break;
#ifdef PRINTERRORS
	kprintf("ip2name() timeout!!\n" );
#endif
	/* else TIMEOUT, try again */
    }
    
    close(dg);
    
    /* check for errors */
    if (ret <= 0) {
#ifdef PRINTERRORS
	if (ret == TIMEOUT)
	    kprintf("ip2name: nameserver timeout\n");
#endif
	freemem(buf, DN_MLEN);
	ip2dot(nam, ip);
	return OK;		/* lie a little */
    }
    
    
    if (net2hs(dnptr->dn_opparm) & DN_RESP || net2hs(dnptr->dn_acount) <= 0) {
	freemem(buf, DN_MLEN);
	ip2dot(nam, ip);
	return OK;
    }
    
    /* In answer, skip name and remainder of resource record header	*/
    while (*p != NULLCH)
	if (*p & DN_CMPRS) 	/* compressed section of name	*/
	    *++p = NULLCH;
	else
	    p += *p + 1;
    p += DN_RLEN + 1;
    
    /* Copy name to user */
    
    *nam = NULLCH;
    
    while (*p != NULLCH) {
	if (*p & DN_CMPRS)
	    p = buf + (net2hs(*(int *)p) & DN_CPTR);
	else {
	    strncat(nam, p+1, *p);
	    strcat(nam, ".");
	    p += *p + 1;
	}
    }
    if (strlen(nam))	/* remove trailing dot */
	nam[strlen(nam) - 1] = NULLCH;
    else
	ip2dot(nam, ip);
    freemem(buf, DN_MLEN);
    
    ipcacheinsert(ip,nam); /* remember it */
    return(OK);
}

static int ipcid;

/*
 * ====================================================================
 * ipcachelookup - look in the generic cache for an addr
 * ====================================================================
 */
static int ipcachelookup(ip, nam)
IPaddr	ip;
char	*nam;
{
    static MARKER ipmark;
    int len;
    
    if (unmarked(ipmark)) {
	ipcid = cacreate("ip2name", 50, CA_LIFETIME);
	mark(ipmark);
    }
    
    len = 100;	/* length isn't passed in */
    return(calookup(ipcid,ip, sizeof(ip), nam, &len));
}

/*
 * ====================================================================
 * ipcacheinsert - insert a new entry in the cache
 * ====================================================================
 */
static int
    ipcacheinsert(ip, nam)
IPaddr	ip;
char	*nam;
{
    return(cainsert(ipcid,ip,sizeof(ip),nam,strlen(nam)+1, 0));
}
