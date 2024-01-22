/* rpclisten.c - rpclisten
 *
 * Author:	Brian K. Grant
 *		Dept. of Computer Sciences
 *		Purdue University
 *
 * Written:	11-3-91
 */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <portmapper.h>
#include <xdr.h>
#include <ctype.h>
#include <sem.h>

/*#define DEBUG*/
/*#define DEBUG_VERBOSE*/

static int rpccheckcall();
static int rpcopenaserver();
static char *rpcgetauth();

extern int rpcalloc();
extern int rpcprepdev();


/*----------------------------------------------------------------------------
 * rpclisten -- wait indefinitely for a RPC call and returns an active RPC
 *	server device to handle the call
 *----------------------------------------------------------------------------
 */
int rpclisten(prpc, pnewdev)
     struct rpcblk	*prpc;
     int		*pnewdev;
{
    struct	rpcblk	*pnewrpc;
    struct	 ep	*pep;	
    struct	ip	*pip;
    struct	udp	*pudp;
    struct	upq	*pup;
    struct	dgblk	*pdg;
    char		*pch;
    unsigned		mtyp;
    int			slot;

    pdg = (struct dgblk *) devtab[prpc->rpc_udpdev].dvioblk;
    pup = &upqs[pdg->dg_upq];

    while (TRUE) {
	/* Get a packet */
#ifdef DEBUG_VERBOSE
	kprintf("rpclisten: listening\n");
#endif
	prpc->rpc_resppep = pep = (struct ep *) preceive(pup->up_xport);
#ifdef RPC_BUF_DEBUG
	kprintf("rpclisten: preceive gives 0x%x\n", pep);
	pep_enq(pep);
#endif
	pip = (struct ip *) pep->ep_data;
	pudp = (struct udp *) pip->ip_data;
	prpc->rpc_respxbuf = pch = pudp->u_data;
	prpc->rpc_respxlen = prpc->rpc_respxbufsize = pudp->u_len - U_HLEN;
	prpc->rpc_respxix = 0;
	++prpc->rpc_recvs;
#ifdef DEBUG_VERBOSE
	kprintf("rpclisten: got one\n");
#endif

	/* get the xid */
	pch = xdrgetunsigned(pch, &prpc->rpc_sequence);

	/* check for mtyp == call */
	pch = xdrgetunsigned(pch, &mtyp);
	if (mtyp != RPC_MSGTYPE_CALL) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("rpclisten: Message not a call\n");
#endif
	    freebuf(pep);
	    ++prpc->rpc_badrecvs;
	    continue;
	}

	/* record who the call was from */
	IP_COPYADDR(prpc->rpc_resphostIP, pip->ip_src);
	prpc->rpc_respport = pudp->u_src;

#ifdef DUMP_CALLS
	{
	    char buf[RPC_MAX_MACHINENAME_LEN];

	    ip2name(prpc->rpc_resphostIP, buf);
	    kprintf("rpclisten: got %d byte call from %d%%%s:\n",
		    prpc->rpc_respxlen, (int) prpc->rpc_respport, buf);
	    rpcdump(prpc->rpc_respxbuf, prpc->rpc_respxlen);
	}
#endif

	/* Check part of the call header and reply if trivial.
	 * Discard the message if something goes wrong.
	 */
	if (rpccheckcall(prpc, &pch) == SYSERR) {
	    freebuf(pep);
	    continue;
	}
	prpc->rpc_respxbufsize = prpc->rpc_respxlen =
	    prpc->rpc_respxbufsize + prpc->rpc_respxbuf - pch;
	prpc->rpc_respxbuf = pch;
	prpc->rpc_respxix = 0;
	break;
    }
    
    /* find an available pseudo device for the active server */
    if ((slot=rpcalloc()) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpclisten ==> SYSERR, no more pseudo devices\n");
#endif
	freebuf(pep);
	return(SYSERR);
    }
    pnewrpc = &rpctab[slot];
    
    /* create the mutex */
    pnewrpc->rpc_mutex = screate(1);
    
    if (rpcopenaserver(pnewrpc, prpc) == SYSERR) {
	sdelete(pnewrpc->rpc_mutex);
	freebuf(pep);
	pnewrpc->rpc_state = RPCS_FREE;

	return (SYSERR);
    }

    *pnewdev = pnewrpc->rpc_dnum;
    return (OK);
}


