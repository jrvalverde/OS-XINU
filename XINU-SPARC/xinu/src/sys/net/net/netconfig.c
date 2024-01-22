/* netconfig.c - netconfig */

#include <netconf.default.h>
#include <kernel.h>
#include <network.h>
#include <mark.h>
#include <openprom.h>

static kgets();
static getone();
static netconf_interactive();

struct netconf netconf;
MARKER netconf_mark;

/*int netconf_method = NETCONF_DEFAULT;*/
int netconf_method = NETCONF_INTERACTIVE;
/*int netconf_method = NETCONF_BOOTP;*/

#define QUERY_AT_STARTUP 


/*------------------------------------------------------------------------
 *  netconfig - set network configuration parms
 *------------------------------------------------------------------------
 */
netconfig()
{
    
#ifdef QUERY_AT_STARTUP
    {
	char resp[100];
	Bool done;
	
	kprintf("\nnetwork configuration method options:\n");
	kprintf("  1: interactive\n");
	kprintf("  2: bootp\n");
	kprintf("  3: default\n");
	kprintf("please select one: ");
	
	while (!done) {
	    *resp = NULLCH;
	    kgets(resp);
	    done = TRUE;
	    switch (*resp) {
	      case '1':
		netconf_method = NETCONF_INTERACTIVE;
		break;
	      case '2':
		netconf_method = NETCONF_BOOTP;
		break;
	      case '3':
		netconf_method = NETCONF_DEFAULT;
		break;
	      default:
		done = FALSE;
		kprintf("Please enter 1, 2, or 3: ");
		break;
	    }
	}
    }
#endif
    if (SYSERR == nconfig(netconf_method))
	nconfig(NETCONF_INTERACTIVE);
    
#ifdef DEBUG
    kprintf("netconf.gateway:  '%s'\n", netconf.gateway);
    kprintf("netconf.nserver:  '%s'\n", netconf.nserver);
    kprintf("netconf.pgserver: '%s'\n", netconf.pgserver);
    kprintf("netconf.rserver:  '%s'\n", netconf.rserver);
    kprintf("netconf.tserver:  '%s'\n", netconf.tserver);
    kprintf("netconf.myIpaddr: '%s'\n", netconf.myIPaddr);
#endif
    
}


/*-------------------------------------------------------------------------
 * nconfig - 
 *-------------------------------------------------------------------------
 */
