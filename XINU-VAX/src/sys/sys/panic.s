/* panic.s - panic, panic0, _panic, panvec0, panvec1, panvec2, panvec3, 
	     panvec4, panvec5, panvec6, panvec7, panvec8, panvec9, panvec10,
	     panvec11, panvec12, panvec13, panvec14, panvec15		*/

#include <procreg.h>
#include <conf.h>

#define	REGMASK 0x3fff			/* mask for pushr reg13-reg0	*/
#define	STKPRNT 12			/* # of stack items to print	*/
#define	PSLDISAB 0x001f0000		/* disable saved psl interrupts	*/

/*------------------------------------------------------------------------
 *  panic0 -- entry point for "branch to zero" error; never called
 *------------------------------------------------------------------------
*/
	.text
	.align 	2			/* align on longword		*/
	.globl	panic0
panic0:
					/* no entry mask -- not called	*/
	movpsl	kernstk-4		/* push psl for message print	*/
	mtpr	$DISABLE, $IPL		/* disable interrupts		*/
	movl	sp, kernstk		/* save old sp for message	*/
	movl	$kernstk-4, sp		/* use kernel stack for printing*/
	pushl	_currpid		/* print process id		*/
	pushal	panmsg1			/* message address		*/
	calls	$4, _kprintf		/* print message		*/
	jmp	__halt			/* halt cpu			*/

/*-------------------------------------------------------------------------
 *  panvec0 -- execute panic -- machine check (probably bus error)
 *	       must be taken on interrupt stack (low bit of vector == 1)
 *             number of bytes of parameters is topmost on stack
 *             interrupts are (supposed to be) disabled by hardware
 *-------------------------------------------------------------------------
*/
	.align	2			/* align on longword bdry	*/
	.globl	panvec0
panvec0:				/* (now on interrupt stack)	*/
	mtpr	$DISABLE, $IPL		/* disable, just in case	*/
					/* clear history of machine chk	*/
#if	defined(VAX780) || defined(VAX785)
		mtpr	$SBIFSCLR, $SBIFS/* clr SBI's machchk history	*/
#endif	VAX78X

#if	defined(UVAX) || defined(VAX725) || defined(VAX730) \
	|| defined(VAX750)
		mtpr	$MCESRCLR, $MCESR/* clear mach chk history reg.	*/
#endif	UVAXX || VAX7X0

#if	defined(VAX8600)
		mtpr	$0, $EHSR	/* clear error handler stat. reg*/
#endif	VAX8600

	addl2	(sp)+, sp		/* pop error parameters		*/
	pushl	r0			/* save old r0 value		*/
	mfpr	$KSP, r0		/* r0 = old kernel stk ptr 	*/
	movl	8(sp), -(r0)		/* psl from int stk on kern stk	*/
	movl	4(sp), -(r0)		/* pc from int stk on kern stk	*/
	mtpr	r0, $KSP		/* revised kernel stack pointer	*/
	movl	(sp)+, r0		/* restore old r0 value		*/
	bisl2	$PSLDISAB, 4(sp)	/* disable psl interrupts	*/
	moval	vec0cont, (sp)		/* get back on kernel stk, 	*/
	rei				/* continue at vec0cont		*/
vec0cont:				/* stack dump can now be	*/
	pushal	pmsg0 			/* meaningful			*/
	mtpr	$intstk, $ISP		/* reset interrupt stack	*/
	jbr	panic

/*------------------------------------------------------------------------
 *  panvec1 -- execute panic -- kernel stack not valid
 *             must be taken on interrupt stack (low bit of vector == 1)
 *             interrupts are (supposed to be) disabled by hardware
 *------------------------------------------------------------------------
*/
	.align	2
	.globl	panvec1
