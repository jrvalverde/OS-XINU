/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)exec.h	7.1 (Berkeley) 6/4/86
 */
/*
 * Header prepended to each a.out file.
 */
struct exec {
	long	a_magic;	/* magic number */
unsigned long	a_text;		/* size of text segment */
unsigned long	a_data;		/* size of initialized data */
unsigned long	a_bss;		/* size of uninitialized data */
unsigned long	a_syms;		/* size of symbol table */
unsigned long	a_entry;	/* entry point */
unsigned long	a_trsize;	/* size of text relocation */
unsigned long	a_drsize;	/* size of data relocation */
};
#define	OMAGIC	0407		/* old impure format */
#define	NMAGIC	0410		/* read-only text */
#define	ZMAGIC	0413		/* demand load format */
/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)a.out.h	5.1 (Berkeley) 5/30/85
 */
/*
 * Definitions of the a.out header
 * and magic numbers are shared with
 * the kernel.
 */
/*
 * Macros which take exec structures as arguments and tell whether
 * the file has a reasonable magic number or offsets to text|symbols|strings.
 */
#define	N_BADMAG(x) \
    (((x).a_magic)!=OMAGIC && ((x).a_magic)!=NMAGIC && ((x).a_magic)!=ZMAGIC)
#define	N_TXTOFF(x) \
	((x).a_magic==ZMAGIC ? 1024 : sizeof (struct exec))
#define N_SYMOFF(x) \
	(N_TXTOFF(x) + (x).a_text+(x).a_data + (x).a_trsize+(x).a_drsize)
#define	N_STROFF(x) \
	(N_SYMOFF(x) + (x).a_syms)
/*
 * Format of a relocation datum.
 */
struct relocation_info {
	int	r_address;	/* address which is relocated */
unsigned int	r_symbolnum:24,	/* local symbol ordinal */
		r_pcrel:1, 	/* was relocated pc relative already */
		r_length:2,	/* 0=byte, 1=word, 2=long */
		r_extern:1,	/* does not include value of sym referenced */
		:4;		/* nothing, yet */
};
/*
 * Format of a symbol table entry; this file is included by <a.out.h>
 * and should be used if you aren't interested the a.out header
 * or relocation information.
 */
struct	nlist {
	union {
		char	*n_name;	/* for use when in-core */
		long	n_strx;		/* index into file string table */
	} n_un;
unsigned char	n_type;		/* type flag, i.e. N_TEXT etc; see below */
	char	n_other;	/* unused */
	short	n_desc;		/* see <stab.h> */
unsigned long	n_value;	/* value of this symbol (or sdb offset) */
};
#define	n_hash	n_desc		/* used internally by ld */
/*
 * Simple values for n_type.
 */
#define	N_UNDF	0x0		/* undefined */
#define	N_ABS	0x2		/* absolute */
#define	N_TEXT	0x4		/* text */
#define	N_DATA	0x6		/* data */
#define	N_BSS	0x8		/* bss */
#define	N_COMM	0x12		/* common (internal to ld) */
#define	N_FN	0x1f		/* file name symbol */
#define	N_EXT	01		/* external bit, or'ed in */
#define	N_TYPE	0x1e		/* mask for all the type bits */
/*
 * Sdb entries have some of the N_STAB bits set.
 * These are given in <stab.h>
 */
#define	N_STAB	0xe0		/* if any of these bits set, a SDB entry */
/*
 * Format for namelist values.
 */
#define	N_FORMAT	"%08x"
/* conf.h (GENERATED FILE; DO NOT EDIT) */
#define	NULLPTR	(char *)0
/* Device table declarations */
#ifndef	ASM
struct	devsw	{			/* device table entry */
	int	dvnum;
	char	*dvname;
	int	(*dvinit)();
	int	(*dvopen)();
	int	(*dvclose)();
	int	(*dvread)();
	int	(*dvwrite)();
	int	(*dvseek)();
	int	(*dvgetc)();
	int	(*dvputc)();
	int	(*dvcntl)();
	int	dvcsr;
	int	dvivec;
	int	dvovec;
	int	(*dviint)();
	int	(*dvoint)();
	char	*dvioblk;
	int	dvminor;
	};
