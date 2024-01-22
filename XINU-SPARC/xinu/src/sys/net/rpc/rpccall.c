/* rpccall.c - rpccall */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <xdr.h>
#include <rpcprogs.h>
#include <portmapper.h>
#include <proc.h>


/*#define DEBUG*/
#define PRINTERRORS
#define PRINTTIMEOUTS
#define PRINT_OUTOFORDER
/*#define DUMP_REPLIES*/
/*#define DUMP_REQUESTS*/

/* defns of locals */
static rpcmakereq();
static rpcgetresp();
static rpccheckreply();
static rpcsetdata();

#define MAX_RPC_NETWORK_HEADERS 152

/*------------------------------------------------------------------------
 *  rpccall - make an RPC call
 *------------------------------------------------------------------------
 */
int rpccall(prpc)
     struct rpcblk	*prpc;
{
    unsigned	len;
    unsigned	maxlen;
    unsigned	rpcrtogettime();
    struct ep	*pep_req;
    char		*pch;
    int		ret;

#ifdef DEBUG_VERBOSE
    kprintf("rpccall() called\n");
#endif

    ++prpc->rpc_sequence;
    prpc->rpc_trans = 1;
    prpc->rpc_status = RPC_OK;

    /* determine the maximum size of the request */
    maxlen = prpc->rpc_reqxlen + MAX_RPC_NETWORK_HEADERS;

    /* check for packet TOO large */
    if (maxlen > IP_MAXLEN) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpccall() request too large: %d bytes\n", maxlen);
#endif
	/* invalidate the request */
	prpc->rpc_reqxix = prpc->rpc_reqxlen = 0;

	return(SYSERR);
    }

    for (;;) {
	if (maxlen < NETBUFS) 
	    pep_req = (struct ep *) getbuf(Net.netpool);
	else
	    pep_req = (struct ep *) getbuf(Net.lrgpool);

	if ((len = rpcmakereq(pep_req, prpc)) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("rpccall() couldn't make request, attempt %d\n",
		    prpc->rpc_trans);
#endif
	    freebuf(pep_req);
	    return(SYSERR);
	}

#ifdef DEBUG
	{ char buf[128];
	  ip2name(prpc->rpc_hostIP,buf);
	  kprintf("rpccall: sending %d byte packet to '%s'\n",
		  len, buf);
      }
#endif
	++prpc->rpc_sends;
	prpc->rpc_timeout = rpcrtogettime();
	prpc->rpc_timein  = 0;
 	ret = udpsend(prpc->rpc_hostIP, prpc->rpc_rport,
		      prpc->rpc_lport, pep_req, len, TRUE);
	if (ret != OK) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("rpccall(): error (%d) from udpsend\n", ret);
#endif
	    return(SYSERR);
	}

	if (rpcgetresp(prpc, prpc->rpc_RTO, &prpc->rpc_resppep,
		       &pch, &len) == SYSERR) {
	    if (prpc->rpc_status == RPC_ERROR_TIMEOUT) {
		++prpc->rpc_trans;
		rpcrtocompute(prpc);	/* recompute new RTO values */
		if (prpc->rpc_trans > prpc->rpc_maxtrans) {
		    /* no more retrys allowed */
		    prpc->rpc_reqxix = 0;
		    prpc->rpc_reqxlen = 0;
		    return(SYSERR);
		}
		/* clean up and retry, more patietly */
#if defined(DEBUG) || defined(PRINTTIMEOUTS)
		kprintf("rpccall(%d): timeout (%d), trying #%d\n",
			prpc->rpc_dnum, prpc->rpc_RTO,
			prpc->rpc_trans);
#endif
		prpc->rpc_status = RPC_OK;
		++prpc->rpc_resends;
		continue;
	    } else {
		/* some other kind of error		*/
		/* no other errors returned ... yet	*/
		/* invalidate the request		*/
		prpc->rpc_reqxix = prpc->rpc_reqxlen = 0;
		return(SYSERR);
	    }
	}

	prpc->rpc_timein = rpcrtogettime();
	rpcrtocompute(prpc);		/* recompute new RTO values */

	if (rpccheckreply(prpc, &pch, &len) != OK) {
	    freebuf(prpc->rpc_resppep);
	    prpc->rpc_resppep = (struct ep *) NULLPTR;
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("rpccall() bad response: '%s'\n",
		    RPC_ERROR_GET_NAME(prpc->rpc_status));
#endif
	    /* invalidate the request */
	    prpc->rpc_reqxix = prpc->rpc_reqxlen = 0;

	    return(SYSERR);
	}

	/* else OK, just exit the loop */
	break;
    }

    /* invalidate the request */
    prpc->rpc_reqxix = prpc->rpc_reqxlen = 0;

    /* if we got here, pch points to the beginning of the data */
    ret = rpcsetdata(prpc, pch, len);
    return(ret);
}


