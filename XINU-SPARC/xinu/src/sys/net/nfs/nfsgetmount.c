/* nfsgetmount.c -- nfsgetmount */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <nfs.h>
#include <mount.h>

/*#define DEBUG*/
#define PRINTERRORS


/*-------------------------------------------------------------------------
 * nfsgetmount - return a pointer to a mount table entry for the given
 *               host and file system, if they're valid.  If that file
 *               system is not mounted, we'll attempt to mount it
 *               N.B. assumes mount table mutex is held
 *-------------------------------------------------------------------------
 */
struct mountentry *nfsgetmount(host, fs)
     char *host;
     char *fs;
{
    int tablefull;
    int i;
    struct mountentry *pme;

#ifdef DEBUG
    kprintf("getmount(%s,%s) called\n", host, fs);
#endif

    tablefull = TRUE;
    for (i = 0; i < MOUNTTABLESIZE; ++i) {
	pme = &mounttab[i];

	if (pme->mnt_state == MOUNT_FREE)
	    tablefull = FALSE;

	if ((pme->mnt_state == MOUNT_VALID) &&
	    (strcmp(host,pme->mnt_host) == 0) && 
	    (strcmp(fs,pme->mnt_fs) == 0)) {
	    /* here it is, and it's already mounted */
#ifdef DEBUG
	    kprintf("getmount(%s,%s) already mounted at slot %d\n",
		    host, fs, i);
#endif
	    return(pme);
	}
    }

    if (tablefull) {
#ifdef DEBUG
	kprintf("getmount: not mounted, but table is full\n");
#endif
	return((struct mountentry *) SYSERR);
    }

    /* wasn't there, try to mount it */
#ifdef DEBUG
    kprintf("getmount(%s,%s), trying to mount\n", host, fs);
#endif

    pme = nfsmount(host, fs, 0);

    return(pme);
}
