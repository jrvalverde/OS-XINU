/* kernel.h - halt, isodd, pause, min */

/* Symbolic constants used throughout Xinu */

#ifndef	ASM
typedef	char		Bool;		/* Boolean type			*/
typedef long		PStype[1];	/* type of PS save location	*/
#endif	ASM

#define	FALSE		0		/* Boolean constants		*/
#define	TRUE		1
#define	NULL		(char *)0	/* Null pointer for linked lists*/
#define	NULLCH		'\0'		/* The null character		*/
#define	NULLSTR		""		/* Pointer to empty string	*/
#define	SYSCALL		int		/* System call declaration	*/
#define	LOCAL		static		/* Local procedure declaration	*/
#define	COMMAND		int		/* Shell command declaration	*/
#define	BUILTIN		int		/* Shell builtin " "		*/
#define	INTPROC		int		/* Interrupt procedure  "	*/
#define	PROCESS		int		/* Process declaration		*/
#define	RESCHYES	1		/* tell	ready to reschedule	*/
#define	RESCHNO		0		/* tell	ready not to resch.	*/
#define	MINSHORT	0100000		/* minimum short integer 	*/
#define	MAXSHORT	077777		/* maximum short integer	*/
#define	LOWBYTE		0377		/* mask for low-order 8 bits	*/
#define	LOW6		077		/* mask for low-order 6 bits	*/
#define	LOW16		0177777		/* mask for low-order 16 bits	*/
					/* Following are indexes of	*/
					/* saved values of registers	*/
					/* in the process table		*/
					/* register save area:		*/
#define	SPINDX		0		/*   kernel stack pointer	*/
#define	APINDX		16		/*   argument pointer		*/
#define	FPINDX		17		/*   frame pointer		*/
#define	PCINDX		18		/*   program counter		*/
#define	PSINDX		19		/*   program status longword	*/
#define P0BRINDX	20		/*   p0 base register		*/
#define	P0LRINDX	21		/*   p0 length register		*/
#define	P1BRINDX	22		/*   p1 base register		*/
#define	MINSTK		128		/* minimum process stack size	*/
#define	NULLSTK		1024		/* process 0 stack size		*/
#define	MAGIC		0xaaaaaaaa	/* unusual value for top of stk	*/

/* Universal return constants */

#define	OK		 1		/* returned when system	call ok	*/
#define	SYSERR		-1		/* returned when sys. call fails*/
#define	EOF		-2		/* returned for end-of-file	*/
#define	TIMEOUT		-3		/* returned  "  "  "  times out	*/
#define	INTRMSG		-4		/* keyboard "intr" key pressed	*/
					/*  (usu. defined as ^B)	*/

/* Initialization constants */

#define	INITSTK		1024		/* initial process stack	*/
#define	INITPRIO	20		/* initial process priority	*/
#define	INITNAME	"main"		/* initial process name		*/
#define	INITARGS	1,0		/* initial count/arguments	*/
#define	INITRET		userret 	/* INITRET=process' return point*/
#define	INITPS		0x00000000	/* initial process PSL:		*/
					/* integer oflow trap disabled,	*/
					/* interrupts enabled		*/
#define	INITBR		0x80000000	/* P0 & P1 base register	*/
#define	INITLR		0x04000000	/* P0LR-> ASTLVL==none pending	*/
#define	INITREG		0		/* initial register contents	*/
#define	QUANTUM		10		/* clock ticks until preemption	*/

/* Miscellaneous utility inline functions */

#ifndef	ASM
#define	isodd(x)	(01&(int)(x))
#define pause()		asm("nop")	/* pause-like instruction	*/
#define	halt()		asm("jmp __halt")
#define	min(a,b)	((a) < (b) ? (a) : (b))
#endif	ASM

#ifndef ASM
extern	int	rdyhead, rdytail;
extern	int	preempt;
#endif	ASM
