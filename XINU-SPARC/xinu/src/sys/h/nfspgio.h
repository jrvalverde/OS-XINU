/*---------------------------------------------------------------------------
 * nfspgio.h - NFS page in/out head file
 *---------------------------------------------------------------------------
 */

/* constants for NFS paging swap directory */
/*
 * The default uid and gid for accessing the swap directory is set to
 * "nobody". Make sure your swap directory is "world" readable and writable.
 * Or you can change the uid and gid to different values in
 * sys/conf/Configuration file.
 */
#define NFS_SWAP_HOST   nfspgpstohost()
/*#define NFS_SWAP_FS     "/usr"*/
#define NFS_SWAP_FS     "/u6"
/*#define NFS_SWAP_DIR    "/export/swap"*/
#define NFS_SWAP_DIR    "/lin/xinu8.1/swap"
#define NFS_SWAP_ROOT   "swap/"

/* define the maximum number of requests that can be on the paging	*/
/* lists.  The create/terminate list can only have one request per	*/
/* process and address space at one time, and the page fetch list	*/
/* can only have one per process.  2*NPROC seems safe for both...	*/
  

#define RQNUM		(2*NPROC)	/* number of request nodes	*/

#define CREATE_REQ	1		/* proc. creation request	*/
#define CRSTOR_REQ	2		/* proc. create&page store rqt	*/
#define STORE_REQ	3		/* page store request		*/
#define FETCH_REQ	4		/* page fetch request		*/
#define FETCH_AOUT      5               /* page fetch request           */
#define TERMIN_REQ	6		/* proc. termination request	*/
#define START_REQ	13		/* system start request		*/
#define END_REQ		14		/* system end request		*/
					/* out of sequence message	*/
#define NFSPGPROC       nfspgioproc     /* nfs paging thread            */
#define NFSPGSTK        0x2000		/* nfs paging stack size        */
#define NFSPGPRI        98              /* nfs paging priority          */
#define NFSPGIN         "nfspgin"       /* nfs paging name              */
#define NFSPGOUT        "nfspgout"      /* nfs paging name              */

/* structure of paging requests */
struct pgreq {
	int	pm_type;	/* message type			*/
	int	pm_num;		/* message number		*/
	int	pm_wakepid;	/* process id to awaken		*/
	int	pm_id;		/* process or address-space id	*/
	unsigned pm_vaddr;	/* address			*/
	int	pm_fn;		/* frame num			*/
};
