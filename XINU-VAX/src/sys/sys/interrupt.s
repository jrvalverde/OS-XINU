/* interrupt.s - disable, restore, enable */

#include <procreg.h>

/*----------------------------------------------------------------------------
 * disable -- disable interrupts and save current IPL at argument address
 *----------------------------------------------------------------------------
 *	On entry, the stack contains (at AP+4) the address of a longword at
 *	which disable should save the current IPL.
 *	To call:  PStype ps;  disable(ps);
*/
	.text				/* text in subsegment 0		*/
	.align 	1			/* clr low bit of loc counter	*/
	.globl	_disable		/* external name		*/
_disable:				/* entry point for disable	*/
	.word	0x0			/* entry mask,save no registers	*/
	mfpr	$IPL, *4(ap)		/* store old IPL		*/
	mtpr	$DISABLE, $IPL		/* disable interrupts		*/
	ret

/*----------------------------------------------------------------------------
 * restore -- restore interrupts to argument IPL
 *----------------------------------------------------------------------------
 *	On entry, the stack contains (at AP+4) the address of a longword
 *	containing the new IPL level.
 *	To call:  PStype ps;  restore(ps);
*/
	.text
	.align	1
	.globl	_restore		/* global procedure name	*/
_restore:
	.word	0x0			/* entry mask,no regs saved	*/
	mtpr	*4(ap), $IPL		/* restore IPL			*/
	ret

/*----------------------------------------------------------------------------
 * enable -- enable interrupts
 *----------------------------------------------------------------------------
 *	To call:  enable();
*/
	.text
	.align	1
	.globl	_enable			/* global procedure name	*/
_enable:
	.word	0x0			/* entry mask,no regs saved	*/
	mtpr	$ENABLE, $IPL		/* enable interrupts		*/
	ret
