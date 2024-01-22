/* naminit.c - naminit */

#include <conf.h>
#include <kernel.h>
#include <name.h>

#define USE_NFS
/*#define USE_RFS*/

#ifndef	RFILSYS
#define	RFILSYS	SYSERR
#endif
#ifdef	Nnsys
struct	nam	Nam;
#endif

/*------------------------------------------------------------------------
 *  naminit  -  initialize the syntactic namespace pseudo-device
 *------------------------------------------------------------------------
 */
naminit()
{
    Nam.nnames = 0;

    /* Xinu namespace definition */
    mount("",               NAMESPACE, "/bin/");
    mount("/dev/console",   CONSOLE,   NULLSTR);
    mount("/dev/",          SYSERR,    NULLSTR);
    mount("~/",             NAMESPACE, "/usr/");
    mount("h/",             NAMESPACE, "xinu/src/sys/h/");
    mount("kernel/",        NAMESPACE, "xinu/src/sys/");
    
#ifdef USE_NFS
    /* mount NFS file system */
    mount("/", 		NFS,	"gwen:/u6:/lin/xinu8.1/programs/");
    mount("xinu/", 	NFS, 	"gwen:/u6:/lin/xinu8.1/xinu/");
    mount("/dev/null", 	NFS, 	"gwen:/u6:/dev/null/");
#endif
	
#ifdef USE_RFS
    /* mount RFS file system */
    mount("xinu/",	RFILSYS,   "/usr/xinu8.1/xinu/");
    mount("/",		RFILSYS,   "/usr/xinu8.1/programs/");
    mount("/unix/",     RFILSYS,   "/");
    mount("/dev/null",  RFILSYS,   "/dev/null");
#endif
}

