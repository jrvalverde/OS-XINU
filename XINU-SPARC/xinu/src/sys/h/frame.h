/* frame.h - Declarations related to frame table and support routines */

/* low water mark - 1/16 of total # of frames */
#define FTLOWWATER	(16)		/* frames in use/free frames	*/
#define FTLOWSHIFT	4		/* log2 of FTLOWWATER		*/
#define FM_LOW		(ftinfo.frameavail >> FTLOWSHIFT)

/* high water mark - 1/8 of total # of frames */
#define FTHIGHWATER	(8)		/* frames in use/free frames	*/
#define FTHIGHSHIFT	3		/* log2 of FTHIGHWATER		*/
#define FM_HIGH         (ftinfo.frameavail >> FTHIGHSHIFT)

/* frame manager (global clock) constants */
/* Default dist. between clock hands in number of frames.		*/
/* It should be less than 1/4 of the available frames			*/
#define	FMCLKDIST	2*1024*1024/PGSIZ
#define FMSLEEP		50		/* frame mgr sleep time - 5 secs*/

/* info for the frame manager */
extern  int     fmgr;                   /* frame manager process id     */
/* frame manager process constant */
#define  PGRPLC         pgrplce         /* page replacement routine     */
#define  FMSTK          4096		/* frame manager stack size     */
#define  FMPRI          20              /* frame manager priority       */
#define  FMNAM          "frmgr"         /* frame manmager name          */
#define  FMARGC         0               /* frame manager argument count */

extern  int             PGRPLC();

/* frame table constants */
#define FTXTRAS		6		/* xtra fte entries for lists	*/
#define FTAHD		(ftinfo.frameavail) 	/* active list head	*/
#define FTATL		(ftinfo.frameavail+1)	/* active list tail	*/
#define FTRHD		(ftinfo.frameavail+2)	/* reclaim list head	*/
#define FTRTL		(ftinfo.frameavail+3)	/* reclaim list tail	*/
#define FTMHD		(ftinfo.frameavail+4)	/* modified list head	*/
#define FTMTL		(ftinfo.frameavail+5)	/* modified list tail	*/
#define FTINVALIDFRAME	(-5)		/* weird/invalid frame number	*/

#define FTNLIST		0		/* no list - locked		*/
#define FTALIST		1		/* active list value		*/
#define FTMLIST		2		/* modified list value		*/
#define FTRLIST		3		/* reclaim list value		*/

union	ftebits	{
	short	value;				/* all the bits */
	struct	{
		unsigned	used:1;		/* used_or_free bit */
		unsigned	shared:1;	/* is it a S/P frame */
		unsigned	wanted:1;	/* is this frame wanted*/
		unsigned	pgout:1;	/* frame being written out */
		unsigned	list:2;		/* which list a=1,m=2,r=3 */
		unsigned	unused:10;	/* unused bits */
	    } st;
	};

struct fte {				/* frame table entry		*/
	short	id;			/* process owning this frame 	*/
	union	ftebits	    bits;	/* various bits 		*/
	unsigned int	frame_num;	/* frame number			*/
	int     tstamp;		        /* addr space/process timestamp */
	int	pageno;			/* page number			*/
	int	next;			/* next pointer 		*/
	int	prev;			/* prev pointer 		*/
};

struct ftinfo {				/* frame table information	*/
    int frameavail;			/* total number of frames avail	*/
    int	framebase;			/* first frame available	*/
    int	ftsize;				/* # fte entries in frame table	*/
    int	alen;				/* length of active list	*/
    int	rlen;				/* length of reclaim list	*/
    int	mlen;				/* length of modified list	*/
    int	locked;				/* number of locked frames	*/
};

struct fminfo {				/* frame manager (clock) info	*/
    int hand1;				/* clock first hand		*/
    int hand2;				/* clock second hand		*/
};

/*
 * Memory layout stuff
 */
struct physmemdesc {
        unsigned int    addr;           /* starting address of memory segment*/
        unsigned int    size;           /* size of same */
};

extern	struct	fte *ft;		/* frame table array		*/
extern	int	ftfreesem;		/* Free frame semaphore		*/
extern	struct	ftinfo	ftinfo;		/* frame table info		*/
extern	int	pgfault();		/* page fault interrupt handler	*/
extern  struct	fminfo	fminfo;		/* frame manager info		*/

#define	isbadframe(x)	(x < 0 || (x > ftinfo.frameavail))
#define isfthead(x)	(((x)==FTAHD)||((x)==FTMHD)||((x)==FTRHD))
#define isfttail(x)	(((x)==FTATL)||((x)==FTMTL)||((x)==FTRTL))
#define	isfthdortl(x)	(isfthead(x)||isfttail(x))
#define ftused(x)	(ft[x].bits.st.used)
#define ftshared(x)	(ft[x].bits.st.shared)
#define ftwanted(x)	(ft[x].bits.st.wanted)
#define ftpgout(x)	(ft[x].bits.st.pgout)
#define ftlist(x)	(ft[x].bits.st.list)
#define ftlocked(x)	(ftlist(x)==FTNLIST)
#define ftonalist(x)	(ftlist(x)==FTALIST)
#define ftonmlist(x)	(ftlist(x)==FTMLIST)
#define ftonrlist(x)	(ftlist(x)==FTRLIST)

/* ** NOTE ** physical memory space may not be contiguous */

/* map a frame number to the index of the frame table */
#define frameindex(x)	(frame_to_index((x)))

/* map a frame table index to the frame number */
#define toframenum(x)	(ft[x].frame_num)