extern	struct	devsw devtab[];		/* one entry per device */
#endif
/* Device name definitions */
#define	CONSOLE     0			/* type tty      */
#define	ETHER       1			/* type eth      */
#define	INTERNET    2			/* type dgm      */
#define	DGRAM1      3			/* type dg       */
#define	DGRAM2      4			/* type dg       */
#define	DGRAM3      5			/* type dg       */
#define	DGRAM4      6			/* type dg       */
#define	DGRAM5      7			/* type dg       */
#define	DGRAM6      8			/* type dg       */
#define	RFILSYS     9			/* type rfm      */
#define	RFILE1      10			/* type rf       */
#define	RFILE2      11			/* type rf       */
#define	RFILE3      12			/* type rf       */
#define	RFILE4      13			/* type rf       */
#define	RFILE5      14			/* type rf       */
#define	RFILE6      15			/* type rf       */
#define	RFILE7      16			/* type rf       */
#define	RFILE8      17			/* type rf       */
#define	RFILE9      18			/* type rf       */
#define	RFILE10     19			/* type rf       */
#define	NAMESPACE   20			/* type nsys     */
/* Control block sizes */
#define	Ntty	1
#define	Neth	1
#define	Ndgm	1
#define	Ndg	6
#define	Nrfm	1
#define	Nrf	10
#define	Nnsys	1
#define	NDEVS	21
/* Declarations of I/O routines referenced */
#ifndef	ASM
extern	int	vcuinit();
extern	int	ttyopen();
extern	int	ionull();
extern	int	ttyread();
extern	int	vcuwrite();
extern	int	ioerr();
extern	int	vcucntl();
extern	int	ttygetc();
extern	int	vcuputc();
extern	int	vcuiin();
extern	int	vcuoin();
extern	int	ethinit();
extern	int	ethread();
extern	int	ethwrite();
extern	int	ethinter();
extern	int	dgmopen();
extern	int	dgmcntl();
extern	int	dginit();
extern	int	dgclose();
extern	int	dgread();
extern	int	dgwrite();
extern	int	dgcntl();
extern	int	rfopen();
extern	int	rfcntl();
extern	int	rfinit();
extern	int	rfclose();
extern	int	rfread();
extern	int	rfwrite();
extern	int	rfseek();
extern	int	rfgetc();
extern	int	rfputc();
extern	int	naminit();
extern	int	namopen();
#endif
/* Configuration and Size Constants */
#define	NPROC	100			/* number of user processes	*/
#define	NSEM	200			/* total number of semaphores	*/
#define	MEMMARK				/* define if memory marking used*/
#define	RTCLOCK				/* system has a real-time clock	*/
#define	STKCHK				/* resched checks stack overflow*/
#define	NETDAEMON			/* Ethernet network daemon runs	*/
#define	GATEWAY     128,10,2,70		/* Gateway address		*/
#define	TSERVER     "128.10.2.3:37"	/* Time server address		*/
#define	RSERVER     "128.10.2.8:2001"	/* Remote file server address	*/
#define	DSERVER     "128.10.2.8:53"	/* Domain Name server address	*/
#define	VERSION     "7.6 (12/10/86)"	/* label printed at startup	*/
					/* (vers7: Diskless Workstation)*/
/************************************************************************/
/*			Target machine type				*/
/* To reconfigure for a different target VAX system, change the		*/
/* following statement to define one of:				*/
/*									*/
/*	VAX8600 VAX785 VAX780 VAX750 VAX730 VAX725 UVAXII UVAXI		*/
/*									*/
/* Yes folks, we actually downloaded Xinu into an 8600 (but there's	*/
/* no Unibus Ethernet driver.						*/
/*									*/
/************************************************************************/
#define	UVAXI				/* machine type			*/
#ifdef	UVAXI
#define	SYSTEM		"MicroVAX I"	/* name printed at startup	*/
#endif
#ifdef	UVAXII
#define	SYSTEM		"MicroVAX II"
#endif
#ifdef	VAX725
#define	SYSTEM		"VAX 11/725"
#endif
#ifdef	VAX730
#define	SYSTEM		"VAX 11/730"
#endif
#ifdef	VAX750
#define	SYSTEM		"VAX 11/750"
#endif
#ifdef	VAX780
#define	SYSTEM		"VAX 11/780"
#endif
#ifdef	VAX785
#define	SYSTEM		"VAX 11/785"
#endif
#ifdef	VAX8600
#define	SYSTEM		"VAX 8600"
#endif
/* coreX.h */
/* Definitions and constants for VAX Xinu core dump files */
#define	REGISTERS 	24		/* number of g.p. registers	*/
#define	COREMAGIC	0477		/* unix "magic number" for core	*/
struct	coreX {				/* header of VAX core dump file	*/
	short c_magic;			/* UNIX convention		*/
	int   c_size;			/* size of valid info (0=>entire*/
					/*  file contains core dump	*/
	short c_zero1;			/* padding to follow UNIX style	*/
	short c_zero2;			/*    "     "    "     "    "	*/
	int   c_regs[REGISTERS];	/* dump of machine registers	*/
	int   c_psw;			/* dump of program status reg	*/
	short c_zero3;			/* more padding (as in UNIX)	*/
	short c_zero4;			/*   "     "      "  "   "      */
	short c_zero5;			/*   "     "      "  "   "      */
					/* Core image follows header	*/
};
#define	KSP	0
#define	ESP	1
#define	SSP	2
#define	USP	3
#define	R0	4
#define	R1	5
#define	R2	6
#define	R3	7
#define	R4	8
#define	R5	9
#define	R6	10
#define	R7	11
#define	R8	12
#define	R9	13
#define	R10	14
#define	R11	15
#define	AP	16
#define	FP	17
#define	PC	18
#define	PSL	19
#define	P0BR	20
#define	P0LR	21
#define	P1BR	22
#define	P1LR	23
/* kernel.h - halt, isodd, pause, min */
/* Symbolic constants used throughout Xinu */
#ifndef	ASM
typedef	char		Bool;		/* Boolean type			*/
typedef long		PStype[1];	/* type of PS save location	*/
#endif	ASM
#define	FALSE		0		/* Boolean constants		*/
#define	TRUE		1
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
/* q.h - firstid, firstkey, isempty, lastkey, nonempty */
/* q structure declarations, constants, and inline procedures		*/
#ifndef	NQENT
#define	NQENT		NPROC + NSEM + NSEM + 4	/* for ready & sleep	*/
						/* & preemption		*/
