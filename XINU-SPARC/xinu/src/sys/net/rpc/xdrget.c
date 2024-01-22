/* xdrget.c - xdrget */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <xdr.h>


/*#define DEBUG*/
#define PRINTERRORS

/*-------------------------------------------------------------------------
 * xdrget -  convert a data item in the incoming buffer from XDR format
 *-------------------------------------------------------------------------
 */
/*VARARGS3*/
xdrget(prpc, dtyp, data, optlen)
     struct	rpcblk	*prpc;
     int	dtyp;
     unsigned	data;
     unsigned	optlen;
{
    unsigned xdrlen;
    char	*pdata;
    char	*pch;
		
#ifdef DEBUG
    kprintf("xdrget(prpc,%s,0x%08x,0x%08x) called\n",
	    xdrt_names[dtyp], data, optlen);
#endif

    /* check for a valid packet to read from */
    if (!prpc->rpc_respxbuf) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("xdrget: no response buffer, return SYSERR\n");
#endif
	prpc->rpc_status = RPC_ERROR_BUFERROR;
	return(SYSERR);
    }

    /* where the data will come from */
    pdata = prpc->rpc_respxbuf + prpc->rpc_respxix;

    /* determine the length of the data to be returned */
    switch (dtyp) {
      case XDRT_UINT:
      case XDRT_ENUM:
      case XDRT_BOOL:
      case XDRT_INT:
      case XDRT_FLOAT:
	xdrlen = XDRUNSIGNEDLEN;
	break;
      case XDRT_DOUBLE:
	xdrlen = XDRDOUBLELEN;
	break;
      case XDRT_FOPAQ:
	xdrlen = optlen;
	break;
      case XDRT_STRING:
      case XDRT_VOPAQ:
	/* peek at the length field */
	(void) xdrgetunsigned(pdata,&xdrlen);
	break;
    }

    /* check to be sure we have that much data available */
    if ((prpc->rpc_respxix + xdrlen) > prpc->rpc_respxlen) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("xdrget(prpc,%d,0x%08x) ==> SYSERR, no more data\n",
		dtyp, data);
	kprintf("xdrget: respxix: %d\n", prpc->rpc_respxix);
	kprintf("xdrget: xdrlen: %d\n", xdrlen);
	kprintf("xdrget: respxlen: %d\n", prpc->rpc_respxlen);
#endif
	prpc->rpc_status = RPC_ERROR_BUFERROR;
	return(SYSERR);
    }

    /* do the conversion */
    switch (dtyp) {
      case XDRT_UINT:
      case XDRT_ENUM:
      case XDRT_BOOL:
	pch = xdrgetunsigned(pdata,data); break;
      case XDRT_INT:
	pch = xdrgetint(pdata,data); break;
      case XDRT_FLOAT:
	pch = xdrgetfloat(pdata,data); break;
      case XDRT_DOUBLE:
	pch = xdrgetdouble(pdata,data); break;
      case XDRT_STRING:
	pch = xdrgetstring(pdata,data,optlen); break;
      case XDRT_FOPAQ:
	pch = xdrgetfopaque(pdata,data,optlen); break;
      case XDRT_VOPAQ:
	pch = xdrgetvopaque(pdata,data,optlen); break;
    }

    /* determine where the next read should start */
    prpc->rpc_respxix = pch - prpc->rpc_respxbuf;
#ifdef DEBUG
    kprintf("xdrget: final xix: %d\n", prpc->rpc_respxix);
#endif

    return(OK);
}