panvec1:				/* (now on interrupt stack)	*/
	mtpr	$DISABLE, $IPL		/* disable interrupts (safety)	*/
	mfpr	$KSP, -(sp)		/* push args to kprintf--old sp	*/
	pushr	$REGMASK		/* 	r13-r0			*/
	pushl	64(sp)			/* 	old psl			*/
	pushl	60(sp)			/* 	old pc			*/
	pushl	_currpid		/* 	process id		*/
	pushal	pmsg1			/* 	exception type message	*/
	pushal	panmsg2			/* 	format string		*/
	calls	$20, _kprintf		/* print reg dump		*/
	pushal	panmsg4			/* no stack dump		*/
	calls	$1, _kprintf
	jmp	__halt			/* halt cpu			*/

/*------------------------------------------------------------------------
 *  panvec2 -- execute panic -- power fail
 *------------------------------------------------------------------------
*/
	.align	2
	.globl	panvec2
panvec2:
	mtpr	$DISABLE, $IPL		/* disable interrupts	*/
	pushal	pmsg2
	jbr	panic

/*-------------------------------------------------------------------------
 *  panvec3 -- execute panic -- reserved/privileged instruction
 *-------------------------------------------------------------------------
*/
	.align	2
	.globl	panvec3
panvec3:
	mtpr	$DISABLE, $IPL		/* disable interrupts	*/
	pushal	pmsg3
	jbr	panic

/*-------------------------------------------------------------------------
 *  panvec4 -- execute panic -- virtual memory fault
 *-------------------------------------------------------------------------
*/
	.align	2
	.globl	panvec4
panvec4:
	mtpr	$DISABLE, $IPL			/* disable interrupts	*/
	addl2	$8, sp				/* pop parameters	*/
	pushal	pmsg4
	jbr	panic

/*-------------------------------------------------------------------------
 *  panvec5 -- execute panic -- trace pending
 *-------------------------------------------------------------------------
*/
	.align	2
	.globl	panvec5
panvec5:
	mtpr	$DISABLE, $IPL			/* disable interrupts	*/
	pushal	pmsg5
	jbr	panic

/*-------------------------------------------------------------------------
 *  panvec6 -- execute panic -- breakpoint fault
 *-------------------------------------------------------------------------
*/
	.align	2
	.globl	panvec6
panvec6:
	mtpr	$DISABLE, $IPL			/* disable interrupts	*/
	pushal	pmsg6
	jbr	panic

/*-------------------------------------------------------------------------
 *  panvec7 -- execute panic -- compatibility mode
 *-------------------------------------------------------------------------
*/
	.align	2
	.globl	panvec7
panvec7:
	mtpr	$DISABLE, $IPL			/* disable interrupts	*/
	tstl	(sp)+				/* pop parameter	*/
	pushal	pmsg7
	jbr	panic

/*-----------------------------------------------------------------------
 *  panvec8 -- execute panic -- arithmetic trap/fault
 *-----------------------------------------------------------------------
*/
	.align	2
	.globl	panvec8
panvec8:
	mtpr	$DISABLE, $IPL			/* disable interrupts	*/
	pushl	r0				/* save copy of r0	*/
	subl3	$1, 4(sp), r0			/* get trap type code-1	*/
	movl	arithvec[r0], 4(sp)		/* replace type code w	*/
						/* address of trap mesg	*/
	movl	(sp)+, r0			/* pop saved r0		*/
	jbr	panic

/*----------------------------------------------------------------------
 *  panvec9 -- execute panic -- unused exception vector
 *----------------------------------------------------------------------
*/
	.align	2
	.globl	panvec9
panvec9:
	mtpr	$DISABLE, $IPL			/* disable interrupts	*/
	pushal	pmsg9
	jbr	panic

/*-----------------------------------------------------------------------
 *  panvec10 -- execute panic -- corrected memory read
 *-----------------------------------------------------------------------
*/
	.align	2
	.globl	panvec10
panvec10:
	mtpr	$DISABLE, $IPL			/* disable interrupts	*/
	pushal	pmsg10
	jbr	panic

/*------------------------------------------------------------------------
 *  panvec11 -- execute panic -- write bus timeout
 *------------------------------------------------------------------------
*/
	.align	2
	.globl	panvec11
