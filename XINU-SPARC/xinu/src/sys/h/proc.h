/* proc.h - isbadpid */

#ifdef XSELECT
#include <select.h>
#endif
#include <a.out.h>

/* process table declarations and defined constants			*/


#ifndef	NPROC				/* set the number of processes	*/
#define	NPROC		100		/*  allowed if not already done	*/
#endif

#ifndef	NADDR				/* set the number of addr spaces*/
#define	NADDR		NPROC		/*  allowed if not already done	*/
#endif


/* process state constants */

#define	PRCURR		'\001'		/* process is currently running	*/
#define	PRFREE		'\002'		/* process slot is free		*/
#define	PRREADY		'\003'		/* process is on ready queue	*/
#define	PRRECV		'\004'		/* process waiting for message	*/
#define	PRSLEEP		'\005'		/* process is sleeping		*/
#define	PRSUSP		'\006'		/* process is suspended		*/
#define	PRWAIT		'\007'		/* process is on semaphore queue*/
#define	PRTRECV		'\010'		/* process is timing a receive	*/

/* miscellaneous process definitions */

#ifdef SUN3
#define	PNREGS		19		/* size of saved register area	*/
#endif

#ifdef SPARC
#define	PNREGS		12		/* size of saved register area	*/
			/* PNREGS = 12 : 7 globals, sp, fp, pc, psr, y	*/
#endif

#define ASIDBASE	NPROC		/* first adrress space id	*/
#define	PNMLEN		8		/* length of process "name"	*/
#define	NULLPROC	0		/* id of the null process	*/
#define	XINUASID	ASIDBASE	/* id of the xinu address space	*/
#define	BADPID		-1		/* used when invalid pid needed	*/
#define BADASID		-1		/* used when invalid asid needed*/
#define	PNRETPOP	4		/* number of registers RET pops,*/
					/*  other than PC		*/

/* miscellaneous address space and process functions */
#define	isbadpid(x)	(x<0 || x>=NPROC)
#define isbadasid(x)	(x<NPROC || x>=NPROC+NADDR)
#define ispid(x)	(x>=0 && x<NPROC)
#define isasid(x)	(x>=NPROC && x<NPROC+NADDR)
#define pidindex(x)	(x)
#define asidindex(x)	(x-ASIDBASE)
#define iskernproc(x)	(proctab[x].asid == XINUASID)

/* process table entry */

struct	pentry	{
	char	pname[PNMLEN];		/* process name			*/
	int	pstate;			/* process state: PRCURR, etc.	*/
	int	pprio;			/* process priority		*/
	int	psem;			/* semaphore if process waiting	*/
	int	pmsg;			/* message sent to this process	*/
	int	phasmsg;		/* True iff pmsg is valid	*/
	int	pbase;			/* base of run time stack	*/
	int	pstklen;		/* stack length	in bytes	*/
	int	plimit;			/* lowest extent of stack	*/
	int	pargs;			/* initial number of arguments	*/
	int	paddr;			/* initial code address		*/
	int	pnxtkin;		/* next-of-kin notified of death*/
	Bool	ptcpumode;		/* proc is in TCP urgent mode	*/
	int	pdevs[2];		/* devices to close upon exit	*/
	int	asid;			/* address space id		*/
	int	tstamp;			/* timestamp			*/
	int	next_thd;		/* next thread this addr space  */
	int	prev_thd;		/* prev thread in this addr space*/
	Bool	psinformed;		/* page server knows about this pid*/
	int	pginsem;		/* wait on it when paging in	*/
#ifdef XSELECT
	fd_set  infds;                  /* input fds for xselect        */
	fd_set  outfds;                 /* output fds for xselect       */
#endif
	WORD	pregs[PNREGS];	/* process save area corresponding
				   to Sun Process Control Block, e.g.:
				     pregs[?]     register function
				   -----------  --------------------
					0-6	g1, ... , g7
					 7	sp
					 8	fp
					 9	pc
					10	psr
					11	y
					*/
	int	ctxt;			/* current context in mmu */
#define	SP		7 		/* User stack pointer		*/
#define FP		8		/* Frame pointer 		*/
#define	PC		9		/* Program Counter		*/
#define	PS		10		/* Processor Status Word	*/
#define YREG		11		/* Y Register			*/
};					/* end pentry declaration */

/* address space table */
struct aentry {
	Bool	valid;			/* this entry in use		*/
	int	tstamp;			/* timestamp on this entry	*/
	short	numthd;			/* number of threads in addr sp	*/
	short	thd_id;			/* pid of first thd in this addr sp */
	Bool	psinformed;		/* page server knows about this asid*/
	struct  exec aout;              /* the a.out header from the image */
        int     devload;                /* device to load the text from */
};					/* end aentry declaration	*/

extern	struct	pentry proctab[];
extern	struct	aentry addrtab[];
extern	int	numproc;		/* currently active processes	*/
extern	int	nextproc;		/* search point for free slot	*/
extern	int	numaddr;		/* currently active addr spaces	*/
extern	int	nextaddr;		/* search point for free slot	*/
extern	int	currpid;		/* currently executing process	*/
