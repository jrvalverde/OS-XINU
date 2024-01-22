/* pmapproc.c -- portmapper routines
 *
 * Author:	Brian K. Grant
 *		Dept. of Computer Sciences
 *		Purdue University
 *
 * Written:	11-10-91
 */

#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <xdr.h>
#include <network.h>
#include <rpc.h>
#include <portmapper.h>
#include <rpcprogs.h>
#include <pmapd.h>

/*#define DEBUG*/
/*#define DEBUG_VERBOSE*/

/*----------------------------------------------------------------------------
 * pmaplookuprpc -- locate a particular RPC program in rpctab
 *----------------------------------------------------------------------------
 */
int pmaplookuprpc(prog, vers, prot, port)
    unsigned prog, vers, prot, port;
{
    int dev;
    struct rpcblk *prpc;

    dev = 0;
    
    for (dev = 0 ; dev < Nrpc ; ++dev) {
	prpc = &rpctab[dev];
	if ((prpc->rpc_state == RPCS_INUSE) &&
	    (prpc->rpc_type == RPCT_LSERVER) &&
	    (prpc->rpc_program == prog) &&
	    (prpc->rpc_protocol == prot) &&
	    ((prpc->rpc_version >= vers) ||
	     (prpc->rpc_minversion <= vers)) &&
	    ((port == 0) || (prpc->rpc_lport))) {

#ifdef DEBUG_VERBOSE
	    kprintf("pmaplookuprpc(): Found prog (server) %u at dnum = %d\n",
		    prog, dev);
#endif
	    return dev;
	}
    }

#ifdef DEBUG_VERBOSE
    kprintf("pmaplookuprpc(): Did not find prog %u\n", prog);
#endif
    return SYSERR;
}


/*----------------------------------------------------------------------------
 * pmapskipauth -- strip the authentication from a call (we don't care)
 *----------------------------------------------------------------------------
 */
int pmapskipauth(dev)
    int dev;
{
    unsigned u;

    control(dev, RPC_GETRAUTH, &u);
    switch (u) {
      case RPC_AUTHFLAVOR_UNIX:
      case RPC_AUTHFLAVOR_NULL:
	/* Already taken care of */
	break;
	
      default: /* unsupported authentication flavor */
	{
	    unsigned len;
	    char pch[RPC_MAX_AUTH_SIZE];
	    
	    /* Need to skip the cred and verf here */
	    len = RPC_MAX_AUTH_SIZE;
	    if (control(dev, XGET_VOPAQ, pch, &len) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("pmapskipauth: Error in getting the cred opaque field\n");
#endif
		return (SYSERR);
	    }
	    if (control(dev, XGET_UINT, &u) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("pmapskipauth: Error in getting the verf flavor\n");
#endif
		return (SYSERR);
	    }
	    len = RPC_MAX_AUTH_SIZE;
	    if (control(dev, XGET_VOPAQ, pch, &len) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("pmapskipauth: Error in getting the verf opaque field\n");
#endif
		return (SYSERR);
	    }
	}
	break;
    }

    return (OK);
}



/*----------------------------------------------------------------------------
 * pmapgetmapping -- extract a program mapping from a call message
 *----------------------------------------------------------------------------
 */
int pmapgetmapping(dev, pprog, pvers, pprot, pport)
    int dev;
    unsigned *pprog, *pvers, *pprot, *pport;
{
    if (control(dev, XGET_UINT, pprog) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("pmapgetmapping: Error in getting the program number\n");
#endif
	return (SYSERR);
    }
    if (control(dev, XGET_UINT, pvers) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("pmapgetmapping: Error in getting the version number\n");
#endif
	return (SYSERR);
    }
    if (control(dev, XGET_UINT, pprot) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("pmapgetmapping: Error in getting the protocol number\n");
#endif
	return (SYSERR);
    }
    if (control(dev, XGET_UINT, pport) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("pmapgetmapping: Error in getting the port number\n");
#endif
	return (SYSERR);
    }

    return (OK);
}


/*----------------------------------------------------------------------------
 * pmapset -- verify a server program mapping
 *----------------------------------------------------------------------------
 */
int pmapset(dev, prog, vers, prot, port)
    int dev;
    unsigned prog, vers, prot, port;
{
    /* We just check to see that the mapping is there */
    if (pmaplookuprpc(prog, vers, prot, port) == SYSERR) {
	control(dev, XSET_BOOL, FALSE);
#ifdef DEBUG_VERBOSE
	kprintf("pmapset: Program does not match any servers in rpctab\n");
#endif
	return (SYSERR);
    }

#ifdef DEBUG_VERBOSE
    kprintf("pmapset: Program match found\n");
#endif
    control(dev, XSET_BOOL, TRUE);
    return (OK);
}


/*----------------------------------------------------------------------------
 * pmapunset -- verify that a mapping is not listed with the portmapper
 *----------------------------------------------------------------------------
 */
int pmapunset(dev, prog, vers, prot, port)
    int dev;
    unsigned prog, vers, prot, port;
{
    /* We just check to see that the mapping is there */
    if (pmaplookuprpc(prog, vers, prot, port) != SYSERR) {
	control(dev, XSET_BOOL, FALSE);
#ifdef DEBUG_VERBOSE
	kprintf("pmapunset: Program still registered with portmapper.\n");
#endif
	return (SYSERR);
    }

#ifdef DEBUG_VERBOSE
    kprintf("pmapunset: Program match not found\n");
#endif
    control(dev, XSET_BOOL, TRUE);
    return (OK);
}


