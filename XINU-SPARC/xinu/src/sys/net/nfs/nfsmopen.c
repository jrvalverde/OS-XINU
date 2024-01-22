/* nfsmopen.c - nfsmopen */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <nfs.h>
#include <mount.h>


/*#define DEBUG*/
/*#define DEBUG_SYMLINK*/
/*#define REMIND_NFS*/
/*#define REMIND_NOFIND*/
#define PRINTERRORS


/* local routines */
static nfsDoOpen();


/*
 * ====================================================================
 * nfsmopen - open an NFS file
 *            path is of the form HOST:FS:FILENAME
 * ====================================================================
 */
/*ARGSUSED*/
nfsmopen(devptr, path, mode)
     struct devsw *devptr;
     char *path;
     char *mode;
{
    int ret;
    int	slot;
    struct nfsblk *pnfs;

#ifdef REMIND_NFS
    kprintf("NFS: %s\n", path);
#endif

#ifdef DEBUG
    kprintf("nfsmopen(devptr,%s,%s) called\n", path, mode);
#endif


    /* find a free pseudo device */
    if ((slot = nfsalloc()) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("nfsmopen: nfsalloc failed\n");
#endif
	return(SYSERR);
    }
    pnfs = &nfstab[slot];


    ret = nfsDoOpen(pnfs,path, mode);
    if (ret <= 0) {
	nfsdealloc(pnfs);
#if defined(DEBUG) || defined(REMIND_NOFIND)
	kprintf("nfsmopen(%s,%s) ==> %d\n", path, mode, ret);
#endif
    }

    return(ret);
}



LOCAL nfsDoOpen(pnfs, path, mode)
     struct nfsblk *pnfs;
     char *path;
     char *mode;
{
    int	retval;

    /* check the mode bits */
    if ((pnfs->nfs_mode = ckmode(mode)) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("nfsmopen: bad mode bits\n");
#endif
	return(SYSERR);
    }

    retval= nfsfindfile(pnfs, path, FALSE, TRUE);

#ifdef DEBUG
    kprintf("nfsmopen: retval %d status: %s\n",
	    retval, nfsStatString(pnfs->nfs_status));

    if (pnfs->nfs_phandle_valid)
	kprintf("parent's handle is valid\n");
    
    if (pnfs->nfs_fhandle_valid)
	kprintf("my handle is valid.  name part is %s\n", pnfs->nfs_fname);
#endif


    if (retval == SYSERR) {
	/* check for absolute symbolic link */
	if (pnfs->nfs_status == NFSERR_ABSSYMLINK) {
#if defined(DEBUG) || defined(DEBUG_SYMLINK)
	    kprintf("nfsmopen: SYMLINK: trying to open '%s'\n",
		    pnfs->nfs_fpath);
#endif
	    retval = open(NAMESPACE,pnfs->nfs_fpath,mode);
	    /* cleanup the first one */
	    nfsdealloc(pnfs);
	} else if (pnfs->nfs_status == NFSERR_RPCTIMEOUT) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("nfsmopen: NFS open timed out for %s\n", path);
#endif
	    retval = SYSERR;
	} else {
	    /* misc error */
	    retval = SYSERR;
	}
#if defined(DEBUG)
	if (retval == SYSERR)
	    kprintf("nfsmopen: SYSERR\n");
#endif
	return(retval);
    }

    /* open an RPC connection for use on this file */
    if (nfsOpenConnection(pnfs) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("nfsmopen: couldn't open NFS connection\n");
#endif
	return(SYSERR);
    }

    /* check for non-existent file */
    if (!pnfs->nfs_fhandle_valid) {
	/* no such file */
	if (pnfs->nfs_mode & FLOLD) {
	    /* if OLD was specified, this is an error */
#if defined(DEBUG)
	    kprintf("nfsmopen: OLD && no such file\n");
#endif
	    return(SYSERR);
	}

	/* else we'll create this file if we're writing or	*/
	/* they said "rn" (?? why??)				*/
	if ((pnfs->nfs_mode & FLWRITE) ||
	    ((pnfs->nfs_mode & FLREAD) && (pnfs->nfs_mode & FLNEW))) {
	    retval = nfscreatefile(pnfs);
	    if (retval == OK) {
#if defined(DEBUG)
		kprintf("nfsmopen: created file %s\n", pnfs->nfs_fname);
#endif
	    } else {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("nfsmopen: couldn't create %s: %s\n", pnfs->nfs_fname,
			nfsStatString(pnfs->nfs_status));
#endif
		return(SYSERR);
	    }
	}
    } else {
	/* file exists */
	if (pnfs->nfs_mode & FLWRITE) {
	    retval = nfstruncfile(pnfs);
	    if (retval == OK) {
#if defined(DEBUG)
		kprintf("nfsmopen: truncated file %s\n", pnfs->nfs_fname);
#endif
	    } else {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("nfsmopen: couldn't truncate %s: %s\n",
			pnfs->nfs_fname, nfsStatString(pnfs->nfs_status));
#endif
		return(SYSERR);
	    }
		
	}
    }

    wait(mountTableMutex);
    ++pnfs->nfs_pme->mnt_opencount;
    signal(mountTableMutex);

    return(pnfs->nfs_dnum);
}
