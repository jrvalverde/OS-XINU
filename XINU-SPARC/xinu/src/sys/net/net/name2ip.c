/* name2ip.c - name2ip, gname, resolve */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <mark.h>
#include <gcache.h>

/*#define DEBUG*/
#define PRINTERRORS

#define MAX_RETRIES 3

static int  namecachelookup();
static void namecacheinsert();

/*------------------------------------------------------------------------
 *  name2ip  -  return the IP address for a given DNS name
 *------------------------------------------------------------------------
 */
SYSCALL	name2ip(ip, nam)
     IPaddr	ip;
     char	*nam;
{
    Bool	isnum;
    char	*p, *index();
    int	ret;
    
#ifdef DEBUG
    kprintf("name2ip(0x%x,%s) called\n", ip, nam);
#endif
    
    isnum = TRUE;
    for (p=nam; *p; ++p) {
	isnum &= ((*p >= '0' && *p <= '9') || *p == '.');
	if (!isnum)
	    break;
    }
    if (isnum) {
	dot2ip(ip, nam);
	return OK;
    }
    
    /* check in the cache first */
    if (namecachelookup(ip, nam) == OK)
	return(OK);
    
    if ((ret = resolve(ip, nam)) == OK)
	namecacheinsert(ip, nam);
    
#ifdef DEBUG
    { char junk[40];
      kprintf("name2ip return (%s, %s)\n", ip2dot(junk, ip), nam);
    }
#endif
    return(ret);
}


/*------------------------------------------------------------------------
 *  gname  -  use the DNS to look up the name
 *------------------------------------------------------------------------
 */
int gname(ip, nam)
     IPaddr	ip;
     char	*nam;
{
    char	tmpstr[64];		/* temporary string buffer	*/
    char	*buf;			/* buffer to hold domain query	*/
    int	dg, i, ret;
    register char	*p, *p2, *p3;
    register struct	dn_mesg *dnptr;
    unsigned short 	dntype, drlen;
    
#ifdef DEBUG
    kprintf("gname(0x%x,%s) called\n", ip, nam);
#endif
    
    dnptr = (struct dn_mesg *) (buf = (char *) getmem(DN_MLEN));
    dnptr->dn_id = 0;
    dnptr->dn_opparm = hs2net(DN_RD);
    dnptr->dn_qcount = hs2net(1);
    dnptr->dn_acount = dnptr->dn_ncount = dnptr->dn_dcount = 0;
    p = dnptr->dn_qaaa;
    
    strcpy(tmpstr, nam);
    p2 = tmpstr;
    while (p3=index(p2, '.')) {
	*p3 = '\0';
	dn_cat(p, p2);
	p2 = p3+1;
    }
    dn_cat(p, p2);
    *p++ = NULLCH;	/* terminate name */

#if defined(SPARC) || defined(RISC)
    {
	/* to fix memory alignment problem */
	struct dn_qsuf dn_qsuf;
	
	dn_qsuf.dn_type = hs2net(DN_QTHA);
	dn_qsuf.dn_clas = hs2net(DN_QCIN);
	blkcopy(p, &dn_qsuf, sizeof(struct dn_qsuf));
    }
#else
    ((struct dn_qsuf *)p )->dn_type = hs2net(DN_QTHA);
    ((struct dn_qsuf *)p )->dn_clas = hs2net(DN_QCIN);
#endif
    p += sizeof(struct dn_qsuf);
    
    /* send query - up to MAX_RETRIES times */
    dg = open(UDP, NSERVER, ANYLPORT);
    control(dg, DG_SETMODE, DG_DMODE | DG_TMODE | DG_CMODE);

    for (i=0; i < MAX_RETRIES; ++i) {
	if ((ret = write(dg, buf, p - buf)) == SYSERR) {
#ifdef PRINTERRORS
	    kprintf("name2ip(ip,%s): SYSERR on write\n", nam);
#endif
	    break;
	}
	if ((ret = read(dg, buf, DN_MLEN)) == SYSERR) {
#ifdef PRINTERRORS
	    kprintf("name2ip(ip,%s): SYSERR on read\n", nam);
#endif
	    break;
	}
	if (ret > 0)	/* got a response */
	    break;
	/* else TIMEOUT, try again */
    }
    close(dg);

    /* check for errors */
    if (ret <= 0) {
#ifdef PRINTERRORS
	if (ret == TIMEOUT)
	    kprintf("name2ip(ip,%s): nameserver timeout\n", nam);
#endif
	freemem(buf, DN_MLEN);
	return SYSERR;
    }
    
    if (net2hs(dnptr->dn_opparm) & DN_RESP || net2hs(dnptr->dn_acount) <= 0) {
	freemem(buf, DN_MLEN);
	return SYSERR;
    }
    /* In answer, skip name and remainder of resource record header	*/
    
    do {
	while (*p != NULLCH)
	    if (*p & DN_CMPRS) 	/* compressed section of name*/
		*++p = NULLCH;
	    else
		p += *p + 1;
	blkcopy(&dntype,p+1,sizeof(short));
	dntype = net2hs(dntype);
	blkcopy(&drlen, p+9,sizeof(short));
	drlen = net2hs(drlen);
	p += DN_RLEN + 1;
	
	/* if this is not an 'A' record, skip to the next answer */
	if (dntype != DN_QTHA)
	    p += drlen;
    } while ((dntype != DN_QTHA) && (--dnptr->dn_acount >= 1));
    if (dnptr->dn_acount < 0) {
	freemem(buf, DN_MLEN);
	return(SYSERR);
    }
    
    /* Copy IP to user */
    for (i=0; i < IP_ALEN; ++i)
	ip[i] = *p++;
    freemem(buf, DN_MLEN);
    return OK;
}