/*----------------------------------------------------------------------------
 * pmapgetport -- find the port of a server program
 *----------------------------------------------------------------------------
 */
int pmapgetport(dev, prog, vers, prot)
    int dev;
    unsigned prog, vers, prot;
{
    int mapdev;
    
    if ((mapdev = pmaplookuprpc(prog, vers, prot, 0)) == SYSERR) {
#ifdef DEBUG_VERBOSE
	kprintf("pmapgetport: Program not found\n");
#endif
	/* Signals an unregistered program */
	control(dev, XSET_UINT, 0);
    } else {
#ifdef DEBUG_VERBOSE
	kprintf("pmapgetport: Program found at dnum = %d, port = %u\n",
		mapdev, (unsigned) rpctab[mapdev].rpc_lport);
#endif
	control(dev, XSET_UINT, rpctab[mapdev].rpc_lport);
    }
    
    return (OK);
}


/*----------------------------------------------------------------------------
 * pmapdump -- dump a listing of all of the registered RPC server devices
 *----------------------------------------------------------------------------
 */
int pmapdump(dev)
    int dev;
{
    int curdev,
        vers;
    struct rpcblk *prpc;

    curdev = 0;
    
    for (curdev = 0 ; curdev < Nrpc ; ++curdev) {
	prpc = &rpctab[curdev];
	if ((prpc->rpc_state == RPCS_INUSE) &&
	    (prpc->rpc_type == RPCT_LSERVER)) {
	    for (vers = rpctab[curdev].rpc_minversion;
		 vers <= prpc->rpc_version; ++vers) {
		control(dev, XSET_BOOL, TRUE);
		control(dev, XSET_UINT, prpc->rpc_program);
		control(dev, XSET_UINT, vers);
		control(dev, XSET_UINT, prpc->rpc_protocol);
		control(dev, XSET_UINT, prpc->rpc_lport);
	    }
	}
    }
    control(dev, XSET_BOOL, FALSE);

    return (OK);
}


/*----------------------------------------------------------------------------
 * pmapgetcallargs -- extract the common call arguments from an RPC call msg
 *----------------------------------------------------------------------------
 */
int pmapgetcallargs(dev, pprog, pvers, pproc)
    int dev;
    unsigned *pprog, *pvers, *pproc;
{
    if (control(dev, XGET_UINT, pprog) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("pmapgetcallargs: Error in getting the program number\n");
#endif
	return (SYSERR);
    }
    if (control(dev, XGET_UINT, pvers) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("pmapgetcallargs: Error in getting the program number\n");
#endif
	return (SYSERR);
    }
    if (control(dev, XGET_UINT, pproc) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("pmapgetcallargs: Error in getting the program number\n");
#endif
	return (SYSERR);
    }

    return (OK);
}


/*----------------------------------------------------------------------------
 * pmapcallit -- call an RPC program for the client and return the results
 *
 *
 * From RFC 1057:
 *   (1) This procedure only sends a reply if the procedure was
 *   successfully executed and is silent (no reply) otherwise.
 *
 *   (2) The port mapper communicates with the remote program using UDP
 *   only.
 *
 *   The procedure returns the remote program's port number, and the reply
 *   is the reply of the remote procedure.
 *----------------------------------------------------------------------------
 */
int pmapcallit(dev, prog, vers, proc)
    int dev;
    unsigned prog, vers, proc;
{
    struct rpcblk *prpcclient;
    int client;
    IPaddr maddr;
    char strprog[128];
    char mname[RPC_MAX_MACHINENAME_LEN];
    unsigned status;

    /* Open the client RPC device */
    (void) getaddr(maddr);
    ip2name(maddr, mname);
    sprintf(strprog, "%s:%u:%u:%u", mname, prog, vers, proc);
    if ((client = open(RPC, strprog, RPCT_CLIENT)) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("pmapcallit: Could not open RPC client device\n");
#endif
	return (SYSERR);
    }
    prpcclient = (struct rpcblk *) devtab[client].dvioblk;

    /* Transfer the args */
    prpcclient->rpc_reqxlen = RPC_REQBUF_SIZE;
    if (control(dev, XGET_VOPAQ, prpcclient->rpc_reqxbuf,
		&prpcclient->rpc_reqxlen) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("pmapcallit: Error in getting the call arguments\n");
#endif
	return (SYSERR);
    }

    /* Make the call */
    if (control(client, RPC_CALL) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("pmapcallit: Error in making the RPC call\n");
#endif
	return (SYSERR);
    }

    /* Check the results and make the reply (or remain silent) */
    control(client, RPC_GETSTATUS, &status);
    if (status == RPC_OK) {
	struct rpcblk *prpc;
	struct rpcblk *prpcserver;

	prpc = (struct rpcblk *) devtab[dev].dvioblk;
	prpcserver = &rpctab[pmaplookuprpc(prog, vers, IPT_UDP, 0)];
	
	control(dev, XSET_UINT, prpcserver->rpc_lport);
	control(dev, XSET_UINT, prpcclient->rpc_respxlen);
	if (control(client, XGET_FOPAQ, prpc->rpc_reqxbuf + prpc->rpc_reqxix,
		    prpcclient->rpc_respxlen) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("pmapcallit: Error in getting the call results: len = %u\n",
		    prpcclient->rpc_respxlen);
#endif
	    return (SYSERR);
	}
	prpc->rpc_reqxlen += prpcclient->rpc_respxlen;

	close(client);
	return (OK);
    }

#ifdef DEBUG_VERBOSE
    kprintf("pmapcallit: Call status is not ok: %d\n", status);
#endif
    close(client);
    return (SYSERR);
}
