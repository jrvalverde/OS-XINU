/* user_genericint.s - generic interrupt dispatcher (like ioint.s in LSI version) */

/*#define DEBUG*/

#include <asl.h>
#include <psl.h>
#include <vmem.h>

/* The start of all kernel stacks (see vmaddrsp.kernstk in vminit.c) */
#define KSTK_START_ADDR  (VM_KERNEL_OFFSET-VM_SP_SIZE-VM_RSA_SIZE-STACK_ALIGN)

!---------------------------------------------------------------------------
! user_genericint() - generic interrupt handler
!---------------------------------------------------------------------------
! Upon Entry
!   %l0 = %psr (%psr value just AFTER the trap occurred)
!   %l1 = return pc
!   %l2 = return npc
!   %l3 = possible stack pointer from window overflow/underflow
! We then set
!   set %l4 <- 2^cwp at time of trap
!   set %l5 <- base of kernel stack
!   set %l6 <- tic number
!---------------------------------------------------------------------------
	.seg	"text"
	.align	4
	PROCEDURE(user_genericint)

#ifdef DEBUG
	KPUTC(LTR_G,%l5,%l6)
	KPUTC(LTR_T,%l5,%l6)
	KPUTC(LTR_SP,%l5,%l6)
#endif

	!
	! Set %l5 to the beginning of the kernel stack minus the space
	! needed to save all of the registers from all of the windows
	!
	set	(KSTK_START_ADDR-(MAX_NUM_WINDOWS*WINDOW_SAVE_SIZE)), %l5

	!
	! Store 2^cwp in %l4
	!
	and	%l0, PSR_CWP, %l7
	mov	1, %l4
	sll	%l4, %l7, %l4

	!
	! Store the trap number in %l6
	!
	mov 	%tbr, %l6
	and	%l6, TBR_TIC_MASK, %l6
	srl	%l6, TBR_TIC_SHIFT, %l6

	!
	! save globals
	!
	sub	%l5, GLOBAL_SAVE_SIZE, %l7
	SAVE_G_REGS(%l7)

	!
	! Store important info in globals while saving the reg window contents
	!
	mov	%l4, %g1			! %g1 <- 2^cwp
	mov	%wim, %g2			! %g2 <- %wim
	mov	%l5, %g3			! %g3 <- save area
	mov	%l0, %g4			! %g4 <- %psr
	mov	%g0, %wim

	!
	! Save the WIM to the stack for later use
	!
	st	%g2, [ %g3 ]

	!
	! Save all active local windows, including trap window
	!
	add	%g3, 8, %g3
_save_user_window:
	SAVE_WINDOW(%g3)
	add	%g3, WINDOW_SAVE_SIZE, %g3
	ROTATE_WIM_VALUE_ONE_LEFT(%g1,%g5)
	andcc	%g1, %g2, %g0
	bne	_done_saving_user_windows
	nop
	ba	_save_user_window
	restore

