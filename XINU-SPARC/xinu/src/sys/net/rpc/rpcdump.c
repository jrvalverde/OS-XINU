/* rpcdump.c  dump a <disassembled> rpc mesg in readable form */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <rpcprogs.h>
#include <xdr.h>
#include <portmapper.h>
#include <nfs.h>


char *rpc_msgtype_names[]	= RPC_MSGTYPE_NAMES;
char *rpc_replystat_names[]	= RPC_REPLYSTAT_NAMES;
char *rpc_acceptstat_names[]	= RPC_ACCEPTSTAT_NAMES;
char *rpc_rejectstat_names[]	= RPC_REJECTSTAT_NAMES;
char *rpc_authstat_names[]	= RPC_AUTHSTAT_NAMES;
char *rpc_authflavor_names[]	= RPC_AUTHFLAVOR_NAMES;

/* global routine defns */
char	*rpcnameof();

/* local routine definitions */
static char *rpcdumpcall();
static char *rpcdumpreply();
static char *rpcdumpdenied();
static char *rpcdumpauth();
static char *rpcdumpauthunix();
static char *rpcdumpaccepted();
static char *rpcprogname();
static char *rpcprocname();


/*#define DUMP_BYTES*/
#define DUMP_DATA



/*
 * ====================================================================
 * rpcdump - describe the contents of an RPC packet
 * ====================================================================
 */
rpcdump(pch,len)
     char *pch;
{
	unsigned xid,u;
	char	*pchin;

	pchin = pch;

#ifdef DUMP_BYTES
	kprintf("  Packet Data:\n");
	rpcdumpbytes(pch,len);
#endif

	pch = xdrgetunsigned(pch,&xid);
	kprintf("  XID:     0x%x\n", xid);

	pch = xdrgetunsigned(pch,&u);
	switch (u) {
	      case RPC_MSGTYPE_CALL:
		kprintf("  Mtype:   0x%x (%s)\n", u,
			rpcnameof(rpc_msgtype_names,u));
		pch = rpcdumpcall(pch);
#ifdef DUMP_DATA
		if (pch != (char *) SYSERR) {
			kprintf("  Call Data:\n");
				rpcdumpbytes(pch,len - (pch - pchin));
			}
#endif
		return(OK);
	      case RPC_MSGTYPE_REPLY:
		kprintf("  Mtype:   0x%x (%s)\n", u,
			rpcnameof(rpc_msgtype_names,u));
		pch = rpcdumpreply(pch);
#ifdef DUMP_DATA
		if (pch != (char *) SYSERR) {
			kprintf("  Reply Data:\n");
			rpcdumpbytes(pch,len - (pch - pchin));
		}
#endif
		return(OK);
	      default:
		kprintf("  rpcdump: unknown type = 0x%08x\n", u);
		return(SYSERR);
	}
}



/*
 * ====================================================================
 * rpcprogname - give the name for the given program (if I know it)
 * ====================================================================
 */
static char *
rpcprogname(prog)
     unsigned prog;
{
	switch(prog) {
	      case RPCPROG_PMAP:	return("PMAP");
	      case RPCPROG_RSTAT:	return("RSTAT");
	      case RPCPROG_RUSERS:	return("RUSERS");
	      case RPCPROG_NFS:		return("NFS");
	      case RPCPROG_YPSERV:	return("YPSERV");
	      case RPCPROG_MOUNT:	return("MOUNT");
	      case RPCPROG_YPBIND:	return("YPBIND");
	      case RPCPROG_WALL:	return("WALL");
	      case RPCPROG_YPPASSWD:	return("YPPASSWD");
	      case RPCPROG_ETHERSTAT:	return("ETHERSTAT");
	      case RPCPROG_RQUOTA:	return("RQUOTA");
	      case RPCPROG_SPRAY:	return("SPRAY");
	      case RPCPROG_P3270MAP:	return("P3270MAP");
	      case RPCPROG_RJEMAP:	return("RJEMAP");
	      case RPCPROG_SELECTION:	return("SELECTION");
	      case RPCPROG_DATABASE:	return("DATABASE");
	      case RPCPROG_REX:		return("REX");
	      case RPCPROG_ALIS:	return("ALIS");
	      case RPCPROG_SCHED:	return("SCHED");
	      case RPCPROG_LLOCKMGR:	return("LLOCKMGR");
	      case RPCPROG_NLOCKMGR:	return("NLOCKMGR");
	      case RPCPROG_X25INR:	return("X25INR");
	      case RPCPROG_STATMON:	return("STATMON");
	      case RPCPROG_STATUS:	return("STATUS");
	      case RPCPROG_BOOTPARAM:	return("BOOTPARAM");
	      case RPCPROG_YPUPDATE:	return("YPUPDATE");
	      case RPCPROG_KEYSERV:	return("KEYSERV");
	      case RPCPROG_TFS:		return("TFS");
	      case RPCPROG_NSE:		return("NSE");
	      case RPCPROG_NSEMNT:	return("NSEMNT");
	      default:			return("<unknown>");
	}
}


