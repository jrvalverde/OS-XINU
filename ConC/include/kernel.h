/* kernel.h */

#include <sys/types.h>

/* Symbolic constants used throughout ConC */

typedef	int		Bool;		/* Boolean type			*/

#define DONTCARE	0

#define	FALSE		0		/* Boolean constants		*/
#define	TRUE		1
#define	NULL		(char *)0	/* Null pointer for linked lists*/
#define	SYSCALL		int		/* System call declaration	*/
#define	LOCAL		static		/* Local procedure declaration	*/
#define	INTPROC		int		/* Interrupt procedure  "	*/
#define	PROCESS		int		/* Process declaration		*/
#define	RESCHYES	1		/* tell	ready to reschedule	*/
#define	RESCHNO		0		/* tell	ready not to resch.	*/
#define	MININT		020000000000	/* minimum integer		*/
#define	MAXINT		017777777777	/* maximum integer		*/
#define	AP		0		/* stored regs[0] is arg ptr	*/
#define	FP		1		/* stored regs[1] is frame ptr	*/
#define	SP		2		/* stored regs[2] is stack ptr	*/
#define	PS		3		/* stored regs[3] is sigvec mask*/
#define	MINSTK		1000		/* minimum process stack size	*/
#define	OK		 0		/* returned when system	call ok	*/
#define	SYSERR		-1		/* returned when sys call fails */
#define	NPROC		50		/* max number of processes	*/
#define NSEM		(FD_SETSIZE+100)/* number of semaphores		*/
#define MEMMARK				/* memory marking yes		*/
#define VERSION		"ConC (Version 4.3.1)"

/* initialization constants */

#define INITNAME	"prmain"	/* main process name		*/
#define INITSTK		1000		/* main process stack size	*/
#define	INITPRIO	20		/* initial process priority	*/
#define	INITRET		userret		/* processes return address	*/
#define START_NAME	"unnamed"	/* task name for start_task	*/
#define	DISABLE		027776377761	/* disable (soft) interrupts	*/
					/* all but SIGINT SIGCONT	*/
					/* 	SIGTSTP SIGQUIT	SIGILL	*/
#define ENABLE		0		/* enable  (soft) interrupts	*/
#define	SERV_DISABLE	027777777777	/* disable all (soft) interrupts*/
#define	SERV_ENABLE	027756377767	/* disable all but SIGIO, SIGILL*/
					/* SIGTSTP and SIGCONT ints for */
					/* server (so I can catch an	*/
					/* interrupt to sys__exit the	*/
					/* process without concern	*/
					/* about a descriptor being	*/
					/* worked on as non-blocking)	*/
#define S_TIME_QUANT	0		/* default time quantum seconds	*/
#define U_TIME_QUANT	200000		/* default time quantum u_secs	*/

/* misc. utility inline	functions */

#define	isodd(x)	(01&(int)(x))
#define	disable(ps)	(ps = sys_sigsetmask(DISABLE))/* disable soft inter */
#define	restore(ps)	sys_sigsetmask(ps)	/* restore soft interrup*/
#define	enable()	sys_sigsetmask(ENABLE)	/* enable soft interrupt*/
#define	serv_disable(ps)	(ps = sys_sigsetmask(SERV_DISABLE))
						/* disable soft inter for serv*/
#define	serv_restore(ps)	sys_sigsetmask(ps)
						/* restore soft inter for serv*/
#define	serv_enable()		sys_sigsetmask(SERV_ENABLE)
						/* enable soft inter  for serv*/
#define	xpause()	sys_sigpause(0)		/* wait for interrupt.	*/
#define	halt()		exit(0)			/* halt	instruction	*/
#define isbadsig(x)	((x) == SIGALRM || (x) == SIGVTALRM || (x) == SIGIO  \
							    || (x) == SIGILL)
						/* signals not allowed	*/

#define TIME_QUANT_FILE	"time_quant"		/* for optional time quant    */

extern struct itimerval	vtival,vtioval;		/* virt timer structures*/
extern struct itimerval	rtival,rtioval;		/* real timer structures*/

extern	int	rdyhead, rdytail;

extern	int	_ccc__userproc();/* users main process			*/

extern	int	clockq;		/* q index of sleeping process list	*/
extern	int	slnempty;	/* 1 iff clockq is nonempty		*/
extern	int	squantum;	/* sec  clk ticks til preemption	*/
extern	int	usquantum;	/* usec clk ticks til preemption	*/

extern	int	sem_sys_write;	/* sys_write semaphore			*/
extern	int	sem_sys_gen;	/* mutex sem for gen routines		*/
extern	int	sem_sys_malloc;	/* mutex sem for malloc routines	*/
extern	int	sem_sys_dir;	/* mutex sem for dir routines		*/
extern	int	sem_sys_net;	/* mutex sem for net routines		*/
extern	int	sem_sys_compat; /* mutex sem for compat routines	*/
extern	int	errno;		/* actual errno				*/
extern	int	ConC_SYS_PIPE;	/* fd for ConC side of pipe		*/
extern	int	Serv_SYS_PIPE;	/* fd for io_server side of pipe	*/
extern	int	SYS_PIPE;	/* current process's fd pipe		*/
extern	int	server_pid;	/* pid of server (for kill at xdone)	*/
extern	int	is_serv_init;	/* io_server initialized ??		*/
extern	char	cwd[];		/* global current working directory	*/

/* sleep definitions */

#define time_to_s10(sec,usec)		(10*(sec) + (usec)/100000)
#define s10_to_time(s10,sec,usec)	sec  = (s10)/10,\
					usec = ((s10) - 10*sec)*100000
