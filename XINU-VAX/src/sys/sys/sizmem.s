/* sizmem.s - sizmem */

#include <procreg.h>
#include <conf.h>
#include <mach.h>

/*----------------------------------------------------------------------------
 *  sizmem  -- size memory, place highest valid address of an int in _maxaddr
 *		assumption: executing on kernel stack, interrupts disabled
 *----------------------------------------------------------------------------
*/

#define	HIGHADDR 0x20000000		/* highest address of int on VAX*/
					/* (below device csr addresses)	*/
#define	CONSDB	0x2000			/* sizeof UVAXII console database */
	.text
	.align	1
	.globl	sizmem
sizmem:
	.word	0x4			/* save register 2 		*/
	tstl	-(sp)			/* allocate local for disable	*/
	pushal	-4(fp)			/* address of local		*/
	calls	$1, _disable		/* disable interrupts		*/
	movl	lowcore+4, -(sp)	/* push old exception vector	*/
	bisl3	$1, $siztrap, lowcore+4	/* set exception vector to	*/
					/* catch memory exceptions	*/
					/* (with memory mapping 	*/
					/* disabled, the VAX takes a	*/
					/* machine check exception)	*/
					/* NOTE: must handle mach. chk.	*/
					/* on interrupt stack		*/
					/* (hence bits<1,0> == 01)	*/
	calls	$0, _ovfloff		/* disable int oflow exceptions	*/
	clrl	r0			/* r0 holds low address		*/
	movl	$HIGHADDR, r1		/* r1 = highest addr of an int	*/
sizloop:
	cmpl	r0, r1			/* see if low > high		*/
	jgtru	setsize			/* yes: restore & return	*/
	addl3	r0, r1, r2		/* calculate midpoint		*/
	jcc	rotate			/* if carry set, set low bit,	*/
	bisl2	$1, r2			/* soon to be high bit		*/
rotate:
	rotl	$-1, r2, r2		/* div by 2 by shifting right 1	*/
	bicl2	$3, r2			/* address is multiple of 4	*/
	tstl	(r2)+			/* reference what r2 points to	*/
					/* note: if exception taken, r2	*/
					/* is not incremented		*/ 
	movl	r2, r0			/* mid caused no exception -- 	*/
					/* set low to mid address + 4	*/
	jbr	sizloop
setsize:
	mfpr 	$SID, r3		/* get system id register value	*/
	bicl2	$SYSTYPE, r3		/* mask off system id		*/
	cmpl	r3, $UVAXIISID		/* compare with UVAXII sid value*/
	jneq	uimax			/* if MicroVAX II, reserve 8K	*/
	subl3	$CONSDB, r1, _maxaddr	/* high mem for console database*/
	jmp	rest
uimax:	movl	r1, _maxaddr		/* store result			*/
rest:	movl	(sp)+, lowcore+4	/* restore exception vector	*/
	pushal	-4(fp)			/* argument to restore		*/
	calls	$1, _restore		/* restore interrupts		*/
	ret				/* return to caller		*/
	.align	2			/* align on longword boundary	*/
siztrap:
					/* machine chk--on intrrpt stk	*/
	subl3	$4, r2, r1		/* midpt caused exception, set	*/
					/* high address to mid - 4	*/
	addl2	(sp)+, sp		/* pop error data from exception*/
					/* erase machine check history:	*/
#if defined(VAX780) || defined(VAX785)
	mtpr	$SBIFSCLR, $SBIFS	/* clear SBI history of machchk	*/
#endif	VAX78X
#if	defined(UVAX) || defined(VAX750) || \
	defined(VAX730) || defined(VAX725)
	mtpr	$MCESRCLR, $MCESR	/* clear machine check history	*/
#endif	UVAXX || VAX750 || VAX730 || VAX725
#if	defined(VAX8600)
	mtpr	$0, $EHSR
#endif	VAX8600
	moval	sizloop, (sp)		/* go back to sizloop on rei	*/
	rei				/* back on kernel stack	again	*/
