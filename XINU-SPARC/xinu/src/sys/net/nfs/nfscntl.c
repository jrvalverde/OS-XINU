/* nfscntl.c -- nfscntl, nfsmcntl */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <nfs.h>
#include <mount.h>

#define PRINTERRORS

/*-------------------------------------------------------------------------
 * nfsmcntl - perform control calls on the NFS master device
 *-------------------------------------------------------------------------
 */
/*ARGSUSED*/
nfsmcntl(devptr, func, addr, addr2)
     struct	devsw	*devptr;
     int	func;
     char	*addr;
     char	*addr2;
{
    int retval;

    switch (func) {
      case FLRENAME:
	retval = nfsrename(addr, addr2);
	break;
      case FLREMOVE:
	retval = nfsremove(addr);
	break;
      case FLACCESS:
	break;	/* not implemented -- ignore */
      case NFS_STATFILE:
	retval = nfsstatfile(addr,(struct fattr *)addr2, TRUE);
	break;
      case NFS_LSTATFILE:
	retval = nfsstatfile(addr,(struct fattr *)addr2, FALSE);
	break;
      case NFS_READLINK:
	retval = nfsreadlink(addr, addr2);
	break;
      case NFS_MKDIR:
	retval = nfsmkdir(addr);
	break;
      case NFS_RMDIR:
	retval = nfsrmdir(addr);
	break;
      default:
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("nfsmcntl: func=%d not implemented\n", func);
#endif
	retval = SYSERR;
    }
    return(retval);
}


/*-------------------------------------------------------------------------
 * nfscntl - perform control calls on an open NFS pseudo device
 *-------------------------------------------------------------------------
 */
/*ARGSUSED*/
nfscntl(devptr, func, addr, addr2)
struct	devsw	*devptr;
int	func;
char	*addr;
char	*addr2;
{
    struct nfsblk *pnfs;
    int retval;

    pnfs = (struct nfsblk *) devptr->dvioblk;

    wait(pnfs->nfs_mutex);

    switch (func) {
      case NFS_GETSTATUS:
	retval = SYSERR;		/* not done */
	break;
      case NFS_GETATTR:
	blkcopy(addr, &pnfs->nfs_attributes, sizeof(struct fattr));
	retval = OK;
	break;
      case NFS_GETRPCSTATUS: {
	  struct rpcblk *prpc;
	  prpc = &rpctab[pnfs->nfs_rpcdev];
	  *((int *) addr) = prpc->rpc_status;
	  retval = OK;
	  break;
      }
      case NFS_READDIR:
	retval = nfsreaddir(pnfs, (struct nfs_readdir_req *)addr);
	break;
      case NFS_TRUNCFILE:
	retval = nfstruncfile(pnfs);
	break;
      default:
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("nfscntl:unknown func %d\n", func);
#endif
	retval = SYSERR;
	break;
    }

    signal(pnfs->nfs_mutex);
    return(retval);
}
