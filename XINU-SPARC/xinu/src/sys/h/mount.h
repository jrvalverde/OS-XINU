/* mount.h - definitions for the NFS mount RPC calls */

/* supported version number */
#define RPCMOUNT_VERS			1

/* MOUNT procedure numbers */
#define RPCMOUNT_PROC_NULL		0
#define RPCMOUNT_PROC_MNT		1
#define RPCMOUNT_PROC_DUMP		2
#define RPCMOUNT_PROC_UMNT		3
#define RPCMOUNT_PROC_UMNTALL		4
#define RPCMOUNT_PROC_EXPORT		5

/* #defines for status */
#define RPCMOUNT_STATUS_FREE		0
#define RPCMOUNT_STATUS_RESERVED	1
#define RPCMOUNT_STATUS_VALID		2



#define MOUNTTABLESIZE	32		/* num entries in mount table	*/
#define MNTPATHLEN	1024

/* #defines for status */
#define MOUNT_FREE	0
#define MOUNT_RESERVED	1
#define MOUNT_VALID	2


struct mountentry {
	int		mnt_state;	/* is this entry free, or not?	*/
	int		mnt_mutex;
	int		mnt_generation;

	/* used for finding mountpoints, etc */
	IPaddr		mnt_hostIP;
	char		mnt_host[NFS_HOSTNAMELEN];
	char		mnt_fs[NFS_FSLEN];
	int		mnt_rpcdev;	/* RPC device to use for searching */

	/* timeout and attempts values */
	unsigned	mnt_timeout;
	unsigned	mnt_attempts;

	int		mnt_hcache;	/* handle cache		*/

	fhandle		mnt_handle;
	int		mnt_opencount;
	int		mnt_mountmode;
};

/* global variables */
extern int mountTableMutex;
extern struct mountentry mounttab[];

/* global routine definitions */
extern struct mountentry *nfsgetmount();
extern struct mountentry *nfsmount();
extern nfsumount();