/*------------------------------------------------------------------------
 *  rpcnextresp - check for another response to the last request, within
 *	          a timeout of RTO		
 *------------------------------------------------------------------------
 */
int rpcnextresp(prpc, RTO)
     struct rpcblk	*prpc;
     unsigned		RTO;
{
    unsigned	len, ret;
    char		*pch;

    if (rpcgetresp(prpc, RTO, &prpc->rpc_resppep,&pch,&len) == SYSERR) {
	if (prpc->rpc_status == RPC_ERROR_TIMEOUT) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpcnextresp() RPC time-out: '%s'\n",
		RPC_ERROR_GET_NAME(prpc->rpc_status));
#endif	    
	    return(SYSERR);
	}
	/* no other errors returned ... yet */
	return(SYSERR);
    }

    if (rpccheckreply(prpc,&pch,&len) != OK) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpcnextresp() bad response: '%s'\n",
		RPC_ERROR_GET_NAME(prpc->rpc_status));
#endif
	freebuf(prpc->rpc_resppep);
	prpc->rpc_resppep = (struct ep *) NULLPTR;
	return(SYSERR);
    }

    ret = rpcsetdata(prpc,pch,len);
    return(ret);
}


/*-------------------------------------------------------------------------
 * rpcmakereq - fill in the network packet to contain the completed
 * request given by prpc
 *-------------------------------------------------------------------------
 */
LOCAL rpcmakereq(pep, prpc)
     struct ep		*pep;
     struct rpcblk	*prpc;
{
    char	*pch;
    struct	ip	*pip;
    struct	udp	*pudp;

    pip  = (struct ip *) pep->ep_data;
    pudp = (struct udp *) pip->ip_data;
    pch  = pudp->u_data;

    pch = xdrsetunsigned(pch,prpc->rpc_sequence);
    pch = xdrsetunsigned(pch,RPC_MSGTYPE_CALL);
    pch = xdrsetunsigned(pch,RPCVERS);

    if (prpc->rpc_isbrc) {
	/* for broadcast, call the PORTMAPPER */
	pch = xdrsetunsigned(pch,RPCPROG_PMAP);
	pch = xdrsetunsigned(pch,PMAP_VERS);
	pch = xdrsetunsigned(pch,PMAP_PROC_CALLIT); 
    } else {
	pch = xdrsetunsigned(pch,prpc->rpc_program);
	pch = xdrsetunsigned(pch,prpc->rpc_version);
	pch = xdrsetunsigned(pch,prpc->rpc_procedure);
    }

    /* set the authentication */
    if (prpc->rpc_reqauth == RPC_AUTHFLAVOR_NULL) {
	pch = xdrsetunsigned(pch,RPC_AUTHFLAVOR_NULL);
	pch = xdrsetunsigned(pch,0);
	pch = xdrsetunsigned(pch,RPC_AUTHFLAVOR_NULL);
	pch = xdrsetunsigned(pch,0);
    } else if (prpc->rpc_reqauth == RPC_AUTHFLAVOR_UNIX) {
	char	machinename[128];
	int	machlen;
	int	authlen;
	char	*pchTop,*pchAuthLen;

	getname(machinename);
	machlen = strlen(machinename);

	pchTop = pch;
	pch = xdrsetunsigned(pch,RPC_AUTHFLAVOR_UNIX);
	pchAuthLen = pch;
	pch = xdrsetunsigned(pch,0);	/* NULL length (for now) */
	pch = xdrsetunsigned(pch,0);	/* timestamp */
	pch = xdrsetstring(pch,machinename,machlen);
	pch = xdrsetunsigned(pch,prpc->rpc_requid);
	pch = xdrsetunsigned(pch,prpc->rpc_reqgid);
	pch = xdrsetunsigned(pch,0);	/* no other gids */

	/* NOW set the auth length */
	authlen = pch - pchTop - 8;
	(void) xdrsetunsigned(pchAuthLen,authlen);

	/* the verifier should be NULL for unix */
	pch = xdrsetunsigned(pch,RPC_AUTHFLAVOR_NULL);
	pch = xdrsetunsigned(pch,0);
    } else {
	/* only UNIX and NULL are supported at present */
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpcmakreq() bad auth: %d\n", prpc->rpc_reqauth);
#endif
	return(SYSERR);
    }

