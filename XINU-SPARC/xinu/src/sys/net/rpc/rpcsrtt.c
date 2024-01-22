/* 
 * rpcsrrr.c - routines for dealing with RPC UPD reliability and estimates
 *             of round trip times
 */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <network.h>
#include <rpc.h>


/*#define DEBUG*/

/* info for cached per-host information */
#define MAX_LIFE	3600	/* 1 hour		*/
#define MAX_ENTRIES	50	/* they're small	*/

LOCAL int rtocid;
struct rto_resp {
	unsigned	srtt;
};

/* constants for the functions below */
/* SRTT = 7/8 SRTT + 1/8 RTT */
#define SRTT_SHIFT 3

/*
 * ====================================================================
 * rpcrtonew - insert the correct fields into the round trip time
 *             fields for a new UDP RPC connection.
 * ====================================================================
 */
rpcrtonew(prpc)
     struct rpcblk *prpc;
{
    LOCAL MARKER	rpcmark;
    struct rto_resp resp;
    int		len;


    /* create the cache if not already done */
    if (unmarked(rpcmark)) {
	rtocid = cacreate("rpc_rto", MAX_ENTRIES, MAX_LIFE);
	mark(rpcmark);
    }

    len = sizeof(resp);
    if (calookup(rtocid, prpc->rpc_hostIP, IP_ALEN, &resp, &len) == OK) {
	/* found it */
	prpc->rpc_SRTT = resp.srtt;
	prpc->rpc_RTO  = min((prpc->rpc_SRTT >> SRTT_SHIFT) << 1,
			     RPC_DEFAULT_MAXRTO);
    } else {
	/* use the default */
	prpc->rpc_RTO  = RPC_DEFAULT_RTO;
	prpc->rpc_SRTT = (RPC_DEFAULT_RTO << SRTT_SHIFT);
    }
}


/*
 * ====================================================================
 * rpcrtosave - save current RTO values for this rpc connection
 * ====================================================================
 */
rpcrtosave(prpc)
     struct rpcblk *prpc;
{
    struct rto_resp resp;

    resp.srtt = prpc->rpc_SRTT;

    (void) cainsert(rtocid, prpc->rpc_hostIP, IP_ALEN, &resp, sizeof(resp));

    return(OK);
}



/*
 * ====================================================================
 * rpcrtocompute - compute new RTO values based on recent request
 * ====================================================================
 */
rpcrtocompute(prpc)
     struct rpcblk *prpc;
{
    unsigned rtt;
	
    /* if we didn't get a response, double the rto */
    if (prpc->rpc_timein == 0) {
	prpc->rpc_SRTT << 1;
	prpc->rpc_RTO << 1;
	return(OK);
    }

    /* if we retransmitted, then the times are useless */
    if (prpc->rpc_trans != 1)
	return(OK);

    /* set the lower bound to 1 sec. */
    rtt = max(prpc->rpc_timein - prpc->rpc_timeout, 1);
#ifdef DEBUG
    kprintf("rpcrtocompute: rtt is %d\n", rtt);
#endif

    prpc->rpc_SRTT += rtt - (prpc->rpc_SRTT >> SRTT_SHIFT);
    /* RTO = 2 * SRTT */
    prpc->rpc_RTO  = (prpc->rpc_SRTT >> SRTT_SHIFT) << 1;

#ifdef DEBUG
    kprintf("rpcrtocompute: RTO:%d, SRTT: %d\n", prpc->rpc_RTO, prpc->rpc_SRTT);
#endif
    return(OK);
}

/*
 * ====================================================================
 * rpcrtogettime - return a time sec
 * ====================================================================
 */
rpcrtogettime()
{
    unsigned now;

    gettime(&now);	/* in second */
    return(now);
}
