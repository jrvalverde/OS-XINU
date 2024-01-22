/* rpcreply.c -- rpcreply
 *
 * Author:	Brian K. Grant
 *		Dept. of Computer Sciences
 *		Purdue University
 *
 * Written:	11-5-91
 */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <xdr.h>
#include <rpcprogs.h>
#include <portmapper.h>
#include <proc.h>

#define MAX_RPC_NETWORK_HEADERS 150
/*#define DEBUG*/
/*#define DEBUG_VERBOSE*/


/*----------------------------------------------------------------------------
 * rpcreply -- reply to a RPC call with a message already set by rpccntl()
 *----------------------------------------------------------------------------
 */
int rpcreply(prpc, reply_stat)
    struct rpcblk *prpc;
    int reply_stat;
{
    unsigned len,
    	     maxlen;
    struct ep *pep_req;
    int ret;
    
    maxlen = prpc->rpc_reqxlen + MAX_RPC_NETWORK_HEADERS;

    /* check for reply TOO large */
    if (maxlen > IP_MAXLEN) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpcreply() reply too large: %d bytes\n", maxlen);
#endif
	/* invalidate the reply */
	prpc->rpc_reqxix = prpc->rpc_reqxlen = 0;
	return (SYSERR);
    }

    if (maxlen < NETBUFS) {
	pep_req = (struct ep *) getbuf(Net.netpool);
    } else {
	pep_req = (struct ep *) getbuf(Net.lrgpool);
    }
#ifdef RPC_BUF_DEBUG
    kprintf("rpcreply: getbuf gives 0x%x\n", pep_req);
    pep_enq(pep_req);
#endif

    if ((len = rpcmakereply(pep_req, prpc, reply_stat)) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpcreply() couldn't make reply\n");
#endif
#ifdef RPC_BUF_DEBUG
	kprintf("rpcreply: ");
#endif
	freebuf(pep_req);
	return (SYSERR);
    }

#ifdef DEBUG
    {
	char buf[100];
	ip2name(prpc->rpc_resphostIP, buf);
	kprintf("rpcreply: sending %d byte reply to '%d%%%s'\n",
		len, prpc->rpc_respport, buf);
    }
#endif

    ++prpc->rpc_sends;
#ifdef RPC_BUF_DEBUG
    kprintf("rpcreply: passing pep 0x%x to udpsend\n", pep_req);
#endif
    ret = udpsend(prpc->rpc_resphostIP, prpc->rpc_respport,
		  prpc->rpc_lport, pep_req, len, TRUE);

    if (ret != OK) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpcreply() error (%d) from udpsend\n", ret);
#endif
	return (SYSERR);
    }

    prpc->rpc_reqxix = prpc->rpc_reqxlen = 0;
    return (OK);
}


/*----------------------------------------------------------------------------
 * rpcmakereply -- set the fields of the reply to be sent
 *----------------------------------------------------------------------------
 */
