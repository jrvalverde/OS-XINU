/* kern_genericint.s - generic interrupt dispatcher  */

#define DEBUG

#include <asl.h>
#include <psl.h>
#include <vmem.h>

!---------------------------------------------------------------------------
! kern_genericint() - generic interrupt handler - no need to switch stacks
!---------------------------------------------------------------------------
! Upon Entry
!   %l0 = %psr (%psr value just AFTER the trap occurred)
!   %l1 = return pc
!   %l2 = return npc
!   %l3 = possible stack pointer from window overflow/underflow
! We then set
!   set %l4 <- 2^cwp at time of trap
!---------------------------------------------------------------------------
	.seg	"text"
	.align	4
	PROCEDURE(kern_genericint)

#ifdef DEBUG
	!MYPUTC(LTR_K,%l5,%l6)
	!MYPUTC(LTR_G,%l5,%l6)
	!MYPUTC(LTR_T,%l5,%l6)
	!MYPUTC(LTR_SP,%l5,%l6)
#endif
	!
	! set %l3 and %l4
	!
	and	%l0, PSR_CWP, %l3
	mov	1, %l4
	sll	%l4, %l3, %l4

	!
	! save globals onto current stack
	!
	sub	%fp, GLOBAL_SAVE_SIZE, %l5
	SAVE_G_REGS(%l5)

	!
	! test for overflow ( (2^cwp && wim) != 0 )
	!
	mov	%wim, %g2			! %g2 <- wim
	andcc	%l4, %g2, %g0
	be	_nooverflow
	nop

	!
	! overflow detected, save next window (rotate wim) (wim saved in %g1)
	!
	save	%g0, %g0, %g0

	SAVE_LOCALS(%sp)
	ROTATE_WIM_RIGHT( %l4, %l5 )
	SAVE_INS(%sp)

	restore

_nooverflow:
	!
	! turn off interrupts
	!
	or 	%l0, PSR_PIL, %g1
	mov 	%g1, %psr

	!
	! Increment the %sp past the globals that we saved on the stack
	!
	sub	%fp, GLOBAL_SAVE_SIZE, %sp

	!
	! Reserve room for and exception frame
	!   - also, store exception frame ptr in %o1 to be passed to int_demux
	!
	sub	%sp, SPARCMINFRAME, %o1

	!
	! Reserve room for the current window
	!
	sub	%o1, SPARCMINFRAME, %sp

	!
	! enable traps
	!
	or 	%l0, PSR_PIL | PSR_ET, %g1
	mov 	%g1, %psr

	!
	! read the vector number (tic number)
	!
	mov 	%tbr, %o0
	and	%o0, TBR_TIC_MASK, %o0
	srl	%o0, TBR_TIC_SHIFT, %o0

	!
	! Write the exception info into the space reserved above
	!
	st 	%o0, [ %o1 + (0*4) ]		! vector number
	st	%l2, [ %o1 + (2*4) ]		! npc
#ifdef SOME_TRAP_IN_THE_FUTURE_MIGHT_NEED_ALL_THIS_INFO
	st 	%l0, [ %o1 + (3*4) ]		! psr
	and	%l0, PSR_CWP, %g1		! calculate the cwp
	st 	%g1, [ %o1 + (4*4) ]		! cwp
	st 	%g2, [ %o1 + (5*4) ]		! wim
	st 	%fp, [ %o1 + (6*4) ]		! sp_prev_win = fp
#endif

	!
	! Call int_demux(vector, exception_frame_ptr)
	!	- at this point
	!		%o0 = vector
	!		%o1 = exception_frame_ptr
	!
	call 	_int_demux
	st	%l1, [ %o1 + (1*4) ]		! pc


	!
	! Disable Traps
	! 	Interrupts better be disabled, and traps enabled
	!
	mov	%psr, %g1
	andn	%g1, PSR_ET, %g1
	mov	%g1, %psr
	nop; nop; nop

	!
	! If cwp != old cwp then flush windows and reset cwp ( %g1 == psr )
	!
	and	%g1, PSR_CWP, %g1
	cmp	%l3, %g1
	be	_testunderflow
	nop

	!
	! cwp != old cwp (%g1 == cwp)
	!
	mov	1, %g2
	sll	%g2, %g1, %g1			! %g1 <- 2^cwp
	mov	%wim, %g2			! %g2 <- %wim
	mov	%sp, %g3			! %g3 <- %sp
	mov	%l0, %g4			! %g4 <- old %psr
	mov	%g0, %wim

_savewindow:
	SAVE_WINDOW(%sp)
	ROTATE_WIM_VALUE_ONE_LEFT(%g1,%g5)
	andcc	%g1, %g2, %g0
	bne	_savedallwindows
	nop
	ba	_savewindow
	restore

_savedallwindows:

	!
	! restore cwp, condition flags
	!
	mov	%g4, %psr
	nop; nop; nop

	!
	! restore locals
	!
	mov	%g3, %sp
	RESTORE_WINDOW(%sp)

	!
	! set wim, restore previous window
	!
	ROTATE_WIM_VALUE_ONE_LEFT(%l4, %g1)
	ROTATE_WIM_VALUE_ONE_LEFT(%l4, %g1)
	mov	%l4, %wim
	nop; nop; nop
	restore
	RESTORE_WINDOW(%sp)
	save	%g0, %g0, %g0

	!
	! restore globals
	!
	sub	%fp, GLOBAL_SAVE_SIZE, %l5
	RESTORE_G_REGS(%l5)

	!
	! re-execute instruction (must reset condition codes in psr)
	!
	jmp 	%l1
	rett	%l2
	nop

_testunderflow:
	!
	! test for underflow ( (ROTATE_LEFT(2^cwp) && wim ) != 0 )
	! cwp == original cwp
	!
	mov	%l4, %g1
	ROTATE_WIM_VALUE_ONE_LEFT(%g1,%g2)	! %g1 <- ROTATE_LEFT( 2^cwp )
	mov	%wim, %g2			! %g2 <- wim
	andcc	%g2, %g1, %g0
	be	_nounderflow
	nop

	!
	! underflow detected (%g2 saved wim)
	!
	ROTATE_WIM_VALUE_ONE_LEFT(%g2,%g1)
	mov	%g2, %wim
	nop; nop; nop
	restore
	RESTORE_WINDOW(%sp)
	save	%g0, %g0, %g0

_nounderflow:
	!
	! restore globals
	!
	sub	%fp, GLOBAL_SAVE_SIZE, %l5
	RESTORE_G_REGS(%l5)

	!
	! restore interrupts, PS bit and condition flags
	!
	mov	%l0, %psr
	nop; nop; nop

	!
	! re-execute instruction (must reset condition codes in psr)
	!
	jmp 	%l1
	rett 	%l2
	nop


