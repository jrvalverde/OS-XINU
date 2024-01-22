/* xdr.c - xdr marshalling routines */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <xdr.h>


/*#define DEBUG*/
/*#define DEBUG_REAL*/

char *xdrt_names[] = XDRT_NAMES;


/*==============================================================*/
/*								*/
/*	INTEGER							*/
/*								*/
/*==============================================================*/

char *xdrsetint(pxdr, x)
     char	*pxdr;
     int	x;
{
    *((int *) pxdr) = hl2net(x);
    return(pxdr + XDRINTLEN);
}


char * xdrgetint(pxdr, px)
     char	*pxdr;
     int	*px;
{
    if (px) 
	*px = hl2net(*((int *) pxdr));
    return(pxdr + XDRINTLEN);
}


/*==============================================================*/
/*								*/
/*	UNSIGNED						*/
/*								*/
/*==============================================================*/

char *xdrsetunsigned(pxdr, x)
     char	*pxdr;
     unsigned	x;
{
    *((unsigned *) pxdr) = hl2net(x);
    return(pxdr + XDRUNSIGNEDLEN);
}

char * xdrgetunsigned(pxdr, px)
     char	*pxdr;
     unsigned	*px;
{
    if (px)
	*px = hl2net(*((unsigned *) pxdr));
    return(pxdr + XDRUNSIGNEDLEN);
}


/*==============================================================*/
/*								*/
/*	FLOAT - single precision float				*/
/*								*/
/*==============================================================*/

char *xdrsetfloat(pxdr, x)
     char	*pxdr;
     float	x;
{
    float f;

    f = x;
#ifdef DEBUG_REAL
    kprintf("xdrsetfloat() called, &float look like:\n");
    rpcdumpbytes(&f, 8);
#endif
    *((unsigned *) pxdr) = hl2net(*((unsigned *) (&f)));
#ifdef DEBUG_REAL
    kprintf("xdrsetfloat produces:\n");
    rpcdumpbytes(pxdr, 8);
#endif
    return(pxdr + XDRFLOATLEN);
}

char *xdrgetfloat(pxdr, pf)
     char	*pxdr;
     float	*pf;
{
#ifdef DEBUG_REAL
    kprintf("xdrgetfloat(pxdr:0x%08x, pf) called, input is:\n", pxdr);
    rpcdumpbytes(pxdr,8);
#endif

    if (pf) {
	*((unsigned *) pf) = hl2net(*((unsigned *) pxdr));
    }
#ifdef DEBUG_REAL
    kprintf("xdrgetfloat produces:\n");
    rpcdumpbytes(pf,8);
#endif
    return(pxdr + XDRFLOATLEN);
}


/*==============================================================*/
/*								*/
/*	DOUBLE - double precision float				*/
/*								*/
/*==============================================================*/

char *xdrsetdouble(pxdr, x)
     char	*pxdr;
     double	x;
{
    double d;

    d = x;
#ifdef DEBUG_REAL
    kprintf("xdrsetdouble() called, &double looks like:\n");
    rpcdumpbytes(&d,XDRDOUBLELEN);
#endif
    *((unsigned *) (pxdr+0)) =
	hl2net(*((unsigned *) ((unsigned *)(&d)+1)));
    *((unsigned *) (pxdr+4)) =
	hl2net(*((unsigned *) ((unsigned *)(&d)+0)));
#ifdef DEBUG_REAL
    kprintf("xdrsetdouble produces:\n");
    rpcdumpbytes(pxdr,XDRDOUBLELEN);
#endif
    return(pxdr + XDRDOUBLELEN);
}

