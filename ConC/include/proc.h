/* proc.h - isbadpid */

/* process table declarations and defined constants			*/

#ifndef	NPROC				/* set the number of processes	*/
#define	NPROC		20		/*  allowed if not already done	*/
#endif

/* process state constants */

#define	PRCURR		'\01'		/* process is currently running	*/
#define	PRFREE		'\02'		/* process slot is free		*/
#define	PRREADY		'\03'		/* process is on ready queue	*/
#define	PRRECV		'\04'		/* process waiting for message	*/
#define	PRSLEEP		'\05'		/* process is sleeping		*/
#define	PRSUSP		'\06'		/* process is suspended		*/
#define	PRWAIT		'\07'		/* process is on semaphore queue*/

/* miscellaneous process definitions */

#define	PNREGS		4		/* size of saved register area	*/
#define PNSYS_ARGS	3		/* number of syscall args to sav*/
#define PNMLEN		8		/* length of process "name"	*/
#define NULLPROC	0		/* pid of null process		*/

#define	isbadpid(x)	(x<=0 || x>=NPROC)

/* process table entry */

struct	pentry	{
	char	pstate;			/* process state: PRCURR, etc.	*/
	int	pprio;			/* process priority		*/
	long	pregs[PNREGS];		/* saved regs. AP, FP, SP, PS	*/
	int	psem;			/* semaphore if process waiting	*/
	long	pmsg;			/* message sent to this process	*/
	int	phasmsg;		/* nonzero iff pmsg is valid	*/
	long	pbase;			/* base of run time stack	*/
	int	pstklen;		/* stack length			*/
	long	plimit;			/* lowest extent of stack	*/
	char	pname[PNMLEN];		/* process name			*/
	int	pargs;			/* initial number of arguments	*/
	long	paddr;			/* initial code address		*/
	int	sys_command;		/* current syscall command	*/
	int	sys_ret_val;		/* returned value from syscall	*/
	int	sys_errno;		/* errno after syscall		*/
	int	sys_sem;		/* syscall semephore		*/
	char	*tdir;			/* task directory (not yet impl)*/
	long	psys_args[PNSYS_ARGS];	/* (ptr to) syscall args to save*/
};

extern	struct	pentry proctab[];
extern	int	numproc;		/* currently active processes	*/
extern	int	nextproc;		/* search point for free slot	*/
extern	int	currpid;		/* currently executing process	*/
