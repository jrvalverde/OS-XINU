/* ktabobj.h - contains definitions of archindep object structures */

/* array sizes */
#define MAXFNAME        128             /* max file name len            */
#define MAXPNAME        128             /* max process name len         */
#define MAXDNAME        32              /* max device name len          */

/* process states */
#define	PSCURR		'\001'		/* process is currently running	*/
#define	PSFREE		'\002'		/* process slot is free		*/
#define	PSREADY		'\003'		/* process is on ready queue	*/
#define	PSRECV		'\004'		/* process waiting for message	*/
#define	PSSLEEP		'\005'		/* process is sleeping		*/
#define	PSSUSP		'\006'		/* process is suspended		*/
#define	PSWAIT		'\007'		/* process is on semaphore queue*/
#define	PSTRECV		'\010'		/* process is timing a receive	*/

/* type definitions */
typedef int     Address;
typedef char    Boolean;

/* thread/process table entry -- arch indep version from struct pentry */
struct  pentry_ai {
        int     pid;                    /* process id                   */
        char    pname[MAXPNAME];        /* process name                 */
        char    pstate;                 /* process state: PRCURR, etc.  */
        short   pprio;                  /* process priority             */
        short   asid;                   /* address space id             */
        char    stdindev[MAXDNAME];     /* standard input device name   */
        char    stdoutdev[MAXDNAME];    /* standard output device name  */
        Address paddr;                  /* initial code addr - entry pt */
        int     psem;                   /* semaphore if process waiting */
        int     pmsg;                   /* message sent to this process */
        Boolean phasmsg;                /* True iff pmsg is valid       */
        Address pbase;                  /* base of run time stack       */
        int     pstklen;                /* stack length in bytes        */
        short   pnxtkin;                /* next-of-kin notified of death*/
        short   next_thd;               /* next thread this addr space  */
        short   prev_thd;               /* prev thread in this addr space*/
};

/* ktab object table -- arch indep version derived from struct ktspec */
struct ktspec_ai {
	char 	name[16];
	char 	descr[64];
	int	u_entrysize;
	int	k_entrysize;
	int	maxrows;
	int	maxcols;
	Address	baseaddr;
};

/* arch independent version of struct aentry -- the address space table */
struct aentry_ai {
	Boolean	valid;
	int	tstamp;
	short	numthd;
	short	thd_id;
	Boolean	psinformed;
};

/* arch independent version of struct sentry -- the semaphore table */
/* exactly the same as the sentry in sem.h */
struct	sentry_ai  {		/* semaphore table entry		*/
	char	sstate;		/* the state SFREE or SUSED		*/
	int	semcnt;		/* count for this semaphore		*/
	int	sqhead;		/* q index of head of list		*/
	int	sqtail;		/* q index of tail of list		*/
};
#define	SFREE	'\01'		/* this semaphore is free		*/
#define	SUSED	'\02'		/* this semaphore is used		*/

/* arch independent version of struct bpool -- the buffer pool table */
struct bpool_ai {
	int bpsize;
	char *filler;		/* this is where the bpnext pointer is 	*/
	int bpsem;
};

/* arch independent version of struct pt -- the port table */
struct pt_ai {
	char	ptstate;		/* port state (FREE/LIMBO/ALLOC)*/
	int	ptssem;			/* sender semaphore		*/
	int	ptrsem;			/* receiver semaphore		*/
	int	ptmaxcnt;		/* max messages to be queued	*/
	int	ptseq;			/* sequence changed at creation	*/
};	
#define	PTFREE		'\01'		/* port is Free			*/
#define	PTLIMBO		'\02'		/* port is being deleted/reset	*/
#define	PTALLOC		'\03'		/* port is allocated		*/
#define	PTEMPTY		-1		/* initial semaphore entries	*/


/* arch independent version of struct fte -- the frame entry table 	*/
union	ftebits_ai {
	short	value;				/* all the bits */
	struct	{
		unsigned	used:1;		/* used_or_free bit */
		unsigned	shared:1;	/* is it a S/P frame */
		unsigned	wanted:1;	/* is this frame wanted*/
		unsigned	pgout:1;	/* frame being written out */
		unsigned	list:2;		/* which list a=1,m=2,r=3 */
		unsigned	unused:10;	/* unused bits */
	    }st;
	};
struct fte_ai {
	short	id;			/* process owning this frame */
	int	next;			/* next pointer */
	int	prev;			/* prev pointer */
	union	ftebits_ai    bits;	/* various bits */
	int     tstamp;		        /* addr space/process timestamp */
	int	pageno;			/* page number			*/
};

extern int psread();
extern int pswrite();
extern int readobj();		/* generic read routine */
extern int writeobj();		/* generic write routine */