panvec11:
	mtpr	$DISABLE, $IPL			/* disable interrupts	*/
	pushal	pmsg11
	jbr	panic

/*------------------------------------------------------------------------
 *  panvec12 -- execute panic -- unknown device
 *------------------------------------------------------------------------
*/
	.align	2
	.globl	panvec12
panvec12:
	mtpr	$DISABLE, $IPL			/* disable interrupts	*/
	pushal	pmsg12
	jbr	panic

/*------------------------------------------------------------------------
 *  panvec13 -- execute panic -- change mode instruction
 *		must be executed on kernel stack (vector bits 1:0 == 0)
 *------------------------------------------------------------------------
*/
	.align	2
	.globl	panvec13
panvec13:
	mtpr	$DISABLE, $IPL			/* disable interrupts	*/
	tstl	(sp)+				/* pop parameter	*/
	pushal	pmsg13
	jbr	panic

/*------------------------------------------------------------------------
 *  panvec14 -- execute panic -- reserved or illegal operand
 *------------------------------------------------------------------------
 */
	.align	2
	.globl	panvec14
panvec14:
	mtpr	$DISABLE, $IPL			/* disable interrupts	*/
	pushal	pmsg14
	jbr	panic

/*------------------------------------------------------------------------
 *  panvec15 -- execute panic -- reserved or illegal addressing mode
 *------------------------------------------------------------------------
 */
	.align	2
	.globl	panvec15
panvec15:
	mtpr	$DISABLE, $IPL			/* disable interrupts	*/
	pushal	pmsg15
	jbr	panic

/*------------------------------------------------------------------------
 *  _panic  --  entry point when called by user programs
 *------------------------------------------------------------------------
*/
	.align	1
	.globl	_panic
_panic:					/* Note: "_panic" pushes the	*/
					/* PC and PS, since it jumps to	*/
					/* "panic" (the trap entry	*/
					/* point, below)		*/
	.word	0x0			/* save no registers		*/
	movpsl	-(sp)			/* old PS for trap simulation	*/
	mtpr	$DISABLE, $IPL		/* disable interrupts		*/
	pushl	16(fp)			/* push ret. address for trap	*/
	pushl	4(ap)			/* address of users message	*/
	jbr	panic

/*----------------------------------------------------------------------------
 *  panic  -- entry point for traps and exceptions only; not called
 *	      contents of stack on call:
 *	      (sp):message address, (sp+4):old pc, (sp+8):old ps
 *----------------------------------------------------------------------------
*/
	.align	1
panic:
	movl	(sp)+, pansav		/* save message address		*/
	movl	4(sp), kernstk		/* push on kernel stack: old psl*/
	movl	(sp), kernstk-4		/*	old pc			*/
	addl3	$8, sp, kernstk-8	/*	old sp(from before trap)*/
	movl	$kernstk-8, sp		/* switch stack to kernstk	*/
	pushr	$REGMASK		/* save registers r13-r0	*/
	calls	$0, sizmem		/* size up memory (needed later)*/

	/* set up call to kprintf to print register and stack dump */
	pushl	kernstk			/* user's psl			*/
	pushl	kernstk-4		/* user's pc			*/
	pushl	_currpid		/* process id			*/
	pushl	pansav			/* message address		*/
	pushal	panmsg2			/* format address		*/
	calls	$5, _kprintf		/* print message, then pop until*/
					/* saved regs on stack top	*/
	/* dump stack as long as sp was valid */
	movl	kernstk-8, r3		/* r3=user's stk before trap	*/
	tstl	-(sp)			/* space for arg to kprintf	*/
	moval	panmsg3, -(sp)		/* format arg for kprintf	*/
	movl	$STKPRNT, r4		/* count of stack items to print*/
	movl	_maxaddr, r5		/* r5 = copy of _maxaddr	*/
panloop:
	cmpl	r3, r5			/* avoid references beyond	*/
	jgtru	pandone			/*  valid memory addresses	*/
	movl	(r3)+, 4(sp)		/* insert value to print as arg	*/
	calls	$0, _kprintf		/* print; pop no values		*/
	sobgtr	r4, panloop		/* decr count, print more if >0	*/
