/* nfs.c -- nfsStatString, nfsDescribeStat, nfsOpenConnection, openrpcdev  */
/*          NfsPrintHandle */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <rpcprogs.h>
#include <nfs.h>
#include <mount.h>

/*#define DEBUG*/
#define PRINTERRORS

/*-------------------------------------------------------------------------
 * nfsStatString - return a string corresponding to an NFS error code
 *-------------------------------------------------------------------------
 */
char *nfsStatString(stat)
	unsigned stat;
{
    char *perr;
    static char msg[80];

    switch (stat) {
      case NFS_OK:	 	perr = "NFS_OK"; break;
      case NFSERR_PERM:	 	perr = "NFSERR_PERM"; break;
      case NFSERR_NOENT:	perr = "NFSERR_NOENT"; break;
      case NFSERR_IO:	 	perr = "NFSERR_IO"; break;
      case NFSERR_NXIO:	 	perr = "NFSERR_NXIO"; break;
      case NFSERR_ACCES:	perr = "NFSERR_ACCES"; break;
      case NFSERR_EXIST:	perr = "NFSERR_EXIST"; break;
      case NFSERR_NODEV:	perr = "NFSERR_NODEV"; break;
      case NFSERR_NOTDIR:	perr = "NFSERR_NOTDIR"; break;
      case NFSERR_ISDIR:	perr = "NFSERR_ISDIR"; break;
      case NFSERR_FBIG:	 	perr = "NFSERR_FBIG"; break;
      case NFSERR_NOSPC:	perr = "NFSERR_NOSPC"; break;
      case NFSERR_ROFS:	 	perr = "NFSERR_ROFS"; break;
      case NFSERR_NAMETOOLONG:	perr = "NFSERR_NAMETOOLONG"; break;
      case NFSERR_NOTEMPTY:	perr = "NFSERR_NOTEMPTY"; break;
      case NFSERR_DQUOT:	perr = "NFSERR_DQUOT"; break;
      case NFSERR_STALE:	perr = "NFSERR_STALE"; break;
      case NFSERR_WFLUSH:	perr = "NFSERR_WFLUSH"; break;
      case NFSERR_ABSSYMLINK:	perr = "NFSERR_ABSSYMLINK"; break;
      case NFSERR_RPCERR:	perr = "NFSERR_RPCERR"; break;
      case NFSERR_PATHLEN:	perr = "NFSERR_PATHLEN"; break;
      case NFSERR_BADNAME:	perr = "NFSERR_BADNAME"; break;
      case NFSERR_NOMOUNT:	perr = "NFSERR_NOMOUNT"; break;
      case NFSERR_RPCTIMEOUT:	perr = "NFSERR_RPCTIMEOUT"; break;
      default:	 		perr = "UNDEFINED ERROR NUMBER"; break;
    }
	
    sprintf(msg,"(%d,%s)", stat, perr);
    return(msg);
}


/*-------------------------------------------------------------------------
 * nfsDescribeStat - print the NFS status string
 *-------------------------------------------------------------------------
 */
nfsDescribeStat(stat)
	unsigned stat;
{
    kprintf("%s", nfsStatString(stat));
}


/*-------------------------------------------------------------------------
 * nfsOpenConnection -  make sure the there is an open RPC device for
 *                     an nfs file
 *-------------------------------------------------------------------------
 */
nfsOpenConnection(pnfs)
     struct nfsblk *pnfs;
{
    if (pnfs->nfs_rpcdev == -1) {
	pnfs->nfs_rpcdev = openrpcdev(pnfs->nfs_pme);
	if (pnfs->nfs_rpcdev == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("nfsOpenConnection: open failed\n");
#endif
	    return(SYSERR);
	}
    }
    return(OK);
}


/*-------------------------------------------------------------------------
 * openrpcdev - attach an RPC device to a mount table entry for NFS use
 *-------------------------------------------------------------------------
 */
openrpcdev(pme)
     struct mountentry *pme;
{
    char *host;
    int dev;
    char pmapstr[256];