/*----------------------------------------------------------------------------
 * rpcopenaserver -- set up the active server and reset the listening server
 *----------------------------------------------------------------------------
 */
static
int rpcopenaserver(pnewrpc, poldrpc)
    struct rpcblk *pnewrpc, *poldrpc;
{
	pnewrpc->rpc_type = RPCT_ASERVER;

	IP_COPYADDR(pnewrpc->rpc_resphostIP, poldrpc->rpc_resphostIP);
	IP_COPYADDR(pnewrpc->rpc_hostIP, poldrpc->rpc_resphostIP);
	pnewrpc->rpc_rport = pnewrpc->rpc_respport = poldrpc->rpc_respport;

	if (rpcprepdev(pnewrpc) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("rpcopenaserver: rpcprepdev() returned an error\n");
#endif
		return (SYSERR);
	}

	pnewrpc->rpc_program	= poldrpc->rpc_program;
	pnewrpc->rpc_minversion	= poldrpc->rpc_minversion;
	pnewrpc->rpc_version	= poldrpc->rpc_version;
	pnewrpc->rpc_sequence	= poldrpc->rpc_sequence;
	pnewrpc->rpc_reqauth	= poldrpc->rpc_reqauth;
	pnewrpc->rpc_procedure	= poldrpc->rpc_procedure;
	/* requid and reqgid fields are fixed */

	/* set up the call marshalling buffer */
	pnewrpc->rpc_resppep = poldrpc->rpc_resppep;
	poldrpc->rpc_resppep = (struct ep *) NULL;
	pnewrpc->rpc_respxbufsize = poldrpc->rpc_respxbufsize;
	poldrpc->rpc_respxbufsize = 0;
	pnewrpc->rpc_respxbuf = poldrpc->rpc_respxbuf;
	poldrpc->rpc_respxbuf = NULL;
	pnewrpc->rpc_respxix = poldrpc->rpc_respxix;
	poldrpc->rpc_respxix = 0;
	pnewrpc->rpc_respxlen = poldrpc->rpc_respxlen;
	poldrpc->rpc_respxlen = 0;

	pnewrpc->rpc_respvers	= poldrpc->rpc_respvers;
	pnewrpc->rpc_respauth	= poldrpc->rpc_respauth;
	pnewrpc->rpc_respuid	= poldrpc->rpc_respuid;
	pnewrpc->rpc_respgid	= poldrpc->rpc_respgid;

	return (OK);
}


/*----------------------------------------------------------------------------
 * rpccheckcall -- check the RPC headers and set ppch to the start of the data
 *----------------------------------------------------------------------------
 */
static
int rpccheckcall(prpc, ppch)
    struct rpcblk *prpc;
    char **ppch;
{
	char *pch;
	unsigned u;

	pch = *ppch;
    
	/* check the version number */
	pch = xdrgetunsigned(pch, &u);
	if (u != RPCVERS) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("rpccheckcall: wrong RPC version (%u)\n", u);
#endif
		rpcreply(prpc, (prpc->rpc_status = RPC_ERROR_RPCVERS));
		return (SYSERR);
	}

	pch = xdrgetunsigned(pch, &u);
	if (u != prpc->rpc_program) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("rpccheckcall: wrong RPC program (%u)\n", u);
#endif
		rpcreply(prpc, (prpc->rpc_status = RPC_ERROR_PROGUNAVAIL));
		return (SYSERR);
	}
    
	pch = xdrgetunsigned(pch, &u);
	prpc->rpc_respvers = u;
	if ((u < prpc->rpc_minversion) || (u > prpc->rpc_version)) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("rpccheckcall: invalid RPC program version (%u)\n", u);
#endif
		rpcreply(prpc, (prpc->rpc_status = RPC_ERROR_PROGMISMATCH));
		return (SYSERR);
	}

	/* Can't do any checking on the procedure */
	pch = xdrgetunsigned(pch, &prpc->rpc_procedure);

	/* Can only check to see that we have AUTH_UNIX or AUTH_NULL */
	if ((pch = rpcgetauth(pch, prpc)) == NULL) {
#ifdef DEBUG_VERBOSE
		kprintf("rpccheckcall: rpcgetauth() returned an error\n");
#endif
		return (SYSERR);
	}

	*ppch = pch;
	return (prpc->rpc_status = RPC_OK);
}


