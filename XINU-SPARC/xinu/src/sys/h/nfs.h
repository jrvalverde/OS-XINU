/* nfs.h - nfs definitions */

#define NFS_VERSION	2

/* NFS procedure numbers */
#define NFSPROC_NULL		0
#define NFSPROC_GETATTR		1
#define NFSPROC_SETATTR		2
#define NFSPROC_ROOT		3
#define NFSPROC_LOOKUP		4
#define NFSPROC_READLINK	5
#define NFSPROC_READ		6
#define NFSPROC_WRITECACHE	7
#define NFSPROC_WRITE		8
#define NFSPROC_CREATE		9
#define NFSPROC_REMOVE		10
#define NFSPROC_RENAME		11
#define NFSPROC_LINK		12
#define NFSPROC_SYMLINK		13
#define NFSPROC_MKDIR		14
#define NFSPROC_RMDIR		15
#define NFSPROC_READDIR		16
#define NFSPROC_STATFS		17

#define NFS_MAX_SYMLINKS	16

/* possible values for the status return code */
#define NFS_OK			0
#define NFSERR_PERM		1
#define NFSERR_NOENT		2
#define NFSERR_IO		5
#define NFSERR_NXIO		6
#define NFSERR_ACCES		13
#define NFSERR_EXIST		17
#define NFSERR_NODEV		19
#define NFSERR_NOTDIR		20
#define NFSERR_ISDIR		21
#define NFSERR_FBIG		27
#define NFSERR_NOSPC		28
#define NFSERR_ROFS		30
#define NFSERR_NAMETOOLONG	63
#define NFSERR_NOTEMPTY		66
#define NFSERR_DQUOT		69
#define NFSERR_STALE		70
#define NFSERR_WFLUSH		99
/* extended codes, returned by Xinu */
#define NFSERR_ABSSYMLINK	1001	/* contained absolute symlink	*/
#define NFSERR_RPCERR		1002	/* misc. RPC error		*/
#define NFSERR_PATHLEN		1003	/* path length too long		*/
#define NFSERR_BADNAME		1004	/* not a valid Xinu NFS name	*/
#define NFSERR_NOMOUNT		1005	/* mount daemon not responding	*/
#define NFSERR_RPCTIMEOUT	1006	/* rpc call timed out		*/


struct nfs_timeval {
	unsigned seconds;
	unsigned useconds;
};


struct sattr {
	unsigned		sa_mode;
	unsigned		sa_uid;
	unsigned		sa_gid;
	unsigned		sa_size;
	struct nfs_timeval	sa_atime;
	struct nfs_timeval	sa_mtime;
};


/* defines for NFS file types */
#define NFNON	0
#define NFREG	1
#define NFDIR	2
#define NFBLK	3
#define NFCHR	4
#define NFLNK	5



struct fattr {
	unsigned		fa_type;
	unsigned		fa_mode;
	unsigned		fa_nlink;
	unsigned		fa_uid;
	unsigned		fa_gid;
	unsigned		fa_size;
	unsigned		fa_blocksize;
	unsigned		fa_rdev;
	unsigned		fa_blocks;
	unsigned		fa_fsid;
	unsigned		fa_fileid;
	struct nfs_timeval	fa_atime;
	struct nfs_timeval	fa_mtime;
	struct nfs_timeval	fa_ctime;
};


#define NFS_SEPARATOR		'/'	/* filename separator		*/
#define NFS_FHSIZE		32
#define NFS_MAXDATA		8192
#define NFS_PATHLEN		1024
#define NFS_FILELEN		256	/* max length of a file name	*/
#define NFS_HOSTNAMELEN		128	/* max length of a host name	*/
#define NFS_FSLEN		64	/* max length of a host name	*/
#define NFS_COOKIESIZE		4
#define NFS_FHSIZE		32

/* definition of the NFS file handle */

typedef unsigned char fhandle[NFS_FHSIZE];

struct diropargs {
	fhandle handle;
	char name[NFS_HOSTNAMELEN];
};

/* #defines for nfs_state */
#define NFS_STAT_FREE	1
#define NFS_STAT_USED	2

struct nfsblk {
	int		nfs_state;		/* is this entry used or available */
	int		nfs_dnum;		/* File's device num in devtab	*/
	int		nfs_mutex;		/* for exclusive access to file	*/

	struct mountentry *nfs_pme;		/* pointer into mount table */
	int		nfs_mgeneration;	/* ensure coherency */
	int		nfs_rpcdev;		/* RPC device to use	*/

	fhandle		nfs_fhandle;		/* the file's handle	*/
	int		nfs_fhandle_valid;
	char		nfs_fpath[NFS_PATHLEN+4];
	char		*nfs_fname;		/* last comp of file name */
	fhandle		nfs_phandle;		/* the parent's handle	*/
	int		nfs_phandle_valid;

	unsigned	nfs_status;		/* status of last nfs operation */
	struct fattr	nfs_attributes;
	unsigned	nfs_ftype;		/* redundant in attributes */

	int		nfs_mode;		/* mode file was opened with */
	unsigned	nfs_fpos;
};

extern struct nfsblk nfstab[];
extern int nfsTableMutex;


/* nfs routine declarations */
char *nfsStatString();


/* definitions of control calls on the master device */
#define NFS_STATFILE	 201
#define NFS_LSTATFILE	 202
#define NFS_READLINK	 203
#define NFS_MKDIR	 204
#define NFS_RMDIR	 205


/* definitions of control calls on a pseudo device */
#define NFS_GETRPCSTATUS 101
#define NFS_READDIR	 102
#define NFS_GETATTR	 103
#define NFS_GETSTATUS	 104
#define NFS_TRUNCFILE	 105
#define NFS_SEEK	 106



/* Xinu's control call interface for reading directories */
struct nfs_readdir_entry {
	unsigned	fileid;
	char		filename[NFS_FILELEN];
	unsigned	cookie;
};
struct nfs_readdir_req {	/* first arg to NFS_READDIR */
	unsigned	cookie;		/* where to start, 0 = first	*/
	unsigned	nentries;	/* number of entries, in/out	*/
	struct nfs_readdir_entry *pent;	/* where to store responses	*/
	int		eof;		/* is that all?			*/
};


/* definitions of constants for the nfsfindfile() procedure */
#define NFS_MUST_EXIST		TRUE
#define NFS_NEEDNT_EXIST	FALSE

/* what Xinu uses for modes, etc for files it creates */
#define NFS_CREATE_UID		XINU_UID
#define NFS_CREATE_GID		XINU_GID
#define NFS_CREATE_MODE_FILE	0644
#define NFS_CREATE_MODE_DIR	0755
