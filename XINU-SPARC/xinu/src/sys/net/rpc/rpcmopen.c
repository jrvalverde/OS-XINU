/* rpcmopen.c - rpcmopen - rpcalloc */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <mark.h>
#include <rpc.h>
#include <portmapper.h>
#include <rpcprogs.h>
#include <pmapd.h>

/*#define DEBUG*/

extern MARKER rpcmark;

/* local routines */
static rpcopen();
static int rpcparsecargs();
static int rpcparsesargs();

int rpcalloc();
int rpcprepdev();


/*------------------------------------------------------------------------
 *  rpcmopen  -  open a fresh RPC pseudo device and return descriptor
 *     type: RPCT_CLIENT
 *           strprog: "host:program:version:procedure"
 *              host: IP address or domain name of the host to contact,
 *                          or '*' for broadcast.
 *              program: program number (NOT NAME)
 *              version: version number
 *              procedure: procedure number (defaults to 0)
 *
 *     type: RPCT_LSERVER
 *	     strprog: "program:version" |
 *		      "program:minversion-version"
 *              program: program number
 *		minversion: minimum supported version number
 *              version: maximum (or only) supported version number
 *------------------------------------------------------------------------
 */
int rpcmopen(pdev, strprog, type)
     struct	devsw	*pdev;
     char		*strprog;
     int		type;
{
    struct	rpcblk	*prpc;
    int slot;
    int ret;

#ifdef DEBUG
    kprintf("rpcmopen(dev,\"%s\",%s) called\n",
	    strprog, type==RPCT_CLIENT?"client":"listening server");
#endif

    if ((type != RPCT_CLIENT) && (type != RPCT_LSERVER)) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpcmopen(dev,\"%s\",%d) returns SYSERR, bad type\n",
		strprog, type);
#endif
	return(SYSERR);
    }

    /* find an available pseudo device */
    if ((slot = rpcalloc()) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpcmopen ==> SYSERR, no more pseudo devices\n");
#endif
	return(SYSERR);
    }
    prpc = &rpctab[slot];

    /* create the mutex */
    prpc->rpc_mutex = screate(1);

    /* all device numbers are >= 0 */
    if ((ret = rpcopen(prpc, strprog, type)) < 0) {
	prpc->rpc_state = RPCS_FREE;
	sdelete(prpc->rpc_mutex);
    }
#ifdef DEBUG
    kprintf("rpcmopen: ret= %d\n", ret);
#endif
    return(ret);
}


/*----------------------------------------------------------------------------
 * rpcopen -- open an RPC client or listening server device
 *----------------------------------------------------------------------------
 */
static int rpcopen(prpc, strprog, type)
    struct rpcblk *prpc;
    char *strprog;
    int type;
{
    prpc->rpc_type = type;
    if (type == RPCT_CLIENT) {
	if (rpcparsecargs(prpc, strprog) == SYSERR) {
	    /* client type, grab the arguments */
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("rpcopen ==> SYSERR, can't parse args\n");
#endif
	    return(SYSERR);
	}
    } else if (rpcparsesargs(prpc, strprog) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpcopen ==> SYSERR, can't parse args\n");
#endif
	return(SYSERR);
    }

#ifdef DEBUG
    kprintf("rpcopen(dev,%s) ==> device %d\n", strprog, prpc->rpc_dnum);
#endif

#ifdef DEBUG
    {
	char dotbuf[100];

	kprintf("   hostIP: %s\n", ip2dot(dotbuf,prpc->rpc_hostIP));
	kprintf("   program: %d\n", prpc->rpc_program);
	kprintf("   version: %d\n", prpc->rpc_version);
	if (type == RPCT_CLIENT) {
	    kprintf("   procedure: %d\n", prpc->rpc_procedure);
	}
    }
#endif

    /* determine the remote protocol port */
    if (type == RPCT_CLIENT) {
	int ret;

	if ((ret = rpcgetport(prpc)) != OK) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("rpcopen: bad return from getport: %d\n", ret);
#endif
	    return(SYSERR);
	}

	/* make sure that it was registered */
	if (prpc->rpc_rport == 0) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("rpcopen: prog not registered\n");
#endif
	    return(RPC_ERROR_PROGUNAVAIL);
	}
    } else if (type == RPCT_LSERVER) {
	prpc->rpc_rport = 0;
	prpc->rpc_hostIP[0] = prpc->rpc_hostIP[1] = prpc->rpc_hostIP[2] =
	    prpc->rpc_hostIP[3] = 0;
    }

    if (rpcprepdev(prpc) == SYSERR) {
#ifdef DEBUG
	kprintf("rpcopen: rpcprepdev() returned an error\n");
#endif
	return (SYSERR);
    }

    if (unmarked(rpcmark) && (type == RPCT_LSERVER)) {
	mark(rpcmark);
	if (SYSERR == resume(kcreate(PMAPD,
				     PMAPSTK,
				     PMAPPRI,
				     PMAPDNAM,
				     0))) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("rpcmopen: Could not create pmapd\n");
#endif
	}
    }
    return (prpc->rpc_dnum);
}


