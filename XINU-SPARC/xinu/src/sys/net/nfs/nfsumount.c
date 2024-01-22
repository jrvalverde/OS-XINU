/* nfsumount.c -- UNmount a filesystem by host and  name*/

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <nfs.h>
#include <mount.h>


nfsumountentry(entry)	/* flush mount table entry */
{
#ifdef OLD
	/* possible problem here- we get both mutexes.  watch for deadlock.
		MOREWORK */
	struct xdr request;
	struct rpc reply;
	unsigned char req_data[XDRUNSIGNEDLEN + MNTPATHLEN];
	int retval;
	extern semaphore rpcHostTableMutex, mountTableMutex;
	unsigned sequence;
	int dg;

	dg = SYSERR;
	xdrinit(&request);
	xdrsetbuf(&request, req_data, sizeof(req_data));
	rpcinit(&reply);
	wait (mountTableMutex);
	if (MOUNT_VALID != mounttab[entry].status) {
		kprintf("nfsumount: mount entry %d not valid\n", entry);
		signal(mountTableMutex);
		return(SYSERR);
	}
	if (0 != mounttab[entry].opencount) {
		kprintf("nfsumount: (%s) device busy\n");
		signal(mountTableMutex);
		return(SYSERR);
	}

	/* now actually try to unmount the filesystem */
	sequence = rpcGetSequence();
	unsignedxdr(&request, sequence);
	unsignedxdr(&request, CALL);
	unsignedxdr(&request, RPCVERS);
	unsignedxdr(&request, MOUNTPROG);
	unsignedxdr(&request, MOUNTVERS);
	unsignedxdr(&request, MOUNTPROC_UMNT);
	unsignedxdr(&request, AUTH_NULL);
	unsignedxdr(&request, 0);
	unsignedxdr(&request, AUTH_NULL);
	unsignedxdr(&request, 0);
	stringxdr(&request, mounttab[entry].filesystem);

	retval = rpcCall(&dg, sequence, mounttab[entry].pmountHI, req_data,
	 11 * XDRUNSIGNEDLEN+XDRROUNDLEN(strlen(mounttab[entry].filesystem)),
	 &reply, mounttab[entry].timeout, mounttab[entry].attempts);

	if (SYSERR == retval || RPC_ERROR_TIMEOUT == retval)
		kprintf("no response to UNMOUNT request.  Umount anyway\n");

	/* decrement the reference counts */
	releaseRpcHostInfo(mounttab[entry].pnfsHI);
	releaseRpcHostInfo(mounttab[entry].pmountHI);

	mounttab[entry].status = MOUNT_FREE;
	dumpmountTable();
	rpcfree(&reply);
	if (SYSERR != dg)
		close(dg);
	signal(mountTableMutex);
	return(retval);
#endif
}


		
nfsumountremote(host, filesys)
	char *host, *filesys;
{
#ifdef OLD
	int mindex;

	mindex = getmountbyremote(host, filesys);
	if (SYSERR == mindex) {
		kprintf("nfsumount: mount entry not found\n");
		return(SYSERR);
	}
	return(nfsumountentry(mindex));
#endif
}
