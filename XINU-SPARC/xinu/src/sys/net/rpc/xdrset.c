/* xdrset.c - xdrset */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <xdr.h>


/*#define DEBUG*/
#define PRINTERRORS


/*
 * ====================================================================
 * xdrset - convert a data item into XDR format in the request buffer
 * ====================================================================
 */
/*VARARGS3*/
xdrset(prpc, dtyp, data, optlen)
     struct	rpcblk	*prpc;
     int	dtyp;
     unsigned	data;
     unsigned	optlen;
{
    unsigned xdrlen;
    char	*pdata;
    char	*pch;
	
#ifdef DEBUG
    kprintf("xdrset(prpc,%s,0x%08x,0x%08x) called\n",
	    xdrt_names[dtyp], data, optlen);
#endif

    /* where the data will come from */
    pdata = prpc->rpc_reqxbuf + prpc->rpc_reqxix;

    /* set the length of the request */
    switch (dtyp) {
      case XDRT_UINT:
      case XDRT_ENUM:
      case XDRT_BOOL:
      case XDRT_INT:
      case XDRT_FLOAT:
	xdrlen = XDRUNSIGNEDLEN; break;
      case XDRT_DOUBLE:
	xdrlen = XDRDOUBLELEN; break;
      case XDRT_STRING:
	xdrlen = XDRUNSIGNEDLEN + strlen(data); break;
      case XDRT_FOPAQ:
	xdrlen = optlen; break;
      case XDRT_VOPAQ:
	xdrlen = XDRUNSIGNEDLEN + optlen; break;
    }

    /* make sure we have enough room */
    if ((xdrlen + prpc->rpc_reqxix) > prpc->rpc_reqxbufsize) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("xdrset called, no more room for args... FIXME\n");
#endif
	prpc->rpc_status = RPC_ERROR_BUFERROR;
	return(SYSERR);
    }


#ifdef DEBUG_OLD
    kprintf("xdrset(prpc,0x%x,0x%x) called\n", dtyp, data);
    kprintf("    reqxbuf: 0x%08x   reqxix: %d\n",
	    prpc->rpc_reqxbuf, prpc->rpc_reqxix);
#endif

    /* do the conversion */
    switch (dtyp) {
      case XDRT_UINT:
      case XDRT_ENUM:
      case XDRT_BOOL:
	pch = xdrsetunsigned(pdata, data); break; 
      case XDRT_INT:
	pch = xdrsetint(pdata, data); break;
      case XDRT_FLOAT:
	pch = xdrsetfloat(pdata, data); break;
      case XDRT_DOUBLE:
	pch = xdrsetdouble(pdata, data); break;
      case XDRT_STRING:
	pch = xdrsetstring(pdata, data, xdrlen - XDRUNSIGNEDLEN);
	break;
      case XDRT_FOPAQ:
	pch = xdrsetfopaque(pdata, data, optlen);
	break;
      case XDRT_VOPAQ:
	pch = xdrsetvopaque(pdata, data, optlen);
	break;
    }

    /* determine where the next write should start */
    prpc->rpc_reqxix = pch - prpc->rpc_reqxbuf;

    prpc->rpc_reqxlen = max(prpc->rpc_reqxlen, prpc->rpc_reqxix);
/*kprintf("<XDR>");    */
    return(OK);
}