    if (prpc->rpc_isbrc) {
	/* fill in the program to run */
	pch = xdrsetunsigned(pch,prpc->rpc_program);
	pch = xdrsetunsigned(pch,prpc->rpc_version);
	pch = xdrsetunsigned(pch,prpc->rpc_procedure);
	/* length of the parameters */
	pch = xdrsetunsigned(pch,prpc->rpc_reqxlen);
    }

    /* copy in the data */
    blkcopy(pch,prpc->rpc_reqxbuf,prpc->rpc_reqxlen);
    pch += prpc->rpc_reqxlen;

#ifdef DUMP_REQUESTS
    rpcdump(pudp->u_data,pch - pudp->u_data);
#endif

    return(pch - pudp->u_data);
}


/*-------------------------------------------------------------------------
 * rpcgetresp - wait for and return the response to an RPC call
 *-------------------------------------------------------------------------
 */
LOCAL rpcgetresp(prpc, rto, ppep, ppch, plen)
     struct rpcblk	*prpc;
     unsigned		rto;		/* in sec */
     struct ep		**ppep;
     char		**ppch;
     unsigned		*plen;
{
    struct ep	*pep;	
    struct	ip	*pip;
    struct	udp	*pudp;
    char		*pch;
    unsigned	xid;
    unsigned	mtyp;
    struct	upq	*pup;
    struct	dgblk	*pdg;

    *ppch = NULLPTR;
    *plen = 0;

    /* invalidate any previous replies */
    if (*ppep) {
	freebuf(*ppep);
	prpc->rpc_respxbuf = NULL;
	prpc->rpc_respxbufsize = 0;
	prpc->rpc_respxix = 0;
	prpc->rpc_respxlen = 0;
    }
    *ppep = (struct ep *) NULLPTR;


    pdg = (struct dgblk *) devtab[prpc->rpc_udpdev].dvioblk;
    pup = &upqs[pdg->dg_upq];

    while (1) {
	/* look for the response */
	recvclr();
	if (pcount(pup->up_xport) <= 0) {
	    pup->up_pid = getpid();
	    if (recvtim(rto * 10) == TIMEOUT) {
#if defined(DEBUG)
		kprintf("rpcgetresp: TIMEOUT waiting for response\n");
#endif
		pup->up_pid = BADPID;
		prpc->rpc_status = RPC_ERROR_TIMEOUT;
		return(SYSERR);
	    }
	    pup->up_pid = BADPID;
	    if (pcount(pup->up_xport) <= 0) {
		/* still nothing there, very bad :-(  */
#if defined(DEBUG)
		kprintf("rpcgetresp: BAD MESSAGE\n");
#endif
		/* jump up and try again */
		continue;
	    }
	}

	pep = (struct ep *) preceive(pup->up_xport);
	pip = (struct ip *) pep->ep_data;
	pudp = (struct udp *) pip->ip_data;
	pch = pudp->u_data;
	++prpc->rpc_recvs;

	/* check for response to correct request */
	pch = xdrgetunsigned(pch, &xid);
	if (xid != prpc->rpc_sequence) {
#if defined(DEBUG) || defined(PRINT_OUTOFORDER)
	    kprintf("rpcgetresp: expected xid:%d, got %d\n",
		    prpc->rpc_sequence, xid);
#endif
	    freebuf(pep);
	    ++prpc->rpc_badrecvs;
	    continue;
	}

	/* check for mtyp == response */
	pch = xdrgetunsigned(pch,&mtyp);
	if (mtyp != RPC_MSGTYPE_REPLY) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("rpcgetresp: not a reply\n");
#endif
	    freebuf(pep);
	    ++prpc->rpc_badrecvs;
	    continue;
	}

	*plen = pudp->u_len - U_HLEN;
	*ppep = pep;
	*ppch = pudp->u_data;

#ifdef DUMP_REPLIES
	kprintf("rpcgetresp: got %d byte response:\n", *plen);
	rpcdump(*ppch,*plen);
#endif
	/* record who the response was from */
	IP_COPYADDR(prpc->rpc_resphostIP,pip->ip_src);

	return(OK);
    }
}


/*-------------------------------------------------------------------------
 * rpccheckreply - check reply for validity.  Returns OK if packet
 *     checked out, else it returns SYSERR and sets the RPC status code
 *     If return is OK, then ppch and plen are adjusted to reflect the
 *     data portion of the reply.
 *-------------------------------------------------------------------------
 */