char *xdrgetdouble(pxdr, pd)
     char	*pxdr;
     double	*pd;
{
#ifdef DEBUG_REAL
    kprintf("xdrgetdouble(pxdr:0x%08x, pf) called, input is:\n", pxdr);
    rpcdumpbytes(pxdr,XDRDOUBLELEN);
#endif
    if (pd) {
	*((unsigned *) ((unsigned *)pd+1)) = hl2net(*((unsigned *) (pxdr+0)));
	*((unsigned *) ((unsigned *)pd+0)) = hl2net(*((unsigned *) (pxdr+4)));
    }
#ifdef DEBUG_REAL
    kprintf("xdrgetdouble produces:\n");
    rpcdumpbytes(pd,XDRDOUBLELEN);
#endif
    return(pxdr + XDRDOUBLELEN);
}



/*==============================================================*/
/*								*/
/*	STRING							*/
/*								*/
/*==============================================================*/

char *xdrsetstring(pxdr, pstr, len)
     char	*pxdr;
     char	*pstr;
     unsigned	len;
{
    /* store the length of the string */
    pxdr = xdrsetunsigned(pxdr, len);

    /* store the string */
    blkcopy(pxdr, pstr, len);
    pxdr += len;

    /* pad out to a 4 byte boundary */
    pxdr = (char *) ROUND_4BYTE(pxdr);

    return(pxdr);
}

char *xdrgetstring(pxdr, pstr, len)
     char	*pxdr;
     char	*pstr;
     unsigned	len;
{
    unsigned xdrlen;

    /* read the length of the string */
    pxdr = xdrgetunsigned(pxdr,&xdrlen);

    if (pstr) {
	--len;				/* leave room for the null in the string */
	len = min(len,xdrlen);
	blkcopy(pstr,pxdr,len);
	pstr += len;
	*pstr = NULLCH;
    }
    pxdr += xdrlen;

    /* pad out to a 4 byte boundary */
    pxdr = (char *) ROUND_4BYTE(pxdr);

    return(pxdr);
}



/*==============================================================*/
/*								*/
/*	FIXED OPAQUE						*/
/*								*/
/*==============================================================*/

char *xdrsetfopaque(pxdr, pbuf, len)
     char	*pxdr;
     char	*pbuf;
     unsigned	len;
{
    blkcopy(pxdr,pbuf,len);
    pxdr += len;

    /* pad out to a 4 byte boundary */
    pxdr = (char *) ROUND_4BYTE(pxdr);

    return(pxdr);
}

char *xdrgetfopaque(pxdr, pbuf, len)
     char	*pxdr;
     char	*pbuf;
     unsigned	len;
{
#ifdef DEBUG
    kprintf("xdrgetfopaque(pxdr:0x%08x, pbuf:0x%08x, len:%d) called\n",
	    pxdr, pbuf, len);
#endif

    if (pbuf)
	blkcopy(pbuf,pxdr,len);
    pxdr += len;

    /* pad out to a multiple of 4 bytes */
    pxdr = (char *) ROUND_4BYTE(pxdr);

    return(pxdr);
}



/*==============================================================*/
/*								*/
/*	VARIABLE OPAQUE						*/
/*								*/
/*==============================================================*/

char *xdrsetvopaque(pxdr, pbuf, len)
     char	*pxdr;
     char	*pbuf;
     unsigned	len;
{
    /* store the length of the data */
    pxdr = xdrsetunsigned(pxdr,len);

    /* store the data */
    blkcopy(pxdr,pbuf,len);
    pxdr += len;

    pxdr = (char *) ROUND_4BYTE(pxdr);	/* pad out to a 4 byte boundary */

    return(pxdr);
}

char *xdrgetvopaque(pxdr, pbuf, plen)
     char	*pxdr;
     char	*pbuf;
     unsigned	*plen;
{
    unsigned xdrlen;

    /* read the length of the data */
    pxdr = xdrgetunsigned(pxdr, &xdrlen);

    if (pbuf) {
	/* Design decision:				*/
	/* we ALWAYS return the length available, even	*/
	/* if it didn't all fit				*/
	blkcopy(pbuf, pxdr, min(xdrlen, *plen));
	*plen = xdrlen;
    }
    pxdr += xdrlen;

    pxdr = (char *) ROUND_4BYTE(pxdr);	/* pad out to a 4 byte boundary */

    return(pxdr);
}
