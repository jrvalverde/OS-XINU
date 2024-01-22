/* proc.h - isbadpid */

/* process table declarations and defined constants			*/

#ifndef	NPROC				/* set the number of processes	*/
#define	NPROC		100		/*  allowed if not already done	*/
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

#define	PNREGS		24		/* size of saved register area	*/
#define	PNRETPOP	4		/* number of registers RET pops,*/
					/*  other than PC		*/
#define	PNMLEN		8		/* length of process "name"	*/
#define	NULLPROC	0		/* id of the null process; it	*/
					/*  is always eligible to run	*/
#define	BADPID		-1		/* used when invalid pid needed	*/

#define	isbadpid(x)	(x<=0 || x>=NPROC)

/* process table entry */

struct	pentry	{
	int	pregs[PNREGS];		/* process save area corresponding
					   to VAX Process Control Block, e.g.:
					     pregs[?]     register function
					   -----------  --------------------
						0	kernel stack pointer
						1	executive stack ptr
						2	supervisor stack ptr
						3	user stack pointer
						4	general register R0
						5-14	general regs R1-R10
						15	general register R11
						16	argument pointer (R12)
						17	frame pointer (R13)
						18	program counter (R15)
						19	processor status lword
						20	P0BR
						21	P0LR
						22	P1BR
						23	P1LR		*/
	char	pstate;			/* process state: PRCURR, etc.	*/
	short	pprio;			/* process priority		*/
	int	psem;			/* semaphore if process waiting	*/
	int	pmsg;			/* message sent to this process	*/
	Bool	phasmsg;		/* True iff pmsg is valid	*/
	int	pbase;			/* base of run time stack	*/
	int	pstklen;		/* stack length	in bytes	*/
	int	plimit;			/* lowest extent of stack	*/
	char	pname[PNMLEN];		/* process name			*/
	short	pargs;			/* initial number of arguments	*/
	int	paddr;			/* initial code address		*/
	short	pnxtkin;		/* next-of-kin notified of death*/
	short	pdevs[2];		/* devices to close upon exit	*/
};

extern	struct	pentry proctab[];
extern	int	numproc;		/* currently active processes	*/
extern	int	nextproc;		/* search point for free slot	*/
extern	int	currpid;		/* currently executing process	*/
