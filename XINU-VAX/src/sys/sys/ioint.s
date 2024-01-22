/* ioint.s */

/* I/O interrupts trap here.  Original PC and PS and saved values	*/
/* of low registers are on top of the stack upon entry.			*/
/* Devices actually vector to locations defined	in dispconf.h.  	*/
/* Each vector-to code segment in dispconf.h stores the device		*/
/* descriptor value in r1.						*/

	.text
	.align	1

#include <procreg.h>
#include <conf.h>
#include <dispconf.h>			/* ASSEMBLER CODE INCLUDED HERE	*/

ioint:
	pushr	$LOWREGS
	subl3	$_devveci0,NLOWREG*4(sp),r0/* find where interrupt vector*/
	bicl2	$7, r0			/* is and call corresponding	*/
	addl2	$_intmap, r0		/* entry in intmap		*/
	pushl	4(r0)			/* push "code" from intmap	*/
	calls	$1, *(r0)		/* call the interrupt routine	*/
	popr	$LOWREGS		/* restore low registers	*/
	tstl	(sp)+			/* pop pc from bsbw		*/
	rei				/* Return from interrupt	*/
