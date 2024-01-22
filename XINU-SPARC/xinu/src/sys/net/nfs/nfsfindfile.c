/* nfsfindfile.c - nfsfindfile */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <nfs.h>
#include <mount.h>

/*#define DEBUG*/
#define PRINTERRORS

/* global routine defn's */
extern char *getmem();
extern char *index();
extern char *rindex();

/* local routines */
LOCAL nfsparse();

/*-------------------------------------------------------------------------
 * nfsfindfile - return the handles for a file and its parent
 *               xname is the xinu nfs name for the file
 *               fmustexist is a flag that controls whether the file
 *                  itself must exist, or just its parent directory
 *-------------------------------------------------------------------------
 */
nfsfindfile(pnfs,xname,fmustexist,followlink)
     struct nfsblk *pnfs;
     char *xname;
     Bool fmustexist;
     Bool followlink;
{
    int		retval;
    char	*host;
    char	*fs;
    int		xnamelen;
    struct mountentry *pme;

#ifdef DEBUG
    kprintf("nfsfindfile(xname:'%s') called\n", xname);
#endif

    /* check for a legal length xname */
    xnamelen = strlen(xname) + 1;
    if (xnamelen > NFS_PATHLEN) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("nfsfindfile ==> SYSERR, xname too long\n");
#endif
	pnfs->nfs_status = NFSERR_PATHLEN;
	return(SYSERR);
    }

#ifdef DEBUG
    kprintf("nfsfindfile: parsing the file name\n");
#endif
    /* parse the NFS xname into its 3 components */
    host  = getmem(xnamelen);
    fs    = getmem(xnamelen);
    if (nfsparse(pnfs,xname,host,fs) != OK) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("nfsfindfile ==> SYSERR, couldn't parse file name '%s'\n",
		xname);
#endif
	freemem(host,xnamelen);
	freemem(fs,xnamelen);
	pnfs->nfs_status = NFSERR_BADNAME;
	return(SYSERR);
    }

#ifdef DEBUG
    kprintf("nfsfindfile: finding mount\n");
#endif
    /* find the fs root in the mount table (or create one) */
    wait(mountTableMutex);
    
    pme = nfsgetmount(host, fs);
    
    if (pme == (struct mountentry *) SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("nfsfindfile: no mount point\n");
#endif
	signal(mountTableMutex);
	freemem(host,xnamelen);
	freemem(fs,xnamelen);
	pnfs->nfs_status = NFSERR_NOMOUNT;
	return(SYSERR);
    }
    pnfs->nfs_pme = pme;

#ifdef DEBUG
    kprintf("nfsfindfile: freeing\n");
#endif
    /* we're done with these */
    freemem(host,xnamelen);
    freemem(fs,xnamelen);

    signal(mountTableMutex);

#ifdef DEBUG
    kprintf("nfsfindfile: walking path\n");
#endif
    /* find the handle for the file */
    retval = nfswalkpath(pnfs, pnfs->nfs_pme->mnt_handle,
			 NFS_MAX_SYMLINKS, followlink);

    /* there might have been a stale file handle in the cache, so	*/
    /* check before returning it					*/
    if (pnfs->nfs_status == NFSERR_STALE) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("nfsfindfile: STALE path for '%s', try again\n",
		pnfs->nfs_fpath);
#endif
	nfshcpurge(pnfs->nfs_pme);
	retval = nfswalkpath(pnfs, pnfs->nfs_pme->mnt_handle,
			     NFS_MAX_SYMLINKS, followlink);
    }

    if (retval == OK)
	return(OK);

    if ((pnfs->nfs_phandle_valid) && (!fmustexist))
	return(OK);

    return(SYSERR);
}


/*-------------------------------------------------------------------------
 * nfsparse - parse the pathname of a Xinu nfs file into its 3 components
 *-------------------------------------------------------------------------
 */
LOCAL nfsparse(pnfs,xname,host,fs)
     struct nfsblk *pnfs;
     char *xname;
     char *host;
     char *fs;
{
    int retval;
    char *pch;

    retval = sscanf(xname,"%[^:]:%[^:]:%s", host, fs, pnfs->nfs_fpath);

    if (retval != 3) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("nfsparse('%s'...) ==> SYSERR, retval == %d\n",
		xname, retval);
#endif
	pnfs->nfs_status = NFSERR_BADNAME;
	return(SYSERR);
    }

	
#ifdef DEBUG
    kprintf("nfsparse('%s'...) yields:\n", xname);
    kprintf("\thost: %s\n\tfs: %s\n\tname: %s\n", host,fs,pnfs->nfs_fpath);
#endif
    if (strlen(host) > NFS_HOSTNAMELEN) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("nfsparse: host name too long: %s\n", host);
#endif
	pnfs->nfs_status = NFSERR_BADNAME;
	return(SYSERR);
    }
    if (strlen(fs) > NFS_FSLEN) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("nfsparse: fs name too long: %s\n", fs);
#endif
	pnfs->nfs_status = NFSERR_BADNAME;
	return(SYSERR);
    }

    /* set the pointer to the file's name */
    pch = rindex(pnfs->nfs_fpath,'/');
    if (pch)
	pnfs->nfs_fname = pch + 1;
    else
	pnfs->nfs_fname = pnfs->nfs_fpath;
		
    return(OK);
}