/*----------------------------------------------------------------------------
 * rpcprepdev -- set up an RPC device of any type
 *----------------------------------------------------------------------------
 */
int rpcprepdev(prpc)
    struct rpcblk *prpc;
{
    /* set up the "response" marshalling buffer */
    prpc->rpc_resppep = (struct ep *) NULL;
    prpc->rpc_respxbufsize = 0;
    prpc->rpc_respxbuf = NULL;
    prpc->rpc_sequence = 0;
    prpc->rpc_respxix = 0;
    prpc->rpc_respxlen = 0;

    /* grab a UDP device to use */
    prpc->rpc_protocol = IPT_UDP;
    if (prpc->rpc_type == RPCT_LSERVER) {
	int port;

	if (prpc->rpc_program == PMAP_PROG) {
	    port = PMAP_PORT;
	} else {
	    port = ANYRLPORT;
	}
	if ((prpc->rpc_udpdev = open(UDP,ANYFPORT,port)) < 0) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("rpcprepdev: bad return from open(UDP...): %d\n",
		    prpc->rpc_udpdev);
#endif
	    return(SYSERR);
	}
    } else {
	char faddr[28];

	sprintf(faddr,"%u.%u.%u.%u:%u",
		(unsigned) prpc->rpc_hostIP[0],
		(unsigned) prpc->rpc_hostIP[1],
		(unsigned) prpc->rpc_hostIP[2],
		(unsigned) prpc->rpc_hostIP[3],
		(unsigned) prpc->rpc_rport);
	if ((prpc->rpc_udpdev = open(UDP,faddr,ANYRLPORT)) < 0) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("rpcprepdev: bad return from open(UDP...): %d\n",
		    prpc->rpc_udpdev);
#endif
	    return(SYSERR);
	}
    }

#ifdef DEBUG
    kprintf("rpcprepdev: using UDP device %d\n", prpc->rpc_udpdev);
#endif

    prpc->rpc_lport =
	((struct dgblk *)(devtab[prpc->rpc_udpdev].dvioblk))->dg_lport;

#ifdef DEBUG
    kprintf("rpcprepdev: using local port %d\n", prpc->rpc_lport);
#endif


    /* set up the "request" marshalling buffer */
    prpc->rpc_reqxbufsize = RPC_REQBUF_SIZE;
    prpc->rpc_reqxbuf = (char *) getmem(prpc->rpc_reqxbufsize);
    prpc->rpc_sequence = 0;
    prpc->rpc_reqxix = 0;
    prpc->rpc_reqxlen = 0;

    /* set the reliability constraints for clients */
    if (prpc->rpc_type == RPCT_CLIENT) {
	prpc->rpc_nextTO = RPC_DEFAULT_NEXTTO;
	prpc->rpc_maxtrans = RPC_DEFAULT_MAXTRANS;
	rpcrtonew(prpc);
    }

    /* currently, we only support AUTH_UNIX			*/
    /* (for security reasons, uid and gid are NOT resettable)	*/
    prpc->rpc_reqauth = RPC_AUTHFLAVOR_UNIX;
    prpc->rpc_requid  = XINU_UID;
    prpc->rpc_reqgid  = XINU_GID;

    return (OK);
}