    host = pme->mnt_host;

    /* open an RPC device for searching, etc on the fs */
    sprintf(pmapstr,"%s:%d:%d:%d", host,
	    RPCPROG_NFS, NFS_VERSION, NFSPROC_NULL);
    if ((dev = open(RPC,pmapstr,RPCT_CLIENT)) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("openrpcdev: couldn't contact NFS server\n");
#endif
	return(SYSERR);
    }

    /* set desired reliability constraints for this device		*/
    control(dev,RPC_SETRTO, pme->mnt_timeout); /* 1 sec */
    control(dev,RPC_SETMAXTRANS,pme->mnt_attempts);

    return(dev);
}


/*-------------------------------------------------------------------------
 * NfsPrintHandle - print out an NFS file handle
 *-------------------------------------------------------------------------
 */
NfsPrintHandle(handle)
     unsigned char *handle;
{
    int i;

    for (i=0; i < NFS_FHSIZE; ++i) {
	kprintf("%02x", (unsigned) (*handle));
	++handle;
    }
    kprintf("\n");
}

/*-------------------------------------------------------------------------
 * nfsstatfile -  return the stat structure for a file
 *-------------------------------------------------------------------------
 */
nfsstatfile(path, pattr, followlink)
     char *path;
     struct fattr *pattr;
     Bool followlink;
{
    struct nfsblk *pnfs;
    char xname[128];
    int retval;

#ifdef DEBUG
    kprintf("nfsstatfile(%s,pattr,%s) called\n",
	    path, pattr, followlink?"TRUE":"FALSE");
#endif
	
    /* initialize the nfsblk struct */
    pnfs = (struct nfsblk *) getmem(sizeof(struct nfsblk));
    pnfsopen(pnfs);

    /* run the name through the namespace */
    nammap(path, xname);

    /* look up the file */
    retval = nfsfindfile(pnfs, xname, NFS_MUST_EXIST, followlink);
    
    if (retval == SYSERR) {
#if defined(DEBUG)
	kprintf("nfsstatfile(%s) ==> SYSERR, no such file\n", xname);
#endif
	pnfsclose(pnfs);
	freemem(pnfs,sizeof(struct nfsblk));
	return(SYSERR);
    }

    /* copy the attributes out */
    blkcopy(pattr, &pnfs->nfs_attributes, sizeof(struct fattr));

    pnfsclose(pnfs);
    freemem(pnfs, sizeof(struct nfsblk));
#ifdef DEBUG
    kprintf("nfsstatfile: return\n");
#endif
    return(OK);
}


/*-------------------------------------------------------------------------
 * nfsreadlink - 
 *-------------------------------------------------------------------------
 */
nfsreadlink(path,buffer)
     char *path;
     char *buffer;
{
    struct nfsblk *pnfs;
    char xname[128];
    int retval;

#ifdef DEBUG
    kprintf("nfsreadlink(%s) called\n", path);
#endif
	
    /* initialize the nfsblk struct */
    pnfs = (struct nfsblk *) getmem(sizeof(struct nfsblk));
    pnfsopen(pnfs);

    /* run the name through the namespace */
    nammap(path,xname);

    /* look up the file */
    retval = nfsfindfile(pnfs,xname,NFS_MUST_EXIST,FALSE);
    if ((retval == SYSERR) || (pnfs->nfs_ftype != NFLNK)) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("nfsreadlink(%s) ==> SYSERR, %s\n",
		xname,
		(retval == SYSERR)?"no such file":"not a symlink");
#endif
	pnfsclose(pnfs);
	freemem(pnfs,sizeof(struct nfsblk));
	return(SYSERR);
    }

    /* read the symlink */
    retval = nfssymlink(pnfs,pnfs->nfs_fhandle,"",buffer);
#ifdef DEBUG
    kprintf("nfsreadlink returns '%s'\n", buffer);
#endif

    pnfsclose(pnfs);
    freemem(pnfs,sizeof(struct nfsblk));
    return(retval);
}