pandone:
	addl2	$8, sp			/* pop off kprintf args		*/
	popr	$REGMASK		/* pop off registers saved	*/
	subl3	$8, (sp), sp		/* restore user's sp, with psl	*/
					/* and pc from trap on top	*/
	jmp	__halt			/* halt processor		*/
					/* user probably won't continue	*/
					/* pc & psl on top in case	*/
					/* of console "C" command	*/
	rei				/* especially for call to _panic*/
	.data	1
pansav:
	.space	4			/* save location for messg addr	*/
panmsg1:
	.ascii	"\n\nPanic: branch to location 0:\n"
	.ascii	"\tpid = (dec) %d\n"
	.ascii	"\tpsl = (hex) %0x\n"
	.ascii	"\tsp  = (hex) %0x\n\n\0"
panmsg2:
	.ascii	"\n\nPanic: trap type: %s\n"
	.ascii	"pid = %d\n"
	.ascii	"State: pc = %x psl = %08x\n"
	.ascii	"r0  = %8x r1  = %8x r2  = %8x r3  = %8x\n"
	.ascii	"r4  = %8x r5  = %8x r6  = %8x r7  = %8x\n"
	.ascii	"r8  = %8x r9  = %8x r10 = %8x r11 = %8x\n"
	.ascii	"r12 = %8x r13 = %8x\n"
	.ascii	"Stack: (top at %x)\n\0" 
panmsg3:
	.ascii	"%8x\n\0"
panmsg4:
	.ascii	"(can't dump stack)\n\0"
pmsg0:
	.ascii	"bus error or machine check\0"
pmsg1:
	.ascii	"invalid kernel stack pointer\0"
pmsg2:
	.ascii	"power fail\0"
pmsg3:
	.ascii	"reserved or privileged instruction\0"
pmsg4:
	.ascii	"virtual address fault\0" 
pmsg5:
	.ascii	"trace pending\0"
pmsg6:
	.ascii	"breakpoint instruction\0"
pmsg7:
	.ascii	"PDP-11 compatibility mode trap\0"
pmsg9:
	.ascii	"unused exception vector\0"
pmsg10:
	.ascii	"corrected memory read\0"
pmsg11:
	.ascii	"write bus timeout\0"
pmsg12:
	.ascii	"unknown device\0"
pmsg13:
	.ascii	"CHMx instruction\0"
pmsg14:
	.ascii	"reserved or illegal operand\0"
pmsg15:
	.ascii	"reserved or illegal addressing mode\0"
arith1:
	.ascii	"integer overflow\0"
arith2:
	.ascii	"integer divide by 0\0"
arith3:
	.ascii	"floating overflow\0"
arith4:
	.ascii	"floating/decimal divide by 0\0"
arith5:
	.ascii	"floating underflow\0"
arith6:
	.ascii	"decimal overflow\0"
arith7:
	.ascii	"subscript range\0"
arith8:
	.ascii	"floating overflow fault\0"
arith9:
	.ascii	"floating divide by 0 fault\0"
arith10:
	.ascii	"floating underflow fault\0"
arithvec:
	.long	arith1
	.long	arith2
	.long	arith3
	.long	arith4
	.long	arith5
	.long	arith6
	.long	arith7
	.long	arith8
	.long	arith9
	.long	arith10

/* Stack used by kernel at startup and to handle panic messages.	*/

	.data	1			/* kernstk & intstk are in 	*/
					/* initialized data segment	*/
	.align	2			/* align on fullword boundary	*/
	.globl	kernstk
	.space	512			/* decimal bytes in kernel stack*/
kernstk:

/* Intstk is used as interrupt stack (temporarily) by processor.	*/
/* It is used during startup and temporarily during context switch,	*/
/* also when processing certain exceptions.				*/
	.align	2
	.space	128
	.globl	intstk
intstk:
	.space	4			/* no overlap of next symbol	*/
