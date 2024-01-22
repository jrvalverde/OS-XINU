/* nfsminit.c - nfsminit */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nfs.h>
#include <mount.h>


/*#define DEBUG*/
#define PRINTERRORS


/*
 * ====================================================================
 * nfsminit - initialize the nfs device set, including the mount table.
 * ====================================================================
 */
nfsminit()
{
	int i;

	if ((nfsTableMutex = screate(1)) == SYSERR) {
		panic("nfsminit: not enough semaphores for nfsTableMutex\n");
		return(SYSERR);
	}

	if ((mountTableMutex = screate(1)) == SYSERR) {
		panic("nfsminit: not enough semaphores for mountTableMutex\n");
		return(SYSERR);
	}

	for (i = 0; i < MOUNTTABLESIZE; ++i) {
		mounttab[i].mnt_state = MOUNT_FREE;
		mounttab[i].mnt_generation = 0;
	}

	return(OK);
}

int nfsTableMutex;
struct nfsblk nfstab[Nnfs];

int mountTableMutex;
struct mountentry mounttab[MOUNTTABLESIZE];
