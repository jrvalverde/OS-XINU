/* rpccntl.c - rpccntl */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <xdr.h>
#include <ctype.h>

#define DEBUG
#define PRINTERRORS

/* local routines */
LOCAL int rpcgetrhost();
LOCAL int rpcgetrport();


/*-------------------------------------------------------------------------
 * rpccntl - 
 *-------------------------------------------------------------------------
 */
rpccntl(devptr, func, arg, arg2)
     struct	devsw	*devptr;
     int		func;
     int		arg;
     int		arg2;
{
    struct	rpcblk	*prpc;
    int ret;

    prpc = (struct rpcblk *) devptr->dvioblk;

    ret = OK;				/* optimism is a virtue */
    prpc->rpc_status = RPC_OK;

    switch (func) {
      case XSET_INT:	ret = xdrset(prpc, XDRT_INT, arg); break;
      case XSET_UINT:	ret = xdrset(prpc, XDRT_UINT, arg); break;
      case XSET_ENUM:	ret = xdrset(prpc, XDRT_ENUM, arg); break;
      case XSET_BOOL:	ret = xdrset(prpc, XDRT_BOOL, arg); break;
      case XSET_FLOAT:	ret = xdrset(prpc, XDRT_FLOAT, arg); break;
      case XSET_DOUBLE:	ret = xdrset(prpc, XDRT_DOUBLE, arg); break;
      case XSET_STRING:	ret = xdrset(prpc, XDRT_STRING, arg, arg2); break;
      case XSET_FOPAQ:	ret = xdrset(prpc, XDRT_FOPAQ, arg, arg2); break;
      case XSET_VOPAQ:	ret = xdrset(prpc, XDRT_VOPAQ, arg, arg2); break;
      case XSET_FORMAT:	ret = rpcformat(prpc, FALSE, arg, arg2); break;

      case XGET_INT:	ret = xdrget(prpc, XDRT_INT, arg); break;
      case XGET_UINT:	ret = xdrget(prpc, XDRT_UINT, arg); break;
      case XGET_ENUM:	ret = xdrget(prpc, XDRT_ENUM, arg); break;
      case XGET_BOOL:	ret = xdrget(prpc, XDRT_BOOL, arg); break;
      case XGET_FLOAT:	ret = xdrget(prpc, XDRT_FLOAT, arg); break;
      case XGET_DOUBLE:	ret = xdrget(prpc, XDRT_DOUBLE, arg); break;
      case XGET_STRING:	ret = xdrget(prpc, XDRT_STRING, arg, arg2); break;
      case XGET_FOPAQ:	ret = xdrget(prpc, XDRT_FOPAQ, arg, arg2); break;
      case XGET_VOPAQ:	ret = xdrget(prpc, XDRT_VOPAQ, arg, arg2); break;
      case XGET_FORMAT:	ret = rpcformat(prpc, TRUE, arg, arg2); break;

      case RPC_CALL:
	if (prpc->rpc_type != RPCT_CLIENT) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("rpccntl: Only clients may perform an RPC_CALL.\n");
#endif
	    ret = SYSERR;
	    break;
	}
	ret = rpccall(prpc); break;
      case XDR_ERASE:
	prpc->rpc_reqxlen = 0;
	prpc->rpc_reqxix = 0;
	break;
      case XDR_REWIND:
	prpc->rpc_reqxix = 0;
	break;
      case RPC_PROGVERS:
	ret = prpc->rpc_version; break;
      case RPC_SETPROC:
	prpc->rpc_procedure = arg; break;
      case RPC_SETRTO:
	prpc->rpc_RTO = arg; break;
      case RPC_SETMAXTRANS:
	prpc->rpc_maxtrans = arg; break;
      case RPC_GETRTO:
	*((unsigned *) arg) = prpc->rpc_RTO; break;
      case RPC_GETMAXTRANS:
	*((unsigned *) arg) = prpc->rpc_maxtrans; break;
      case RPC_NEXTRESP:
	if (prpc->rpc_type != RPCT_CLIENT) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("rpccntl: Only clients may perform an RPC_NEXTRESP.\n");
#endif
	    ret = SYSERR;
	    break;
	}
	ret = rpcnextresp(prpc,prpc->rpc_nextTO); break;
      case RPC_SETNEXTTO:
	prpc->rpc_nextTO = arg; break;
      case RPC_LISTEN:
	if (prpc->rpc_type != RPCT_LSERVER) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("rpccntl: Only listening servers may perform an RPC_LISTEN.\n");
#endif
	    ret = SYSERR;
	    break;
	}
	ret = rpclisten(prpc,arg); break;
      case RPC_REPLY:
	if (prpc->rpc_type == RPCT_CLIENT) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("rpccntl: Only servers may perform an RPC_REPLY.\n");
#endif
	    ret = SYSERR;
	    break;
	}
	ret = rpcreply(prpc,arg); break;
      case RPC_GETHOST:
	ret = rpcgetrhost(prpc,arg); break;
      case RPC_GETPORT:
	ret = rpcgetrport(prpc,arg); break;
      case RPC_SETAUTH:
	prpc->rpc_reqauth = arg;
	prpc->rpc_requid = XINU_UID;
	prpc->rpc_reqgid = XINU_GID;
	break;
      case RPC_GETSTATUS:
	*((unsigned *) arg) = prpc->rpc_status; break;
      case RPC_GETRAUTH:
	*((unsigned *) arg) = prpc->rpc_respauth; break;
      case RPC_GETRUID:
	*((unsigned *) arg) = prpc->rpc_respuid; break;
      case RPC_GETRGID:
	*((unsigned *) arg) = prpc->rpc_respgid; break;
      case RPC_GETXID:
	*((unsigned *) arg) = prpc->rpc_sequence; break;
      case RPC_GETPROC:
	*((unsigned *) arg) = prpc->rpc_procedure; break;
      case RPC_GETRVERS:
	*((unsigned *) arg) = prpc->rpc_respvers; break;

      default:
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpccntl ==> SYSERR, unknown control: %d\n", func);
#endif
	ret = SYSERR;
	break;
    }

    return(ret);
}


/*-------------------------------------------------------------------------
 * rpcgetrhost - 
 *-------------------------------------------------------------------------
 */
LOCAL int rpcgetrhost(prpc,hostIP)
     struct rpcblk	*prpc;
     IPaddr		hostIP;
{
    if (!prpc->rpc_resppep) {
	prpc->rpc_status = RPC_ERROR_BUFERROR;
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpcgetrhost ==> SYSERR, no response buffer\n");
#endif
	return(SYSERR);
    }
	

    IP_COPYADDR(hostIP,prpc->rpc_resphostIP);
    return(OK);
}


/*-------------------------------------------------------------------------
 * rpcgetrport - 
 *-------------------------------------------------------------------------
 */
LOCAL int rpcgetrport(prpc, pport)
     struct rpcblk	*prpc;
     unsigned int	*pport;
{
    if (!prpc->rpc_resppep) {
	prpc->rpc_status = RPC_ERROR_BUFERROR;
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpcgetrport ==> SYSERR, no response buffer\n");
#endif
	return(SYSERR);
    }

    *pport = prpc->rpc_respport;

    return(OK);
}
