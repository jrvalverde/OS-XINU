/* regwin.s - register window routines */

#include <asl.h>
#include <reg.h>


/*---------------------------------------------------------------------------
 * global window data
 *---------------------------------------------------------------------------
 */
	.seg	"data"
	.align	4

	.global	_nwindows
_nwindows:
	.word	0		

	.global	_nwindows_minus_1
_nwindows_minus_1:
	.word	0		

/*---------------------------------------------------------------------------
 * set_nwindows - set global nwindows to the # reg windows for this machine
 *	-- Works with any # of windows and is independent of where we called
 *	   it from
 *---------------------------------------------------------------------------
 */
	.seg	"text"
	.proc	4
	PROCEDURE(set_nwindows)

	/*
	 * We are in the caller's window so use only o registers
	 */
	mov	%wim, %o5		/* save %wim */
	set	0xffffffff, %o1
	mov	%o1, %wim
	nop; nop; nop


	/* count the 1's in %wim */
	! mask off the cwp from wim
	mov	%wim, %o0
	mov	%g0, %o1

1:	cmp	%o0, %g0
	be	2f
	nop
	srl	%o0, 1, %o0
	ba	1b
	add	%o1, 1, %o1


2:	set	_nwindows, %o0
	st	%o1, [%o0]

	set	_nwindows_minus_1, %o0
	sub	%o1, 1, %o1
	st	%o1, [%o0]

	mov	%o5, %wim	/* restore %wim */
	nop; nop; nop

	retl
	nop