/*----------------------------------------------------------------------------
 * rpcgetauth -- process the cred and verf fields of a call message
 *----------------------------------------------------------------------------
 */
static
char *rpcgetauth(pch, prpc)
    char *pch;
    struct rpcblk *prpc;
{
	char nullbuf[RPC_MAX_AUTH_SIZE];
	unsigned u;
	unsigned len;

	pch = xdrgetunsigned(pch, &u);	/* flavor */

	prpc->rpc_respauth = u;
	switch (u) {
	      case RPC_AUTHFLAVOR_UNIX: {
		      unsigned stamp;
		      char mname[RPC_MAX_MACHINENAME_LEN];
		      unsigned gid;
		      unsigned count;
		      int i;
	    
		      pch = xdrgetunsigned(pch, &len); /* length */
		      pch = xdrgetunsigned(pch, &stamp);
#ifdef DEBUG_VERBOSE
		      kprintf("rpcgetauth: AUTH_UNIX: Stamp is 0x%08x\n", stamp);
#endif	    
		      pch = xdrgetstring(pch, mname, sizeof(mname));
	    
#ifdef DEBUG_VERBOSE
		      kprintf("      machine name: '%s'\n", mname);
#endif
	    
		      pch = xdrgetunsigned(pch, &prpc->rpc_respuid);
		      pch = xdrgetunsigned(pch, &prpc->rpc_respgid);
		      pch = xdrgetunsigned(pch, &count);
#ifdef DEBUG_VERBOSE
		      kprintf("      uid: %d   gid: %d  group count: %d\n",
			      prpc->rpc_respuid, prpc->rpc_respgid, count);
#endif	    
		      for (i = 0; i < count; ++i) {
			      pch = xdrgetunsigned(pch, &gid);
#ifdef DEBUG_VERBOSE
			      kprintf("           group[%d] gid: %d\n", i, gid);
#endif
		      }
	    
		      pch = xdrgetunsigned(pch, &u);
		      if (u != RPC_AUTHFLAVOR_NULL) {
#if defined(DEBUG) || defined(PRINTERRORS)
			      kprintf("rpcgetauth: verf no AUTH_NULL\n");
#endif
			      return (NULL);
		      }
#ifdef DEBUG_VERBOSE
		      kprintf("rpcgetauth: verf AUTH_NULL\n");
#endif
		      len = RPC_MAX_AUTH_SIZE;
		      pch = xdrgetvopaque(pch, nullbuf, &len);
#ifdef DEBUG
		      kprintf("rpcgetauth: len from xdrgetvopaque = %u\n", len);
#endif
	      }
		break;
	
	      case RPC_AUTHFLAVOR_NULL:
#ifdef DEBUG_VERBOSE
		kprintf("rpcgetauth: cred AUTH_NULL\n");
#endif
		/* cred */
		len = RPC_MAX_AUTH_SIZE;
		pch = xdrgetvopaque(pch, nullbuf, &len);
#ifdef DEBUG
		kprintf("rpcgetauth: len from xdrgetvopaque = %u\n", len);
#endif
		/* verf */
		pch = xdrgetunsigned(pch, &u);
		if (u != RPC_AUTHFLAVOR_NULL) {
#if defined(DEBUG) || defined(PRINTERRORS)
			kprintf("rpcgetauth: verf no AUTH_NULL\n");
#endif
			return (NULL);
		}
#ifdef DEBUG_VERBOSE
		kprintf("rpcgetauth: verf AUTH_NULL\n");
#endif
		len = RPC_MAX_AUTH_SIZE;
		pch = xdrgetvopaque(pch, nullbuf, &len);
#ifdef DEBUG
		kprintf("rpcgetauth: len from xdrgetvopaque = %u\n", len);
#endif
		break;
	
	      default:
		/* Let the server deal with it */
		break;
	}

	return (pch);
}