#endif
struct	qent	{		/* one for each process plus two for	*/
				/* each list				*/
	short	qkey;		/* key on which the queue is ordered	*/
	short	qnext;		/* pointer to next process or tail	*/
	short	qprev;		/* pointer to previous process or head	*/
	};
extern	struct	qent q[];
extern	int	nextqueue;
/* inline list manipulation procedures */
#define	isempty(list)	(q[(list)].qnext >= NPROC)
#define	nonempty(list)	(q[(list)].qnext < NPROC)
#define	firstkey(list)	(q[q[(list)].qnext].qkey)
#define lastkey(tail)	(q[q[(tail)].qprev].qkey)
#define firstid(list)	(q[(list)].qnext)
#define	EMPTY	-1		/* equivalent of null pointer		*/
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
/* sem.h - isbadsem */
#ifndef	NSEM
#define	NSEM		100	/* number of semaphores, if not defined	*/
#endif
#define	SFREE	'\01'		/* this semaphore is free		*/
#define	SUSED	'\02'		/* this semaphore is used		*/
struct	sentry	{		/* semaphore table entry		*/
	char	sstate;		/* the state SFREE or SUSED		*/
	short	semcnt;		/* count for this semaphore		*/
	short	sqhead;		/* q index of head of list		*/
	short	sqtail;		/* q index of tail of list		*/
};
extern	struct	sentry	semaph[];
extern	int	nextsem;
#define	isbadsem(s)	(s<0 || s>=NSEM)
/* tty.h */
#define	OBMINSP		20		/* min space in buffer before	*/
					/* processes awakened to write	*/
