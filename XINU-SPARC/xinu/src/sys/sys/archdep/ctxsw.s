/* ctxsw.s - context switch routine */

#include <asl.h>

	! 
	! save_context(optr->pregs) - save the current context
	!
	.seg	"text"
	.proc 04
	PROCEDURE(save_context)

	save	%sp, -SPARCMINFRAME, %sp

	! %i0 has address of where to store the registers
 	st	%g1, [%i0 + (0*4)]	! store %g1 - %g7
	st	%g2, [%i0 + (1*4)]
	st	%g3, [%i0 + (2*4)]
	st	%g4, [%i0 + (3*4)]
	st	%g5, [%i0 + (4*4)]
	st	%g6, [%i0 + (5*4)]
	st	%g7, [%i0 + (6*4)]

	st	%sp, [%i0 + (7*4)]	! store current stack pointer
	st	%fp, [%i0 + (8*4)]	! store current frame pointer
	st	%i7, [%i0 + (9*4)]	! store return address
	mov	%psr, %i1
	st	%i1, [%i0 + (10*4)]	! store psr 
	mov	%y, %i1
	st	%i1, [%i0 + (11*4)]	! store y (multiply) register

	! We don't save the PSR, because the only thing that can change
	! (of any value) is the CWP which we don't want to restore anyway.
	! The only thing that could be interest is the PIL.  But in Xinu
	! we always disable interrupts (i.e. set PIL=15) before calling
	! save_context()

	! Save the floating point registers
	! - assume no one uses floating point - at least not yet

	! We used to push all the reg windows onto the stk in restore_context.
	! Because if we saved them here, the WIM would be off-by-one
	! in restore_context.  By saving them in restore_context, the
	! the WIM ended up being right next to the CWP, so we would take
	! a underflow trap upon return from restore_context (which is
	! what we wanted).
	!
	! HOWEVER: This was stupid and didn't work well for VM Xinu. In
	! particular, it did not allow us to change the page tables mappings
	! in between calling save_context and restore_context.  So, we
	! now do the saving right here where it belongs.  It does leave the
	! WIM in the wrong place when we get into restore_context (i.e., when
	! we get to restore_context the WIM is 2 away from the CWP). We
	! just let restore_context fix up the WIM that way it wants it (i.e.,
	! right next to CWP so an underflow will be triggered).

	/*
	 * Flush all (only need nwindows-2) register windows.
	 */

	! do nwindows -2  saves

	set _nwindows_minus_1, %g2
	ld  [%g2], %g2

	mov %g2, %g1
7:	subcc %g1, 1, %g1
	be 8f
	nop
	ba 7b
	save

8:
	! do nwindows - 2 restores

	mov %g2, %g1
9:	subcc %g1, 1, %g1
	be 6f
	nop
	ba 9b
	restore

6:
	mov	%g0, %i0		! return(0)
 	ret
	restore
 
	! 
	! restore_context(nptr->pregs) - switch to a new context
	!				 We assume all register windows
	!				 have been dumped (and therefore
	!				 contain useless values)
	!
	.seg	"text"
	.proc 04
	PROCEDURE(restore_context)

	save
	
	! %i0 has address where the registers are stored
	ld	[%i0 + (0*4)], %g1	! load %g1 - %g7
	ld	[%i0 + (1*4)], %g2
	ld	[%i0 + (2*4)], %g3
	ld	[%i0 + (3*4)], %g4
	ld	[%i0 + (4*4)], %g5
	ld	[%i0 + (5*4)], %g6
	ld	[%i0 + (6*4)], %g7

	ld	[%i0 + (7*4)], %sp	! load old stack pointer
	ld	[%i0 + (8*4)], %fp	! load old frame pointer
	ld	[%i0 + (9*4)], %i7	! load return address
	! FIXME: The code to reset the PSR should go here.  In particular
	! we should merge the current PSR with the save PIL, EC, and EF fields.
	! However, since those fields never change in this implementation
	! (i.e., interrupts are always disable while context switching and
	! we do not currently use the CP or FP units), we do not have to
	! update the PSR.  Otherwise we would do something like:
	!
	!	ld	[%i0 + (10*4)], %i1	! restore PIL,EC,EF bits of PSR
	!	mov	%psr, %i2		! get cur PSR
	!	andn	%i1, PSR_CWP, %i1	! mask off old CWP
	!	and	%i2, PSR_CWP, %i2	! mask off all but cur CWP
	!	or	%i1, %i2, %i1		! combine them
	!	mov	%i1, %psr		! reset the PSR
	!	nop; nop; nop			! wait for it to take effect
	!
	ld	[%i0 + (11*4)], %i1	 ! load y (multiply) register
	mov	%i1, %y

	/*
	 * Dink with the WIM so that an underflow will occur when returning
	 * from this routine.
	 */
	mov	%psr, %l5
	set	_nwindows, %l4
	ld	[%l4], %l3		! %l3 <- nwindows
	and	%l5, CWP_MASK, %l5
	add	%l5, 0x1, %l5		! %l5 <- CWP + 1
	cmp	%l3, %l5
	bne	1f			! if ((CWP+1) == nwindows)
	nop
	mov	%g0, %l5		!      %l5 = 0
1:					! endif
	mov	0x1, %l4		
	sll	%l4, %l5, %l5		! %l5 <- 0x1 << %l5
	mov	%l5, %wim		! wim <- %l5
	nop; nop; nop			! make sure it takes effect

	set	1, %i0			! return(1) - 	Note: we will be
					!		returning to the
					!		save_context call.

	ret
	restore



