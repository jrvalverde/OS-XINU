/* nfsalloc.c - nfsalloc() nfsdealloc() */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <nfs.h>
#include <io.h>

/*#define DEBUG*/
#define PRINTERRORS

/*
 * ====================================================================
 * nfsalloc - return the index of a free entry in the nfs block array,
 *            returns SYSERR if none found.
 * ====================================================================
 */
nfsalloc()
{
    int i;
    struct nfsblk *pnfs;

    wait (nfsTableMutex);
    for (i = 0; i < Nnfs; ++i) {
	pnfs = &nfstab[i];
	if (pnfs->nfs_state == NFS_STAT_FREE) {
	    pnfs->nfs_state = NFS_STAT_USED;
	    pnfsopen(pnfs);
	    signal (nfsTableMutex);
#if defined(DEBUG)
	    kprintf("nfsalloc: returning entry %d\n", i);
#endif
	    return(i);
	}
    }
    signal (nfsTableMutex);
#if defined(DEBUG) || defined(PRINTERRORS)
    kprintf("nfsAlloc: no entries left\n");
#endif
    return(SYSERR);
}




/*
 * ====================================================================
 * nfsdealloc - return an NFS block to the available pool
 * ====================================================================
 */
nfsdealloc(pnfs)
	struct nfsblk *pnfs;
{
#ifdef DEBUG
    kprintf("nfsdealloc: freeing block %d\n", pnfs-nfstab);
#endif
    pnfs->nfs_state = NFS_STAT_FREE;
    pnfsclose(pnfs);
}



/*
 * ====================================================================
 * pnfsopen - initialize all the fields in an NFS block, need not be
 *            from the nfsblk array....
 * ====================================================================
 */
pnfsopen(pnfs)
     	struct nfsblk *pnfs;
{
    pnfs->nfs_pme = (struct mountentry *) NULL;
    pnfs->nfs_status = NFS_OK;
    pnfs->nfs_mode = 0;
    pnfs->nfs_fpos = 0;
    pnfs->nfs_mgeneration = SYSERR;
    pnfs->nfs_fhandle_valid = FALSE;
    pnfs->nfs_phandle_valid = FALSE;
    pnfs->nfs_rpcdev = -1;
    pnfs->nfs_mutex = screate(1);
}



/*
 * ====================================================================
 * pnfsclose - clean up any state associated with an NFS block
 * ====================================================================
 */
pnfsclose(pnfs)
     	struct nfsblk *pnfs;
{
    /* close the RPC device that we were using for this */
    if (!isbaddev(pnfs->nfs_rpcdev))
	close(pnfs->nfs_rpcdev);

    sdelete(pnfs->nfs_mutex);
    pnfs->nfs_mutex = SYSERR;
}