/*------------------------------------------------------------------------
 *  resolve  -  do shorthand DNS name resolution
 *------------------------------------------------------------------------
 */
int resolve(ip, nam)
     IPaddr	ip;
     char	*nam;
{
    char	myname[100];
    char	name_buf[100];
    char	*name_suffix;
    
#ifdef DEBUG
    kprintf("resolve(0x%x,%s) called\n", ip, nam);
#endif
    
    /* if it ends in a 'dot', remove it and try exactly once */
    if (nam[strlen(nam)-1] == '.') {
	strcpy(name_buf, nam);
	name_buf[strlen(name_buf)-1] = NULLCH;
	return gname(ip, name_buf);
    }
    if (getname(myname) == SYSERR)
	return SYSERR;
    
    name_suffix = myname;
    
    /* tack on successively smaller suffixes of MY name */
    while (name_suffix = index(++name_suffix, '.')) {
	sprintf(name_buf, "%s%s", nam, name_suffix);
	if (gname(ip, name_buf) == OK) {
#ifdef DEBUG
	    kprintf("resolve(0x%x,%s) return\n", ip, nam);
#endif
	    return OK;
	}
    }
    return gname(ip, nam);		/* try the original	*/
}





static int namecid;

/*
 * ====================================================================
 * namecachelookup - look in the generic cache for an addr
 * ====================================================================
 */
static int namecachelookup(ip, nam)
IPaddr	ip;
char	*nam;
{
    static MARKER ipmark;
    int len;

#ifdef DEBUG
    kprintf("namecachelookup name=%s\n", nam);
#endif
    
    if (unmarked(ipmark)) {
	namecid = cacreate("name2ip", 50, CA_LIFETIME);
	mark(ipmark);
    }
    
    len = sizeof(ip); 
    return(calookup(namecid, nam, strlen(nam)+1, ip, &len));
}


/*
 * ====================================================================
 * namecacheinsert - insert a new entry in the cache
 * ====================================================================
 */
static void namecacheinsert(ip, nam)
IPaddr	ip;
char	*nam;
{
#ifdef DEBUG
    kprintf("namecacheinsert name=%s\n", nam);
#endif
    
    cainsert(namecid, nam, strlen(nam)+1, ip, sizeof(ip));
}