int nconfig(method)
     int method;
{
    int         retval;
    
    if (unmarked(netconf_mark)) {
#ifdef DEBUG_VERBOSE
	kprintf("netconf: marking netconf as invalid.\n");
#endif
	mark(netconf_mark);
	netconf.init = FALSE;
	strcpy(netconf.gateway, GATEWAY_DEFAULT);
	strcpy(netconf.nserver, NSERVER_DEFAULT);
	strcpy(netconf.pgserver, PGSERVER_DEFAULT);
	strcpy(netconf.rserver, RSERVER_DEFAULT);
	strcpy(netconf.tserver, TSERVER_DEFAULT);
	strcpy(netconf.myIPaddr, MYIPADDR_DEFAULT);
    }
    
    if (FALSE == netconf.init) {
	switch (method) {
	  case NETCONF_INTERACTIVE:
#ifdef DEBUG_VERBOSE
	    kprintf("Netconf: method NETCONF_INTERACTIVE.\n");
#endif
	    /* read all info from the keyboard */
	    retval = netconf_interactive(&netconf);
#ifdef DEBUG_VERBOSE
	    if (SYSERR == retval)
		kprintf("user config failed.\n");
#endif
	    break;
	  case NETCONF_DEFAULT:
#ifdef DEBUG_VERBOSE
	    kprintf("Netconf: method NETCONF_DEFAULT.\n");
#endif
	    netconf.init = TRUE;
	    /* already have the default values */
	    retval = OK;
	    break;
	  case NETCONF_BOOTP:
#ifdef DEBUG_VERBOSE
	    kprintf("Netconf: method NETCONF_BOOTP.\n");
#endif
	    retval = bootp();
	    if (SYSERR == retval) {
#ifdef DEBUG
		kprintf("********************************\n");
		kprintf("       BOOTP failed\n");
		kprintf("********************************\n");
#endif
	    } else {
#ifdef DEBUG
		kprintf("********************************\n");
		kprintf("       BOOTP succeeded\n");
		kprintf("********************************\n");
#endif
	    }
	    break;
	}
	/* check for required fields */
	if ('\0' == netconf.gateway[0]) {
	    kprintf("Gateway not set.  Aborting.\n");
	    retval = SYSERR;
	}
	if ('\0' == netconf.nserver[0]) {
	    kprintf("Nameserver not set.  Aborting.\n");
	    retval = SYSERR;
	}
	if ('\0' == netconf.pgserver[0]) {
	    kprintf("Pageserver not set.  Aborting.\n");
	    retval = SYSERR;
	}
	if ('\0' == netconf.rserver[0]) {
	    kprintf("RFS server not set.  Aborting.\n");
	    retval = SYSERR;
	}
	if ('\0' == netconf.tserver[0]) {
	    kprintf("Timeserver not set.  Aborting.\n");
	    retval = SYSERR;
	}
	if ('\0' == netconf.myIPaddr[0]) {
	    kprintf("my IP addr.  Aborting.\n");
	    retval = SYSERR;
	}
	if (SYSERR == retval) {	 /* didn't get what we wanted */
	    netconf.init = FALSE;
#ifdef DEBUG
	    kprintf("netconf: configuration failed.\n");
#endif
	}
    }
    else
	retval = OK;		 /* already inited - no errors. */
    
    return retval;
}

#define prputchar(x) (*romp->v_putchar)((unsigned char) x)

/*-------------------------------------------------------------------------
 * kgets - 
 *-------------------------------------------------------------------------
 */
static kgets(inbuffer)
char *inbuffer;
{
    int c;
    
    STATWORD ps;
    
    disable(ps);
    while (1) {
	while ((c = (*romp->v_mayget)()) < 0);
	prputchar(c);		/* echo */
	if ((char) c == 0xd) {
	    prputchar(0xa);	/* new line */
	    break;
	}
	*inbuffer++ = (char) c;
    }
    *inbuffer = '\0';
    restore(ps);
}


/*-------------------------------------------------------------------------
 * isIP - 
 *-------------------------------------------------------------------------
 */
static isIP(addr)
char *addr;
{
    int junk;

    return (sscanf(addr, "%d.%d.%d.%d", &junk,&junk,&junk,&junk) == 4);
}


/*-------------------------------------------------------------------------
 * isport - 
 *-------------------------------------------------------------------------
 */
static isport(port)
char *port;
{
    int junk;
    
    if (sscanf(port, "%d", &junk) != 1)
	return(FALSE);
    return ((junk > 0) && (junk < 0xffff));
}


/*-------------------------------------------------------------------------
 * realIP - 
 *-------------------------------------------------------------------------
 */
static char *readIP(def)
char *def;
{
    static char inbuffer[256];
    
    while (TRUE) {
	inbuffer[0] = '\0';
	kprintf("   IP address [%s]: ", def);
	kgets(inbuffer);
	if ('\0' != inbuffer[0]) {
	    if (isIP(inbuffer))
		return(inbuffer);
	    kprintf("	'%s' is not a valid IP address\n",
		    inbuffer);
	} else {
	    /* use def... */
	    if (isIP(def))
		return(def);
	    kprintf("	default (%s) is not a valid IP address\n", def);
	}
    }
}


/*-------------------------------------------------------------------------
 * readport - 
 *-------------------------------------------------------------------------
 */