#define	EBUFLEN		20		/* size of echo queue		*/
/* size constants */
#ifndef	Ntty
#define	Ntty		1		/* number of serial tty lines	*/
#endif
#ifndef	IBUFLEN
#define	IBUFLEN		128		/* num.	chars in input queue	*/
#endif
#ifndef	OBUFLEN
#define	OBUFLEN		64		/* num.	chars in output	queue	*/
#endif
/* mode constants */
#define	IMRAW		'R'		/* raw mode => nothing done	*/
#define	IMCOOKED	'C'		/* cooked mode => line editing	*/
#define	IMCBREAK	'K'		/* honor echo, etc, no line edit*/
#define	OMRAW		'R'		/* raw mode => normal processing*/
struct	tty	{			/* tty line control block	*/
	int	ihead;			/* head of input queue		*/
	int	itail;			/* tail of input queue		*/
	char	ibuff[IBUFLEN];		/* input buffer for this line	*/
	int	isem;			/* input semaphore		*/
	int	ohead;			/* head of output queue		*/
	int	otail;			/* tail of output queue		*/
	char	obuff[OBUFLEN];		/* output buffer for this line	*/
	int	osem;			/* output semaphore		*/
	int	odsend;			/* sends delayed for space	*/
	int	ehead;			/* head of echo queue		*/
	int	etail;			/* tail of echo queue		*/
	char	ebuff[EBUFLEN];		/* echo queue			*/
	char	imode;			/* IMRAW, IMCBREAK, IMCOOKED	*/
	Bool	iecho;			/* is input echoed?		*/
	Bool	ieback;			/* do erasing backspace on echo?*/
	Bool	evis;			/* echo control chars as ^X ?	*/
	Bool	ecrlf;			/* echo CR-LF for newline?	*/
	Bool	icrlf;			/* map '\r' to '\n' on input?	*/
	Bool	ierase;			/* honor erase character?	*/
	char	ierasec;		/* erase character (backspace)	*/
	Bool	ikill;			/* honor line kill character?	*/
	char	ikillc;			/* line kill character		*/
	Bool	iintr;			/* is interrupt char honored?	*/
	char	iintrc;			/* interrupt character		*/
	int	iintpid;		/* interrupt process id		*/
	Bool	ieof;			/* honor end-of-file char?	*/
	char	ieofc;			/* end-of-file character	*/
	int	icursor;		/* current cursor position	*/
	Bool	oflow;			/* honor ostop/ostart?		*/
	Bool	oheld;			/* output currently being held?	*/
	char	ostop;			/* character that stops output	*/
	char	ostart;			/* character that starts output	*/
	Bool	ocrlf;			/* echo CR/LF for LF ?		*/
	char	ifullc;			/* char to send when input full	*/
	struct	csr	*ioaddr;	/* device address of this unit	*/
};
extern	struct	tty tty[];
#define	BACKSP	'\b'			/* backspace one character pos.	*/
#define	BELL	'\007'			/* usually an audiable tone	*/
#define	BLANK	' '			/* used to print a "space"	*/
#define	KILLCH	'\025'			/* line kill character (^U)	*/
#define	NEWLINE	'\n'			/* line feed			*/
#define	RETURN	'\r'			/* carriage return		*/
#define	STOPCH	'\023'			/* control-S stops output	*/
#define	STRTCH	'\021'			/* control-Q restarts output	*/
#define	INTRCH	'\002'			/* control-B is interrupt	*/
#define	UPARROW	'^'			/* usually for visuals like ^X	*/
/* ttycontrol function codes */
#define	TCSETBRK	1		/* turn on BREAK in transmitter	*/
#define	TCRSTBRK	2		/* turn off BREAK "       "	*/
#define	TCNEXTC		3		/* look ahead 1 character	*/
#define	TCMODER		4		/* set input mode to raw	*/
#define	TCMODEC		5		/* set input mode to cooked	*/
#define	TCMODEK		6		/* set input mode to cbreak	*/
#define	TCICHARS	8		/* return number of input chars	*/
#define	TCECHO		9		/* turn on echo			*/
#define	TCNOECHO	10		/* turn off echo		*/
#define	TCINT		11		/* set input interrupt pid	*/
#define	TCINTCH		12		/* set input interrupt char	*/
#define	TCNOINT		13		/* turn off input interrupt	*/
#define	TFULLC		BELL		/* char to echo when buffer full*/
/* date.h - net2xt, xt2net, isleap, ut2ltim */
/* Xinu stores time as seconds past Jan 1, 1970 (UNIX format), with	*/
/* 1 being 1 second into Jan. 1, 1970, GMT (universal time).  The	*/
/* Internet uses seconds past Jan 1, 1900 (also GMT or universal time)	*/
#define	net2xt(x)	((x)-2208988800L)  /* convert net-to-xinu time	*/
#define	xt2net(x)	((x)+2208988800L)  /* convert xinu-to-net time	*/
/* Days in months and month names used to format a date */
struct	datinfo	{
	int	dt_msize[12];
	char	*dt_mnam[12];
};
extern	struct	datinfo	Dat;
/* Constants for converting time to month/day/year/hour/minute/second	*/
#define	isleap(x)	((x)%4==0)	/* leap year? (1970-1999)	*/
#define	SECPERDY	(60L*60L*24L)	/* one day in seconds		*/
#define	SECPERHR	(60L*60L)	/* one hour in seconds		*/
#define	SECPERMN	(60L)		/* one minute in seconds	*/
/* date doesn't understand daylight savings time (it was built in	*/
/*	Indiana where we're smart enough to realize that renumbering	*/
/*	doesn't save anything).	However, the local time zone can be	*/
/*	set to EST, CST, MST,or PST.					*/
#define	ZONE_EST	5		/* Eastern Standard time is 5	*/
#define	ZONE_CST	6		/*  hours west of England	*/
#define	ZONE_MST	7
#define	ZONE_PST	8
#define	TIMEZONE	ZONE_EST	/* timezone for this system	*/
/* In-line procedure that converts universal time to local time		*/
#define	ut2ltim(x)	((x)-TIMEZONE*SECPERHR)
#ifndef	TSERVER
#define	TSERVER		"128.10.2.3:37"
#endif