LOCAL rpccheckreply(prpc, ppch, plen)
     struct rpcblk	*prpc;
     char		**ppch;
     unsigned		*plen;
{
    char		*pch;
    unsigned	u;
    unsigned	stat;

    pch = *ppch;

    pch = xdrgetunsigned(pch, &u);	/* xid already checked */
    pch = xdrgetunsigned(pch, &u);	/* mtype already checked */

    /* check the reply status */
    pch = xdrgetunsigned(pch, &u);
    if (u == RPC_REPLYSTAT_DENIED) {
	/* check the rejected status */
	pch = xdrgetunsigned(pch, &stat);
	switch (stat) {
	  case RPC_REJECTSTAT_RPCMISMATCH:
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("rpccheckreply: RPCMISMATCH\n");
#endif
	    prpc->rpc_status = RPC_ERROR_RPCVERS;
	    return(SYSERR);

	  case RPC_REJECTSTAT_AUTHERROR:
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("rpccheckreply: AUTHERROR\n");
#endif
	    prpc->rpc_status = RPC_ERROR_AUTHERROR;
	    return(SYSERR);

	  default:
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("rpccheckreply: UNKNOWN ERROR\n");
#endif
	    prpc->rpc_status = RPC_ERROR_UNKNOWN;
	    return(SYSERR);
	}
    } else if (u != RPC_REPLYSTAT_ACCEPTED) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpccheckreply: UNKNOWN ERROR, but reply code\n");
#endif
	prpc->rpc_status = RPC_ERROR_UNKNOWN;
	return(SYSERR);
    }

    /* record the authentication used */
    pch = xdrgetunsigned(pch,&prpc->rpc_respauth);
    prpc->rpc_respuid = -1;
    prpc->rpc_respgid = -1;
    switch (prpc->rpc_respauth) {
	char *pchEnd;
	unsigned len;

      case RPC_AUTHFLAVOR_UNIX:

	/* first is the opaque length, we'll need that */
	pch = xdrgetunsigned(pch,&len);
	pchEnd = xdrgetfopaque(pch,NULLPTR,len);

	pch = xdrgetunsigned(pch,0);	/* ignore tstamp	*/
	pch = xdrgetstring(pch,0,0);	/* ignore mach name	*/
	pch = xdrgetunsigned(pch,&prpc->rpc_respuid); /* read uid */
	pch = xdrgetunsigned(pch,&prpc->rpc_respgid); /* read gid */

	pch = pchEnd;
	break;

      default:
	/* just read and ignore the opaque data */
	pch = xdrgetvopaque(pch,NULLPTR,0);
	break;
    }


    /* check accepted status */
    pch = xdrgetunsigned(pch,&stat);
    switch (stat) {
      case RPC_ACCEPTSTAT_SUCCESS:
	/* expected case, just break */
	break;

      case RPC_ACCEPTSTAT_PROGUNAVAIL:
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpccheckreply: PROGRAM UNAVAILABLE\n");
#endif
	prpc->rpc_status = RPC_ERROR_PROGUNAVAIL;
	return(SYSERR);

      case RPC_ACCEPTSTAT_GARBAGEARGS:
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpccheckreply: GARBAGE ARGS\n");
#endif
	prpc->rpc_status = RPC_ERROR_GARBARGS;
	return(SYSERR);

      case RPC_ACCEPTSTAT_PROGMISMATCH:
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpccheckreply: PROGRAM MISMATCH\n");
#endif
	prpc->rpc_status = RPC_ERROR_PROGMISMATCH;
	return(SYSERR);

      case RPC_ACCEPTSTAT_PROCUNAVAIL:
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpccheckreply: PROCEDURE UNAVAILABLE\n");
#endif
	prpc->rpc_status = RPC_ERROR_PROCUNAVAIL;
	return(SYSERR);

      default:
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpccheckreply: UNKNOWN STATUS\n");
#endif
	prpc->rpc_status = RPC_ERROR_UNKNOWN;
	return(SYSERR);
    }


    /* subtract RPC header */
    *plen -= (pch - *ppch);

    /* leave pch pointing to data portion */
    *ppch = pch;

    return(OK);
}


/*-------------------------------------------------------------------------
 * rpcsetdata - set the XDR receive buffer to contain the data pointed
 * to by 'pch' of length 'len'
 *-------------------------------------------------------------------------
 */
LOCAL rpcsetdata(prpc, pch, len)
     struct rpcblk	*prpc;
     char		*pch;
     unsigned		len;
{
    int resplen;

    /* if was a broadcast, contains port # and length */
    if (prpc->rpc_isbrc) {
	pch = xdrgetunsigned(pch,&prpc->rpc_respport);
	pch = xdrgetunsigned(pch,&resplen);
	len -= 2 * XDRUNSIGNEDLEN;
    } else {
	resplen = len;
    }

    /* point to the data in the packet */
    prpc->rpc_respxbuf = pch;
    prpc->rpc_respxix = 0;
    prpc->rpc_respxlen = resplen;
    prpc->rpc_respxbufsize = resplen;
    return(OK);
}
