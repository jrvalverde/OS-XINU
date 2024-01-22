/* ctxsw.s - ctxsw */

#include <procreg.h>

/*------------------------------------------------------------------------
 * ctxsw  --  actually perform context switch, saving/loading registers
 *------------------------------------------------------------------------
 *  Upon entry to this routine, the current stack is assumed to be the kernel
 *  stack.  The stack contains:
 * 
 * 	AP+4 =>	address of save area with new registers, etc.
 *		Size of save area is PNREGS * sizeof (longword).
 *
 *  Assumes interrupts are disabled.
 *  See proc.h for a description of the saved state (process control block).
*/
	.text
	.align 	1		/* location counter lowest bit is 0	*/
	.globl	_ctxsw		/* declare the routine name global	*/
_ctxsw:				/* entry point to context switch	*/
	.word	0x0		/* entry mask -- save no registers	*/
	movpsl	-(sp)		/* push psl				*/
	pushal	ctxswend	/* push address at which old process	*/
				/* should resume when re-started	*/
	svpctx			/* save old process' context in proctab	*/ 
				/* The values to save for PC & PSL are	*/
				/* popped off the kernel stack.		*/
	mtpr	4(ap), $PCBB	/* move address of new process' save	*/
				/* area to Process Control Block Base	*/
	ldpctx			/* load new process' context, push PSL	*/
				/* & PC	on kernel stack.		*/
	rei			/* start executing new process		*/
ctxswend:
	/* the OLD process returns here when resumed. */
	ret
