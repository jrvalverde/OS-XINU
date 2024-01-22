/* nfsmount.c -- nfsmount */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <rpcprogs.h>
#include <nfs.h>
#include <mount.h>

/*#define DEBUG*/
#define PRINTERRORS

/* local routines */
static callmount();
static getfreemountentry();


/*-------------------------------------------------------------------------
 * nfsmount - create an entry in the mount table for the given host
 *            and file system, if possible, by contacting the mount
 *            daemon on the host and getting the root handle.
 *-------------------------------------------------------------------------
 */
struct mountentry *nfsmount(host, fs, mountmode)
	char *host;
	char *fs;
	unsigned mountmode;
{
    int retval;
    struct mountentry *pme;
    int mindex;

#ifdef DEBUG
    kprintf("nfsmount: trying to mount %s:%s\n", host, fs);
#endif

    if ((mindex = getfreemountentry()) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("nfsmount: mount table is full\n");
#endif
	return((struct mountentry *) SYSERR);
    }
    pme = &mounttab[mindex];

#ifdef DEBUG
    kprintf("nfsmount: got entry %d\n", mindex);
#endif

    /* set up as much of the mount table as possible */
    if (name2ip(pme->mnt_hostIP, host) != OK) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("mount: ==> SYSERR: unknown host '%s'\n", host);
#endif
	pme->mnt_state = MOUNT_FREE;
	return((struct mountentry *) SYSERR);
    }

    /* fill in the interesting mount table fields */
    strcpy(pme->mnt_host, host);
    strcpy(pme->mnt_fs, fs);
    pme->mnt_timeout = 500;
    pme->mnt_attempts = 10;
    pme->mnt_opencount = 0;
    pme->mnt_mountmode = mountmode;
    pme->mnt_mutex = screate(1);

    /* set up the handle cache */
    nfshccreate(pme);

    /* find the root's handle */
    retval = callmount(host, fs, pme->mnt_handle);
    
    if (retval != OK) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("mount: bad status: %d\n", retval);
#endif
	pme->mnt_state = MOUNT_FREE;
	return((struct mountentry *) SYSERR);
    }

    if ((pme->mnt_rpcdev = openrpcdev(pme)) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("callmount: can't find NFS for %s:%s\n", fs, host);
#endif
	pme->mnt_state = MOUNT_FREE;
	return((struct mountentry *) SYSERR);
    }

    ++pme->mnt_generation;
    pme->mnt_state = MOUNT_VALID;

#ifdef DEBUG
    dumpmountTable();
#endif

    return(pme);
}


/*-------------------------------------------------------------------------
 * callmount - contact a remote mount daemon and return the root's handle 
 *-------------------------------------------------------------------------
 */
LOCAL callmount(host, fs, handle)
     char *host;
     char *fs;
     fhandle handle;
{
    unsigned status;
    char pmapstr[256];
    int dev;
    int hlen;

    sprintf(pmapstr,"%s:%d:%d:%d",
	    host, RPCPROG_MOUNT, RPCMOUNT_VERS, RPCMOUNT_PROC_MNT);

#ifdef DEBUG
    kprintf("Calling program '%s'\n", pmapstr);
#endif

    if ((dev = open(RPC, pmapstr, RPCT_CLIENT)) < 0) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("nfsmount: couldn't open device, return = %d\n", dev);
#endif
	return(SYSERR);
    }

    /* kludge in ROOT access - this is SUPPOSED to be ugly :-)  */
    ((struct rpcblk *) (devtab[dev].dvioblk))->rpc_requid = 0;
    ((struct rpcblk *) (devtab[dev].dvioblk))->rpc_reqgid = 0;

    /* make the RPC call and get the result */
    if ((control(dev, XSET_STRING, fs) == SYSERR) || 
	(control(dev, RPC_CALL) == SYSERR) ||
	(control(dev, XGET_UINT, &status) == SYSERR)) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("callmount: mount RPC call failed\n");
#endif
	close(dev);
	return(SYSERR);
    }

    if (status != NFS_OK) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("bad mount status: %s\n", nfsStatString(status));
#endif
	return(SYSERR);
    }

    /* get the file handle */
    hlen = 32;
    control(dev, XGET_FOPAQ, handle, hlen);

#ifdef DEBUG
    kprintf("Handle:\n", hlen);
    NfsPrintHandle(handle);
#endif
    close(dev);

    return(OK);
}


/*-------------------------------------------------------------------------
 * getfreemountentry - return the index of an unused entry in the
 *                     mount table, or SYSERR
 *-------------------------------------------------------------------------
 */
LOCAL getfreemountentry()
{
    int i;

    for (i = 0; i < MOUNTTABLESIZE; ++i) {
	if (mounttab[i].mnt_state == MOUNT_FREE) {
	    mounttab[i].mnt_state = MOUNT_RESERVED;
	    return(i);
	}
    }
    return(SYSERR);
}


/*-------------------------------------------------------------------------
 * dumpmountTable - print it all out for debugging
 *-------------------------------------------------------------------------
 */
dumpmountTable()
{
    int i;
    struct mountentry *pme;

    kprintf("Mount table:\n");
    for (i = 0; i < MOUNTTABLESIZE; ++i) {
	pme = &mounttab[i];

	if (pme->mnt_state != MOUNT_VALID)
	    continue;

	kprintf("%2d:%-3d  host: %s   fs: %   %d open files\n",
		i,
		pme->mnt_generation,
		pme->mnt_host,
		pme->mnt_fs,
		pme->mnt_opencount);
    }
    kprintf("\n");
}




