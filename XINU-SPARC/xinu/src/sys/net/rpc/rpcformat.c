/* rpcformat.c - rpcformat */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <xdr.h>
#include <ctype.h>


/*#define DEBUG*/
#define PRINTERRORS


/* local routines */
LOCAL int rpcparseformat();
LOCAL int Do4byte();
LOCAL int DoDouble();
LOCAL int DoFopaq();
LOCAL int DoVopaq();
LOCAL int ParseError();

/* global routines used */
extern int xdrget();
extern int xdrset();

static struct fstate {
	char		*informat;	/* original format string	*/
	char		*format;	/* current pointer to ""	*/
	Bool		instruct;	/* in a structure??		*/
	Bool		fget;		/* get or set??			*/
	int		(*pfunc)();	/* pointer to xdrget or xdrset	*/
	char		*structptr;	/* struct argument pointer	*/
	unsigned	*args;		/* arguments passed		*/
	int		type;		/* current data type		*/
	int		count;		/* current repeat count		*/
};


/*
 * ====================================================================
 * rpcformat - handle the GET/SET FORMAT control call
 * ====================================================================
 */
rpcformat(prpc, fget, format, args)
     struct rpcblk *prpc;
     Bool fget;
     char *format;
     unsigned *args;
{
    int ret;
    struct fstate fstate;

#ifdef DEBUG
    kprintf("rpcformat(prpc,%s,'%s',(", fget?"GET":"SET", format);
    for (ret=0; ret<10; ++ret)
	kprintf("0x%x, ", args[ret]);
    kprintf("... )) called\n");
#endif
    fstate.instruct = FALSE;
    fstate.fget = fget;
    fstate.pfunc = fget?xdrget:xdrset;
    fstate.args = (unsigned *) args;
    fstate.structptr = NULL;
    fstate.informat = fstate.format = format;

    while (1) {
	if (rpcparseformat(&fstate) == SYSERR) {
	    prpc->rpc_status = RPC_ERROR_FORMAT;
	    return(SYSERR);
	}

	if (fstate.format == NULL)
	    return(OK);

	while (--fstate.count >= 0) {
	    switch (fstate.type) {
	      case XDRT_UINT:
	      case XDRT_ENUM:
	      case XDRT_BOOL:
	      case XDRT_INT:
	      case XDRT_FLOAT:
		ret = Do4byte(prpc,&fstate);
		break;
	      case XDRT_DOUBLE:
		ret = DoDouble(prpc,&fstate);
		break;
	      case XDRT_FOPAQ:
		ret = DoFopaq(prpc,&fstate);
		break;
	      case XDRT_VOPAQ:
	      case XDRT_STRING:
		ret = DoVopaq(prpc,&fstate);
		break;
	    } /* end of switch(type) */

	    /* check the return value */
	    if (ret != OK) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("rpcformat: bad return from xdrget/xdrset\n");
#endif
		return(SYSERR);
	    }
	}
    }
}
     



/*
 * ====================================================================
 * rpcparseformat - return the data from the next portion of a format
 *                  string from a GET or SET control call.  Each
 *                  segment is of the form:
 *                     [0-9]*%<FORMAT-CHAR>
 * ====================================================================
 */
LOCAL rpcparseformat(pfs)
     struct fstate *pfs;
{
    pfs->count = 1;
    pfs->type = -1;

    /* skip white space */
    while (isspace(*pfs->format))
	++pfs->format;

    /* look for "struct" chars */
    if ((*pfs->format == '{') || (*pfs->format == '}')) {
	if (*pfs->format == '{') {
	    if (pfs->instruct) {
		ParseError(pfs, "illegal recursive structure");
		return(SYSERR);
	    }
	    pfs->instruct = TRUE;
	    pfs->structptr = (char *) (pfs->args[0]);
	    ++pfs->args;
	} else {
	    if (!pfs->instruct) {
		ParseError(pfs, "closed non-existent struct");
		return(SYSERR);
	    }
	    pfs->instruct = FALSE;
	}
	++pfs->format;
	/* skip white space */
	while (isspace(*pfs->format))
	    ++pfs->format;
    }
	
    /* done when string is empty */
    if (*pfs->format == NULLCH) {
	pfs->format = NULL;
	return(OK);
    }
	

    /* might be an integer for the repeat count */
    if (isdigit(*pfs->format)) {
	if (!pfs->instruct) {
	    ParseError(pfs, "repeat count outside struct");
	    return(SYSERR);
	}
	pfs->count = atoi(pfs->format);
	while (isdigit(*pfs->format))
	    ++pfs->format;
    }

    /* now, there MUST be a '%' */
    if (*pfs->format != '%') {
	ParseError(pfs, "expected '%'");
	return(SYSERR);
    }
    ++pfs->format;

