/* 
 * rpcgetport.c - determine remote port using remote portmapper
 * 
 * Author:	Shawn Ostermann
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Tue Apr 23 22:36:01 1991
 *
 */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <mem.h>
#include <network.h>
#include <rpc.h>
#include <rpcprogs.h>
#include <portmapper.h>
#include <xdr.h>
#include <gcache.h>

/*#define DEBUG*/
#define PRINTERRORS

extern int pmaplookuprpc();

LOCAL int portcachelookup();
LOCAL int portcacheinsert();


/*-------------------------------------------------------------------------
 * rpcgetport - 
 *-------------------------------------------------------------------------
 */
rpcgetport(prpc)
     struct rpcblk *prpc;
{
    int dev;
    IPaddr ourIP;

	
#ifdef DEBUG
    kprintf("rpcgetport() called\n");
#endif

    /* check for fixed point, program == portmapper */
    if (prpc->rpc_program == RPCPROG_PMAP) {
#ifdef DEBUG
	kprintf("rpcgetport: program == PORTMAPPER, return %d\n", UP_RPC);
#endif
	prpc->rpc_rport = UP_RPC;
	return(OK);
    }

    /* broadcast always uses portmapper port */
    if (prpc->rpc_isbrc) {
#ifdef DEBUG
	kprintf("rpcgetport: hostIP == BROADCAST, return %d\n", UP_RPC);
#endif
	prpc->rpc_rport = UP_RPC;
	return(OK);
    }

    /* If the program is local, we don't need to contact the portmapper */
    getaddr(ourIP);
    if (IP_SAMEADDR(prpc->rpc_hostIP, ourIP)) {
	if ((dev = pmaplookuprpc(prpc->rpc_program, prpc->rpc_version,
				 IPT_UDP, 0)) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("rpcgetport: Local portmapper lookup failed\n");
#endif
	    return (SYSERR);
	}

	prpc->rpc_rport = rpctab[dev].rpc_lport;
    } else {
	unsigned port;
	char pmapstr[200];
	char bufIP[100];
	int ret;

	/* check the cache */
	if (portcachelookup(prpc) == OK)
	    return(OK);

	/* Will have to contact remote port mapper to find the port */
	sprintf(pmapstr,"%s:%d:%d:%d", ip2dot(bufIP, prpc->rpc_hostIP),
		RPCPROG_PMAP,PMAP_VERS,PMAP_PROC_GETPORT);

#ifdef DEBUG
	kprintf("rpcgetport(): calling open(RPC,%s,CLIENT)\n", pmapstr);
#endif
	dev = open(RPC, pmapstr, RPCT_CLIENT);

	control(dev, XSET_UINT, prpc->rpc_program);
	control(dev, XSET_UINT, prpc->rpc_version);
	control(dev, XSET_UINT, IPT_UDP);
	control(dev, XSET_UINT, 0);

#ifdef DEBUG
	kprintf("rpcgetport(): making call to portmapper\n");
#endif
	if ((ret = control(dev,RPC_CALL)) != OK) {
#ifdef DEBUG
	    kprintf("rpcgetport: RPC_CALL failed, returning %d\n", ret);
#endif
	    close(dev);
	    return(ret);
	}

	control(dev,XGET_UINT,&port);

#ifdef DEBUG
	kprintf("rpcgetport: returns port %d (0x%08x)\n", port, port);
#endif

	prpc->rpc_rport = port;

	close(dev);

	/* remember it */
	if (portcacheinsert(prpc));
    }

    return(OK);
}



static int portcid;
struct rpccache {
	IPaddr	ip;
	u_int	program;
	u_int	version;
};


/*-------------------------------------------------------------------------
 * portcachelookup - look in the generic cache for an addr
 *-------------------------------------------------------------------------
 */
LOCAL int portcachelookup(prpc)
     struct rpcblk *prpc;
{
    static MARKER rpcmark;
    struct rpccache rpcc;
    int len;

    if (unmarked(rpcmark)) {
	portcid = cacreate("rpcport", 40, CA_LIFETIME);
	mark(rpcmark);
    }

    IP_COPYADDR(rpcc.ip, prpc->rpc_hostIP);
    rpcc.program = prpc->rpc_program;
    rpcc.version = prpc->rpc_version;

    len = sizeof(unsigned);
    return(calookup(portcid, &rpcc, sizeof(struct rpccache),
		    &prpc->rpc_rport, &len));
}



/*-------------------------------------------------------------------------
 * portcacheinsert - insert a new entry in the cache
 *-------------------------------------------------------------------------
 */
LOCAL int portcacheinsert(prpc)
     struct rpcblk *prpc;
{
    struct rpccache rpcc;

    IP_COPYADDR(rpcc.ip,prpc->rpc_hostIP);
    rpcc.program = prpc->rpc_program;
    rpcc.version = prpc->rpc_version;

    return(cainsert(portcid, &rpcc, sizeof(struct rpccache),
		    &prpc->rpc_rport, sizeof(unsigned)));
}