static
int rpcmakereply(pep, prpc, reply_stat)
    struct ep *pep;
    struct rpcblk *prpc;
    int reply_stat;
{
    char *pch;
    struct ip *pip;
    struct udp *pudp;

    pip = (struct ip *) pep->ep_data;
    pudp = (struct udp *) pip->ip_data;
    pch = pudp->u_data;

    pch = xdrsetunsigned(pch, prpc->rpc_sequence); /* xid */
    pch = xdrsetunsigned(pch, RPC_MSGTYPE_REPLY);  /* mtype */

    switch (reply_stat) {
      case RPC_OK:
	pch = xdrsetunsigned(pch, RPC_REPLYSTAT_ACCEPTED);  	/* reply_stat */
	/* set the verf -- only UNIX and NULL are supported, also no SHORT */
	pch = xdrsetunsigned(pch, RPC_AUTHFLAVOR_NULL);     	/* flavor */
	pch = xdrsetunsigned(pch, 0);			    	/* length */
	pch = xdrsetunsigned(pch, RPC_ACCEPTSTAT_SUCCESS);  	/* accept_stat */
	blkcopy(pch, prpc->rpc_reqxbuf, prpc->rpc_reqxlen); 	/* reply info */
	pch += prpc->rpc_reqxlen;
	break;
	
      case RPC_ERROR_PROGMISMATCH:
	pch = xdrsetunsigned(pch, RPC_REPLYSTAT_ACCEPTED); 	/* reply_stat */
	/* set the verf -- only UNIX and NULL are supported, also no SHORT */
	pch = xdrsetunsigned(pch, RPC_AUTHFLAVOR_NULL);    	/* flavor */
	pch = xdrsetunsigned(pch, 0);			   	/* length */
	pch = xdrsetunsigned(pch, RPC_ACCEPTSTAT_PROGMISMATCH); /* accept_stat */
	pch = xdrsetunsigned(pch, prpc->rpc_minversion);	/* min. version */
	pch = xdrsetunsigned(pch, prpc->rpc_version);		/* max. version */
	break;
	
      case RPC_ERROR_PROGUNAVAIL:
	pch = xdrsetunsigned(pch, RPC_REPLYSTAT_ACCEPTED);	/* reply_stat */
	/* set the verf -- only UNIX and NULL are supported, also no SHORT */
	pch = xdrsetunsigned(pch, RPC_AUTHFLAVOR_NULL);		/* flavor */
	pch = xdrsetunsigned(pch, 0);				/* length */
	pch = xdrsetunsigned(pch, RPC_ACCEPTSTAT_PROGUNAVAIL);	/* accept_stat */
	break;
	
      case RPC_ERROR_PROCUNAVAIL:
	pch = xdrsetunsigned(pch, RPC_REPLYSTAT_ACCEPTED);	/* reply_stat */
	/* set the verf -- only UNIX and NULL are supported, also no SHORT */
	pch = xdrsetunsigned(pch, RPC_AUTHFLAVOR_NULL);		/* flavor */
	pch = xdrsetunsigned(pch, 0);				/* length */
	pch = xdrsetunsigned(pch, RPC_ACCEPTSTAT_PROCUNAVAIL);	/* accept_stat */
	break;
	
      case RPC_ERROR_GARBARGS:
	pch = xdrsetunsigned(pch, RPC_REPLYSTAT_ACCEPTED);	/* reply_stat */
	/* set the verf -- only UNIX and NULL are supported, also no SHORT */
	pch = xdrsetunsigned(pch, RPC_AUTHFLAVOR_NULL);		/* flavor */
	pch = xdrsetunsigned(pch, 0);				/* length */
	pch = xdrsetunsigned(pch, RPC_ACCEPTSTAT_GARBAGEARGS);	/* accept_stat */
	break;
	
      case RPC_ERROR_RPCVERS:
	pch = xdrsetunsigned(pch, RPC_REPLYSTAT_DENIED);	/* reply_stat */
	pch = xdrsetunsigned(pch, RPC_REJECTSTAT_RPCMISMATCH);	/* reject_stat */
	pch = xdrsetunsigned(pch, RPCVERS);			/* min. version */
	pch = xdrsetunsigned(pch, RPCVERS);			/* max. version */
	break;
	
      case RPC_ERROR_AUTH_BADCRED:
	pch = xdrsetunsigned(pch, RPC_REPLYSTAT_DENIED);	/* reply_stat */
	pch = xdrsetunsigned(pch, RPC_REJECTSTAT_AUTHERROR);	/* reject_stat */
	pch = xdrsetunsigned(pch, RPC_AUTHSTAT_BADCRED);	/* auth_stat */
	break;
	
      case RPC_ERROR_AUTH_REJECTEDCRED:
	pch = xdrsetunsigned(pch, RPC_REPLYSTAT_DENIED);	/* reply_stat */
	pch = xdrsetunsigned(pch, RPC_REJECTSTAT_AUTHERROR);	/* reject_stat */
	pch = xdrsetunsigned(pch, RPC_AUTHSTAT_REJECTEDCRED);	/* auth_stat */
	break;
	
      case RPC_ERROR_AUTH_BADVERF:
	pch = xdrsetunsigned(pch, RPC_REPLYSTAT_DENIED);	/* reply_stat */
	pch = xdrsetunsigned(pch, RPC_REJECTSTAT_AUTHERROR);	/* reject_stat */
	pch = xdrsetunsigned(pch, RPC_AUTHSTAT_BADVERF);	/* auth_stat */
	break;
	
      case RPC_ERROR_AUTH_REJECTEDVERF:
	pch = xdrsetunsigned(pch, RPC_REPLYSTAT_DENIED);	/* reply_stat */
	pch = xdrsetunsigned(pch, RPC_REJECTSTAT_AUTHERROR);	/* reject_stat */
	pch = xdrsetunsigned(pch, RPC_AUTHSTAT_REJECTEDVERF);	/* auth_stat */
	break;
	
      case RPC_ERROR_AUTH_TOOWEAK:
	pch = xdrsetunsigned(pch, RPC_REPLYSTAT_DENIED);	/* reply_stat */
	pch = xdrsetunsigned(pch, RPC_REJECTSTAT_AUTHERROR);	/* reject_stat */
	pch = xdrsetunsigned(pch, RPC_AUTHSTAT_TOOWEAK);	/* auth_stat */
	break;
	
      default:
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpcmakereply() unknown reply_stat (%d)\n", reply_stat);
#endif
	return (SYSERR);
    }

#ifdef DUMP_SENT_REPLIES
    rpcdump(pudp->u_data, pch - pudp->u_data);
#endif
    
    return (pch - pudp->u_data);
}