    /* now we look for one of the format characters */
    switch (*pfs->format) {
      case 'U':
	pfs->type = XDRT_UINT; break;
      case 'E':
	pfs->type = XDRT_ENUM; break;
      case 'B':
	pfs->type = XDRT_BOOL; break;
      case 'I':
	pfs->type = XDRT_INT; break;
      case 'R':
	pfs->type = XDRT_FLOAT; break;
      case 'D':
	pfs->type = XDRT_DOUBLE; break;
      case 'S':
	pfs->type = XDRT_STRING; break;
      case 'F':
	pfs->type = XDRT_FOPAQ; break;
      case 'V':
	pfs->type = XDRT_VOPAQ; break;
      default:
	pfs->type = -1;
	ParseError(pfs, "unknown format char");
	return(SYSERR);
    }
    ++pfs->format;

    return(OK);
}



/*
 * ====================================================================
 * Do4byte - for generic 4 byte things
 * ====================================================================
 */
LOCAL Do4byte(prpc,pfs)
     struct rpcblk *prpc;
     struct fstate *pfs;
{
    int ret;
    unsigned x, *px;

    if (pfs->instruct) {
	if (pfs->fget) {
	    px = (unsigned *) pfs->structptr;
	    ret = xdrget(prpc,pfs->type,px);
	} else {
	    x = *((unsigned *) (pfs->structptr));
	    ret = xdrset(prpc,pfs->type,x);
	}
	pfs->structptr += XDRUNSIGNEDLEN;
    } else {
	x = pfs->args[0];
	++pfs->args;
	ret = (*pfs->pfunc)(prpc,pfs->type,x);
    }
    return(ret);
}



/*
 * ====================================================================
 * DoDouble - for double quantities
 * ====================================================================
 */
LOCAL DoDouble(prpc,pfs)
     struct rpcblk *prpc;
     struct fstate *pfs;
{
    int ret;
    double d, *pd;

    if (pfs->instruct) {
	if (pfs->fget) {
	    pd = (double *) pfs->structptr;
	    ret = xdrget(prpc,pfs->type,pd);
	} else {
	    d = *((double *) (pfs->structptr));
	    ret = xdrset(prpc,pfs->type,d);
	}
	pfs->structptr += XDRDOUBLELEN;
    } else {
	if (pfs->fget) {
	    pd = (double *) pfs->args[0];
	    ++pfs->args;
	    ret = xdrget(prpc,pfs->type,pd);
	} else {
	    d = *((double *) pfs->args);
	    pfs->args = (unsigned *)
		((char *) pfs->args + XDRDOUBLELEN);
	    ret = xdrset(prpc,pfs->type,d);
	}
    }
    return(ret);
}


/*
 * ====================================================================
 * DoFopaq - for fopaq quantities
 * ====================================================================
 */
LOCAL DoFopaq(prpc,pfs)
     struct rpcblk *prpc;
     struct fstate *pfs;
{
    int ret;
    char *pch;
    unsigned width;

    if (pfs->instruct) {
	pch = pfs->structptr;
	width = pfs->args[0];
	++pfs->args;
	pfs->structptr += width;
	ret = (*pfs->pfunc)(prpc,pfs->type,pch,width);
    } else {
	pch = (char *) (pfs->args[0]);
	width = pfs->args[1];
	pfs->args += 2;
	ret = (*pfs->pfunc)(prpc,pfs->type,pch,width);
    }
    return(ret);
}



/*
 * ====================================================================
 * DoVopaq - for vopaq quantities (and strings)
 * ====================================================================
 */
LOCAL DoVopaq(prpc,pfs)
     struct rpcblk *prpc;
     struct fstate *pfs;
{
    int ret;
    char *pch;
    int width, *pwidth;

    if (pfs->instruct) {
	pch = pfs->structptr;
	if (pfs->fget) {
	    pwidth = (int *) (pfs->args[0]);
	    pfs->structptr += *pwidth;
	    ret = xdrget(prpc,pfs->type,pch,pwidth);
	} else {
	    width = pfs->args[0];
	    pfs->structptr += width;
	    ret = xdrset(prpc,pfs->type,pch,width);
	}
	++pfs->args;
    } else {
	pch =   (char *) pfs->args[0];
	width = pfs->args[1];
	pfs->args += 2;
	ret = (*pfs->pfunc)(prpc,pfs->type,pch,width);
    }
    return(ret);
}




LOCAL ParseError(pfs,str)
     struct fstate *pfs;
     char *str;
{
#if defined(DEBUG) || defined(PRINTERRORS)
    int i;
	
    kprintf("rpcparse ERROR:\n");
    kprintf("    %s\n    ", pfs->informat);
    for (i=0; i < (pfs->format - pfs->informat); ++i)
	kprintf(" ");
    kprintf("^    %s\n", str);
#endif
}