/*------------------------------------------------------------------------
 *  rpcalloc  -  allocate an RPC psuedo device; return its descriptor
 *------------------------------------------------------------------------
 */
int rpcalloc()
{
    struct	rpcblk	*prpc;
    int		i;

#ifdef DEBUG
    kprintf("rpcalloc() called\n");
#endif

    wait(rpcmutex);

    for (i=0 ; i < Nrpc ; ++i) {
	prpc = &rpctab[i];
	if (prpc->rpc_state == RPCS_FREE) {
	    prpc->rpc_state = RPCS_INUSE;
	    signal(rpcmutex);
#ifdef DEBUG
	    kprintf("rpcalloc: returning free slot %d\n", i);
#endif
	    return i;
	}
    }
    signal(rpcmutex);
#ifdef DEBUG
    kprintf("rpcalloc: no free devices, returning SYSERR\n");
#endif
    return SYSERR;
}


/*-------------------------------------------------------------------------
 * rpcparsecargs - parse client argument string
 *-------------------------------------------------------------------------
 */
static int rpcparsecargs(prpc, str)
     struct rpcblk	*prpc;
     char		*str;	/* host:program:version:procedure */
{
    char	*pch;
    char	*index();
    char	host[100];
    int		i;

    /* skip to program */
    if ((pch = index(str,':')) == NULL) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpcparsecargs ==> SYSERR, no initial colon\n");
#endif
	return(SYSERR);
    }

    prpc->rpc_program = atoi(pch+1);

    /* skip to version */
    if ((pch = index(pch+1,':')) == NULL) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpcparsecargs ==> SYSERR, no version colon\n");
#endif
	return(SYSERR);
    }

    prpc->rpc_version = atoi(pch+1);

    /* skip to  (optional) procedure */
    if ((pch = index(pch+1,':')) == NULL) 
	prpc->rpc_procedure = 0;
    else
	prpc->rpc_procedure = atoi(pch+1);

    /* parse the host name */
    for (i=0; i < sizeof(host); ++i) {
	host[i] = *str++;
	if (*str == ':') {
	    host[i+1] = '\00';
	    break;
	}
    }

#ifdef DEBUG
    kprintf("rpcparsecargs:ver:%d,proc:%d,host:%s\n",  prpc->rpc_version,
	    prpc->rpc_procedure, host);
#endif

    prpc->rpc_isbrc = FALSE;
    if (strcmp(host, RPC_EVERYHOST) == 0) {
	blkcopy(prpc->rpc_hostIP, nif[NI_PRIMARY].ni_brc, IP_ALEN);
	prpc->rpc_isbrc = TRUE;
    } else if (name2ip(prpc->rpc_hostIP, host) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpcparsecargs ==> SYSERR, invalid host: %s\n", host);
#endif
	return(SYSERR);
    }
    return(OK);
}


/*----------------------------------------------------------------------------
 * rpcparsesargs -- parse a RPC open listening server control string
 *----------------------------------------------------------------------------
 */
static
int rpcparsesargs(prpc, strprog)
    struct rpcblk *prpc;
    char *strprog;
{
    char *pch;

#ifdef DEBUG
    kprintf("rpcparsesargs() called: strprog = \"%s\"\n", strprog);
#endif

    pch = strprog;
    prpc->rpc_program = atoi(pch);

    if ((pch = index(pch, ':')) == NULL) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("rpcparsesargs: No colon\n");
#endif
	return (SYSERR);
    }
    prpc->rpc_minversion = atoi(pch+1);

    /* Check for the (optional) maxversion field */
    if ((pch = index(pch+1, '-')) == NULL) {
	prpc->rpc_version = prpc->rpc_minversion;
    } else {
	prpc->rpc_version = atoi(pch+1);
    }

    /* Just in case ... */
    prpc->rpc_isbrc = FALSE;
    return (OK);
}
