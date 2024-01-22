/* halt.s -- halt */

#include <procreg.h>
#include <conf.h>
#include <qbus.h>
#include <mach.h>

/* declare global for use after uploading, with pm */
	.data
	.comm	_pslsave, 4
	.comm	_spsave, 4

/*------------------------------------------------------------------------
 *  halt -- simulate halt on VAX (really halt on UVAXII)
 *------------------------------------------------------------------------
 */
	.text
	.globl	__halt
__halt:
				/* no entry mask--not called		*/
	movpsl	_pslsave	/* save old psl for pm after upload	*/
	mtpr	$DISABLE, $IPL	/* disable interrupts			*/

	mfpr	$SID, r1	/* get system id register value		*/
	bicl2	$SYSTYPE, r1	/* mask off system id			*/
	cmpl	r1, $UVAXIISID	/* compare with UVAX II sid reg. value	*/
	jneq	uvaxi
	movw	$QMHALT,*$QMCPMBX/* raise console program mailbox flag	*/
	halt			/* can halt if UVAXII			*/
	jmp	savepc

uvaxi:	movl	sp, _spsave
	movl	$kernstk, sp	/* give stack memory for kprintf call	*/
	pushab	haltmesg
	calls	$1, _kprintf
	movl	_spsave, sp	/* restore stack pointer		*/
forever:
	jbr	forever		/* simulate halt by tight loop -- 	*/
				/* if halted, the cpu may try to reboot	*/
savepc:	movl	_pslsave, -(sp)	/* in case user moves PC past loop and	*/
	movab	unhalt, -(sp)	/* issues console "C"			*/
	rei			/* load old psl				*/
unhalt:
	ret

haltmesg:
	.ascii	"\nCpu executing tight loop, may be halted at console\n\0"
