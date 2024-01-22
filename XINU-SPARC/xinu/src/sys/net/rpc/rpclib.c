/* rpclib.c - rpcprintf, rpcscanf, rpclopen */

/************************************************************************/
/*									*/
/*     these are library routines that really belong elsewhere		*/
/*									*/
/************************************************************************/

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <xdr.h>
#include <varargs.h>


#define DEBUG
#define PRINTERRORS


/*
 * ====================================================================
 * rpcprintf - set multiple arguments into an RPC device using the
 *             format string and arguments given.
 * ====================================================================
 */
rpcprintf(dev, format, va_alist)
     int	dev;
     char	*format;
     va_dcl
{
    va_list args;

    va_start(args);
    if (control(dev, XSET_FORMAT, format, args) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpcprintf: failed to set arguments\n");
#endif
	return(SYSERR);
    }
    va_end(args);
    return(OK);
}



/*
 * ====================================================================
 * rpcscanf - read multiple fields from an RPC device according to the
 *            formatting string given.
 * ====================================================================
 */
rpcscanf(dev, format, va_alist)
     int	dev;
     char	*format;
     va_dcl
{
    va_list args;

    va_start(args);
    if (control(dev, XGET_FORMAT, format, args) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpcscanf: failed to get arguments\n");
#endif
	return(SYSERR);
    }
    va_end(args);
    return(OK);
}



/*
 * ====================================================================
 * rpclopen - open an RPC device with the parameters given
 * ====================================================================
 */
rpclopen(host,program,version,procedure,type)
     char *host;
     unsigned program;
     unsigned version;
     unsigned procedure;
{
    char	pmapstr[200];

    sprintf(pmapstr,"%s:%d:%d:%d",host,program,version,procedure);

    return(open(RPC,pmapstr,type));
}