_done_saving_user_windows:

	!
	! Restore cwp 
	!
	mov	%g4, %psr
	nop; nop; nop

	!
	! Set wim to (cwp+1)
	!
	mov	%l4, %g2
	ROTATE_WIM_VALUE_ONE_LEFT(%g2,%g1)
	mov	%g2, %wim

	!
	! Increment the %sp past the globals that we saved on the stack
	!
	sub	%l5, GLOBAL_SAVE_SIZE, %sp

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
	! set the vector number (tic number)
	!
	mov 	%l6, %o0

	!
	! Write the exception info into the space reserved above
	!
	st 	%o0, [ %o1 + (0*4) ]		! vector number
	st	%l1, [ %o1 + (1*4) ]		! pc
	st	%l2, [ %o1 + (2*4) ]		! npc
	st 	%l0, [ %o1 + (3*4) ]		! psr
	and	%l0, PSR_CWP, %g1		! calculate the cwp
	st 	%g1, [ %o1 + (4*4) ]		! cwp
	ld	[%l5], %g1			! get wim (%l5 still pts to it)
	st 	%g1, [ %o1 + (5*4) ]		! wim
	st 	%fp, [ %o1 + (6*4) ]		! sp_prev_win = fp
	st 	%l3, [ %o1 + (7*4) ]		! sp_next_win = sp of next win

	!
	! switch to kernel trap tables
	!
	set	_k_traptable, %g1
	mov	%g1, %tbr			! %tbr <- _k_trabtable
	nop; nop; nop

	!
	! Turn off interrupts
	!
	or 	%l0, PSR_PIL, %g1
	mov 	%g1, %psr
	nop; nop; nop

	!
	! Enable traps
	!
	or 	%l0, PSR_PIL | PSR_ET, %g1
	mov 	%g1, %psr
	nop; nop; nop

	!
	! Call int_demux(vector, exception_frame_ptr)
	!	- at this point
	!		%o0 = vector
	!		%01 = exception_frame_ptr
	!
	call 	_int_demux
	nop

	!
	! Disable Traps
	! 	Interrupts better be disabled, and traps enabled
	!
	mov	%psr, %g1
	andn	%g1, PSR_ET, %g1
	mov	%g1, %psr
	nop; nop; nop

	!
	! switch to user trap tables
	!
	set	_u_traptable, %g1
	mov	%g1, %tbr			! %tbr <- _u_trabtable
	nop; nop; nop

	!
	! Store info in globals needed for restoring reg window contents
	!
	mov	%l4, %g1			! %g1 <- old 2^cwp
	mov	%l5, %g3			! %g3 <- save area
	mov	%l0, %g4			! %g4 <- old %psr
	mov	%g0, %wim

	!
	! Reset psr (including the cwp!)
	!	- since we are reloading ALL the registers, we might as well
	!	  put them back in exactly the same register windows
	!
	mov	%l0, %psr

	!
	! Reset %g2 to orig. wim
	!	- we saved the old wim so that we know exactly how many
	!	  windows need to be restored from the stack
	!
	ld	[ %g3 ], %g2			! %g2 <- old %wim

	!
	! Restore locals and cwp (from where we dumped them on the stack)
	!
	add	%g3, 8, %g3
	nop
_restore_user_window:
	RESTORE_WINDOW(%g3)
	add	%g3, WINDOW_SAVE_SIZE, %g3
	ROTATE_WIM_VALUE_ONE_LEFT(%g1,%g5)
	andcc	%g1, %g2, %g0
	bne	_done_restoring_all_windows
	nop
	ba	_restore_user_window
	restore

_done_restoring_all_windows:

	!
	! Restore cwp
	!	- move back to the window we started in
	!
	mov	%g4, %psr
	nop; nop; nop

	!
	! Restore the original wim 
	!
	mov	%g2, %wim
	nop; nop; nop

	!
	! Restore globals
	!
	sub	%l5, GLOBAL_SAVE_SIZE, %l7
	RESTORE_G_REGS(%l7)

	!
	! ( vector number < 128 ) == ( %l6 < 128 ) == ( an interrupt )
	!
	andcc	%l6, 128, %g0	
	bne	_return_from_system_call
	mov	%l0, %psr

_return_from_interrupt:
	!
	! re-execute instruction (must reset condition codes in psr)
	!
	nop; nop; nop		! wait for the psr to take effect

#ifdef DEBUG
	KPUTC(LTR_T,%l5,%l6)
	KPUTC(LTR_G,%l5,%l6)
#endif

	jmp 	%l1
	rett 	%l2
	nop

_return_from_system_call:
	!
	! skip instruction (probably do not need to reset condition codes)
	! 	set return value
	!
#ifdef PASSING_SYSCALL_PARAMS_VIA_REGISTERS
	sub	%l5, GLOBAL_SAVE_SIZE + SPARCMINFRAME, %l7
	ld	[ %l7 + 24 ], %i0
	nop			! wait for the psr to take effect
#else
	nop; nop; nop		! wait for the psr to take effect
#endif

#ifdef DEBUG
	KPUTC(LTR_T,%l5,%l6)
	KPUTC(LTR_G,%l5,%l6)
	KPUTC(LTR_LF,%l5,%l6)
	KPUTC(LTR_CR,%l5,%l6)
#endif

	jmp 	%l2			! old nPC
	rett 	%l2 + 4			! old npC + 4
	nop				! this ^ jumps past the ta instruction