static char *readport(def)
char *def;
{
    static char inbuffer[256];
    
    while (TRUE) {
	inbuffer[0] = '\0';
	kprintf("   port [%s]: ", def);
	kgets(inbuffer);
	if ('\0' != inbuffer[0]) {
	    if (isport(inbuffer))
		return(inbuffer);
	    kprintf("	'%s' is not a valid port\n",
		    inbuffer);
	} else {
	    /* use default... */
	    if (isport(def))
		return(def);
	    kprintf("	default (%s) is not a valid port\n",
		    def);
	}
    }
}

static
    getIP(prompt, dest)
char *prompt;
char *dest;
{
    char buf[256];
    char *defaultIP;
    char *defaultport;
    char *IP;
    char *port;
    
    /* find the default IP addr */
    strcpy(buf,dest);
    defaultIP = buf;
    defaultport = (char *) index(buf,':');
    if (defaultport != NULLPTR) {
	*defaultport = '\00';
	++defaultport;
    }
    
    kprintf("%s:\n", prompt);
    
    IP = readIP(defaultIP);
    if (defaultport) {
	port = readport(defaultport);
	sprintf(dest,"%s:%s", IP, port);
    } else {
	sprintf(dest,"%s", IP);
    }
}

/*-------------------------------------------------------------------------
 * netconf_interactive - 
 *-------------------------------------------------------------------------
 */
static netconf_interactive(nptr)
struct netconf *nptr;
{
    /* the problem with this is that kgets needs to be kkgets... */
    IPaddr	myIP;
    int 	finished;
    int		answered;
    char	answer[256];

    /* try RARP to get my IP address */
    getaddr(myIP);
    ip2dot(netconf.myIPaddr, myIP);
    
    /* if not set, use the default */
    if (!netconf.gateway || !*netconf.gateway)
	sprintf(netconf.gateway, "%s", GATEWAY_DEFAULT);
    if (!netconf.myIPaddr || !*netconf.myIPaddr)
	sprintf(netconf.myIPaddr, "%s", MYIPADDR_DEFAULT);
    if (!netconf.nserver || !*netconf.nserver)
	sprintf(netconf.nserver, "%s", NSERVER_DEFAULT);
    if (!netconf.pgserver || !*netconf.pgserver)
	sprintf(netconf.pgserver, "%s", PGSERVER_DEFAULT);
    if (!netconf.rserver || !*netconf.rserver)
	sprintf(netconf.rserver, "%s", RSERVER_DEFAULT);
    if (!netconf.tserver || !*netconf.tserver)
	sprintf(netconf.tserver, "%s", TSERVER_DEFAULT);
    
    finished = FALSE;
    while (!finished) {
	kprintf("Please enter the following network addresses:\n");
	kprintf("  give IP addresses in dotted decimal notation:");
	kprintf("  byte.byte.byte.byte\n");
	kprintf("  give ports as decimals in the range 0 - 32767\n");
	getIP("My IP address",      nptr->myIPaddr);
	getIP("Gateway address",    nptr->gateway);
	getIP("Pageserver address", nptr->pgserver);
 	getIP("RFS/NFS server address", nptr->rserver);
	getIP("Timeserver address", nptr->tserver);
	getIP("Nameserver address", nptr->nserver);
	
	answered = 0;
	kprintf("\nYou have entered:\n");
	kprintf("netconf.myIPaddr: '%s'\n", netconf.myIPaddr);
	kprintf("netconf.gateway:  '%s'\n", netconf.gateway);
	kprintf("netconf.nserver:  '%s'\n", netconf.nserver);
	kprintf("netconf.pgserver: '%s'\n", netconf.pgserver);
	kprintf("netconf.rserver:  '%s'\n", netconf.rserver);
	kprintf("netconf.tserver:  '%s'\n", netconf.tserver);
	while (!answered) {
	    kprintf("Is this information correct [y/n]? ");
	    if (SYSERR == kgets(answer))
		return SYSERR;
	    if ('y' == answer[0] || 'Y' == answer[0]) {
		finished = 1;
		answered = 1;
	    }
	    else if ('n' == answer[0] || 'N' == answer[0])
		answered = 1;
	}
    }
    return OK;
}