/*
 * ====================================================================
 * rpcprocname - give the name for the given procedure (if I know it)
 * ====================================================================
 */
static char *
rpcprocname(prog,proc)
     unsigned prog;
     unsigned proc;
{
	if (proc == 0)
	    return("NULL");
	
	switch(prog) {
	      case RPCPROG_PMAP: switch(proc) {
		    case PMAP_PROC_SET:		return("PMAP_PROC_SET");
		    case PMAP_PROC_UNSET:	return("PMAP_PROC_UNSET");
		    case PMAP_PROC_GETPORT:	return("PMAP_PROC_GETPORT");
		    case PMAP_PROC_DUMP:	return("PMAP_PROC_DUMP");
		    case PMAP_PROC_CALLIT:	return("PMAP_PROC_CALLIT");
		    default:			return("PMAP_UNKNOWN");
	      }
	      case RPCPROG_NFS:	 switch(proc) {
		    case NFSPROC_GETATTR:	return("NFSPROC_GETATTR");
		    case NFSPROC_SETATTR:	return("NFSPROC_SETATTR");
		    case NFSPROC_ROOT:		return("NFSPROC_ROOT");
		    case NFSPROC_LOOKUP:	return("NFSPROC_LOOKUP");
		    case NFSPROC_READLINK:	return("NFSPROC_READLINK");
		    case NFSPROC_READ:		return("NFSPROC_READ");
		    case NFSPROC_WRITECACHE:	return("NFSPROC_WRITECACHE");
		    case NFSPROC_WRITE:		return("NFSPROC_WRITE");
		    case NFSPROC_CREATE:	return("NFSPROC_CREATE");
		    case NFSPROC_REMOVE:	return("NFSPROC_REMOVE");
		    case NFSPROC_RENAME:	return("NFSPROC_RENAME");
		    case NFSPROC_LINK:		return("NFSPROC_LINK");
		    case NFSPROC_SYMLINK:	return("NFSPROC_SYMLINK");
		    case NFSPROC_MKDIR:		return("NFSPROC_MKDIR");
		    case NFSPROC_RMDIR:		return("NFSPROC_RMDIR");
		    case NFSPROC_READDIR:	return("NFSPROC_READDIR");
		    case NFSPROC_STATFS:	return("NFSPROC_STATFS");
		    default:			return("NFSPROC_UNKNOWN");
	      }
	      default:			return("<unknown>");
	}
}




/*
 * ====================================================================
 * rpcdumpcall - dump an RPC packet, assuming type CALL
 * ====================================================================
 */
static char *
rpcdumpcall(pch)
     char *pch;
{
	unsigned u,u2;

	pch = xdrgetunsigned(pch,&u);
	kprintf("  RPC version:       %08d\n", u);

	pch = xdrgetunsigned(pch,&u2);
	kprintf("  Program number:    %08d (%s)\n",
		u2, rpcprogname(u2));

	pch = xdrgetunsigned(pch,&u);
	kprintf("  Program version:   %08d\n", u);

	pch = xdrgetunsigned(pch,&u);
	kprintf("  Procedure number:  %08d (%s)\n",
		u, rpcprocname(u2,u));

	kprintf("  Cred auth:\n");
	pch = rpcdumpauth(pch);

	kprintf("  Verif auth:\n");
	pch = rpcdumpauth(pch);

	return(pch);
}


static char *
rpcdumpreply(pch)
     char *pch;
{
	unsigned reply;

	pch = xdrgetunsigned(pch,&reply);
	kprintf("  Reply status: %x (%s)\n", reply,
		rpcnameof(rpc_replystat_names,reply));

	switch (reply) {
		case RPC_REPLYSTAT_ACCEPTED:
			return rpcdumpaccepted(pch);
		case RPC_REPLYSTAT_DENIED:
			return rpcdumpdenied(pch);
		default:
			kprintf("  Unknown reply status: 0x%x\n",
				reply);
			return((char *) SYSERR);
	}
}



