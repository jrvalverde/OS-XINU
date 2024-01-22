/* startup.s - start */

#include <procreg.h>
#include <conf.h>
#include <kernel.h>
#include <mach.h>

#define	PSLDISAB	0x001f0000	/* initial PSL -- interrupts	*/
					/* disabled, on kernel stack,	*/
					/* kernel mode			*/

/*----------------------------------------------------------------------------
 *  Xinu system entry point -- first location beyond interrupt vectors
 *  assumption: processor is in kernel mode on entry 
 *----------------------------------------------------------------------------
*/
	.text
	.globl	start
	.globl	_restart
start:					/* boot utility entry pt	*/
_restart:				/* shell reboot cmd entry pt	*/
	nop; nop			/* == .word 0x0101; can jmp to	*/
					/* start or calls $0, start	*/
	mtpr	$DISABLE, $IPL		/* disable interrupts 		*/
#if defined(VAX750) || defined(VAX730) || defined(VAX725) || \
    defined(UVAX)
	mtpr	$IURSET, $IUR		/* reset the bus		*/
	mtpr	$IURUNSET, $IUR		/* unreset the bus		*/
#endif
	mtpr	$intstk, $ISP		/* interrupt stack given memory	*/
	mtpr	$NOMAP, $MAPEN		/* disable memory management	*/
	movab	_end, r2		/* zero out bss	(just in case)	*/
 	movab	_edata, r1
clrbss:
	clrb	(r1)+
	cmpl	r1, r2			/* more bss to zero?		*/
	jlssu	clrbss			/* yes => back to clrbss	*/
	mtpr	$lowcore, $SCBB		/* sys ctrl blck pts to vectors	*/
	mtpr	_nullpcb, $PCBB		/* PCBB points to null process'	*/
					/* process control block entry	*/
					/* in proctab[]			*/
					/* set up nulluser's proc ctrl	*/
	movl	_nullpcb, r1 		/* block--init all general reg	*/
	addl3	$72, r1, r0		/* clear fp first, then r12-r0	*/
pcbloop:				/* then 4 stack pointers	*/
	clrl	-(r0)
	cmpl	r0, r1
	jgtru	pcbloop
	moval	kernstk, (r0)		/* stack pointer given memory--	*/
					/* kernstk used for now		*/
	movab	pcbcont, 72(r0)		/* PC points to pcbcont		*/
	movl	$PSLDISAB, 76(r0)	/* initial psl, interrupts 	*/
					/* disabled, on kernel stack	*/
	movl	$INITBR, 80(r0)		/* set P0BR			*/
	movl	$INITLR, 84(r0)		/* AST lvl set to none pending,	*/
					/* P0LR set to 0		*/
	movl	$INITBR, 88(r0)		/* set P1BR			*/
	clrl	92(r0)			/* set P1LR			*/
	ldpctx				/* load new context		*/
	rei				/* resume at pcbcont		*/
pcbcont:				/* set all pertinent internal	*/
					/* processor registers		*/
					/*   (it may not be necessary	*/
					/*    to set all of these)	*/
					/* (r0 is now 0)		*/
	mtpr	r0, $SISR		/* clear software intrpt summary*/
	mtpr	r0, $ICCS		/* clear interval clock csr	*/
	mtpr	r0, $RXCS		/* clr cons recv intrrpt enable	*/
	mfpr	$SID, r1		/* get system id register value	*/
	bicl2	$SYSTYPE, r1		/* mask off system id		*/
	movl 	r1, _machinesid		/* move machine system id 	*/
	mfpr	$RXDB, r1		/* clear incoming byte if any	*/
	mtpr	r0, $TXCS		/* clear cons trans int enable	*/
#if	defined(VAX750) || defined(VAX730) || defined(VAX725) || \
	defined(UVAX)
	mtpr	$3, $TBDR		/* disable translation buffer	*/
	mtpr	$MCESRCLR, $MCESR	/* clear machine check error reg*/
	mtpr	r0, $CAER		/* clear cache error register	*/
	mtpr	r0, $CADR		/* enable cache 		*/
#endif
#if	defined(VAX780) || defined(VAX785)
	mtpr	r0, $SBIFS		/* clear SBI fault and status	*/
	mtpr	r0, $SBIER		/* clear SBI error register	*/
#endif
#if defined(VAX8600)
	mtpr	$0, $EHSR		/* clear error handling status	*/
#endif
	mtpr	r0, $TBIA		/* clear trans buffer inval all	*/
	calls	$0, sizmem		/* _maxaddr set to max address	*/
	movl	_maxaddr, sp		/* switch stack to high memory	*/
	calls	$0, _nulluser		/* Run C startup routine	*/
					/* nulluser should never return	*/
	pushab	errmes			/* trap return to startup	*/
	calls	$1, _kprintf
	jmp	__halt

	.data	1
errmes:
	.ascii	"error--nulluser returned to startup\n\0"