static char *
rpcdumpdenied(pch)
     char *pch;
{
	unsigned reply, u1, u2;

	pch = xdrgetunsigned(pch,&reply);
	kprintf("  Rejected message status: 0x%x (%s)\n",
		reply,
		rpcnameof(rpc_rejectstat_names,reply));

	switch (reply) {
	      case RPC_REJECTSTAT_RPCMISMATCH:
		pch = xdrgetunsigned(pch,&u1);
		pch = xdrgetunsigned(pch,&u2);
		kprintf("  dumprpcrejected: low = 0x%x  high = 0x%x\n", u1,u2);
	      case RPC_REJECTSTAT_AUTHERROR:
		pch = xdrgetunsigned(pch,&u1);
		switch (u1) {
		      case RPC_AUTHSTAT_BADCRED:
		      case RPC_AUTHSTAT_REJECTEDCRED:
		      case RPC_AUTHSTAT_BADVERF:
		      case RPC_AUTHSTAT_REJECTEDVERF:
		      case RPC_AUTHSTAT_TOOWEAK:
			kprintf("  Authentication error 0x%x (%s)\n",
				u1,
				rpcnameof(rpc_authstat_names,u1));
			break;
		      default:
			kprintf("  dumprpcrejected: unknown AUTH_ERROR: 0x%8x\n",
				u1);
			return((char *) SYSERR);
		}
	      default:
		kprintf("  dumprpcrejected: unknown reject_stat: 0x%8x\n",
			reply);
	}
	return(pch);
}


static char *
rpcdumpauth(pch)
     char *pch;
{
	unsigned u;
	unsigned len;
	
	pch = xdrgetunsigned(pch,&u);
	kprintf("    Auth flavor: 0x%x (%s)  ", u,
		rpcnameof(rpc_authflavor_names,u));

	pch = xdrgetunsigned(pch,&len);
	kprintf(" Auth len: %d\n", len);

	switch (u) {
		case RPC_AUTHFLAVOR_UNIX:
			pch = rpcdumpauthunix(pch);
			break;
		default:
			pch += len;
			break;
	}
	return pch;
}


static char *
rpcdumpauthunix(pch)
     char *pch;
{
	unsigned stamp;
	char mname[500];
	unsigned uid, gid;
	unsigned count;
	int i;

	pch = xdrgetunsigned(pch,&stamp);
	kprintf("      Stamp is 0x%08x\n", stamp);

	pch = xdrgetstring(pch, mname, sizeof(mname));
	kprintf("      machine name: '%s'\n", mname);

	pch = xdrgetunsigned(pch,&uid);
	pch = xdrgetunsigned(pch,&gid);
	pch = xdrgetunsigned(pch,&count);
	kprintf("      uid: %d   gid: %d  group count: %d\n",
		uid, gid, count);
	for (i=0; i < count; ++i) {
		pch = xdrgetunsigned(pch,&gid);
		kprintf("           group[%d] gid: %d\n", i, gid);
	}

	return pch;
}


static char *
rpcdumpaccepted(pch)
     char *pch;
{
	unsigned reply, u1, u2;
	
	kprintf("  Verification authentication:\n");
	pch = rpcdumpauth(pch);

	pch = xdrgetunsigned(pch,&reply);
	kprintf("  Accepted reply status is 0x%x (%s)\n",
		reply,
		rpcnameof(rpc_acceptstat_names,reply));

	switch (reply) {
		case RPC_ACCEPTSTAT_SUCCESS:
			kprintf("  message was accepted.\n");
			break;
		case RPC_ACCEPTSTAT_PROGMISMATCH:
			pch = xdrgetunsigned(pch,&u1);
			pch = xdrgetunsigned(pch,&u2);
			kprintf("  dumprpcaccepted: low = %d. high = %d.\n",
				u1, u2);
			break;
		case RPC_ACCEPTSTAT_PROGUNAVAIL:
			kprintf("  dumprpcaccepted: program unavilable.\n");
			break;
		case RPC_ACCEPTSTAT_PROCUNAVAIL:
			kprintf("  dumprpcaccepted: procedure unavilable.\n");
			break;
		case RPC_ACCEPTSTAT_GARBAGEARGS:
			kprintf("  dumprpcaccepted: garbage arguments.\n");
			break;
		default:
			kprintf("  dumprpcaccepted: unknown status 0x%8x.\n",
				reply);
	}
	return(pch);
}


#define PRINTABLE(ch) (((ch) >= ' ') && ((ch) <= '~'))?(ch):'.'

rpcdumpbytes(px, nbytes)
     unsigned *px;
     int nbytes;
{
	int i;
	char *pch;
	
	kprintf("      HEX       DECIMAL       netDECIMAL    ASCII\n");
	kprintf("      ========  ============  ============  =======\n");
	for (i=0; i < nbytes; i+=4) {
		pch = (char *) px;
		kprintf("      %08x  %12d  %12d  %c %c %c %c\n",
			*px, *px, net2hl(*px),
			PRINTABLE(pch[0]),
			PRINTABLE(pch[1]),
			PRINTABLE(pch[2]),
			PRINTABLE(pch[3]));
		++px;
	}
}


char *
rpcnameof(strtab,func)
     char	*strtab[];
     unsigned	func;
{
	int i;

	for (i=0; strtab[i] != NULL; ++i) {
		if (i == func)
		    return(strtab[i]);
	}
	return("ILLEGAL VALUE");
}
