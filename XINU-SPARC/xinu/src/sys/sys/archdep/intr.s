/* intr.s - disable(), restore(), enable(), pause(), halt() */

/*#define DISABLE_DEBUG*/
/*#define ATOMIC_DISABLE_DEBUG*/

#define DEBUG

#include <asl.h>
#include <psl.h>
#include <user_traps.h>
#include <vmem.h>

	.seg	"text"
	!
	! pause() - Suppose to suspend processing until an interrupt occurs.
	!	    On the mc68020 we used the STOP instruction, however
	!	    the SPARC does not appear to have an equivalent
	!	    instruction.  If I find one, I will fix this routine.
	!	    Otherwise just return.  I think nulluser is the
	!	    only place this is called from.
	!
	.proc 04
	PROCEDURE(pause)
	retl
	nop

	!
	! halt() - call the PROM v_exit_to_mon routine to return
	!	   to the SUN PROM monitor
	!
	.proc 04
	PROCEDURE(halt)
	set     _romp, %g1
	ld      [%g1], %g1
	add     %g1, 0x74, %g1
	ld      [%g1], %g1
	jmp     %g1
	nop
	retl
	nop

!
	! disable(ps) - Set PIL field of PSR to 0xf (level 15) - save old value
	!		Called by disable(ps) where ps is  int *ps;
	!		We are a leaf node - %o0 has ps
	!
	.proc 04
	PROCEDURE(disable)
#ifdef DISABLE_DEBUG
	KPUTC(68, %l3, %l6)
	KPUTC(73, %l3, %l6)
	KPUTC(10, %l3, %l6)
	KPUTC(13, %l3, %l6)
#endif
	ta	U_TRAP_DISABLE		! trap to atomic_disable()
	nop
	st	%o1, [%o0]		! save old psr in *ps
#ifdef DISABLE_DEBUG
	KPUTC(73, %l3, %l6)
	KPUTC(68, %l3, %l6)
	KPUTC(10, %l3, %l6)
	KPUTC(13, %l3, %l6)
#endif
	retl
	nop

	!
	! atomic_disable()
	!	This routine atomically reads, then writes the PSR and
	!	is called via a ta 0x0 instruction.  It leaves interrupts
	!	disabled (level 15) and returns the old PSR in %i1
	!	(see disable)
	!	Note: the interrupt pushed us into the interrupt window
	!	(i.e. one window past the window disable() was running in)
	!	%l0 has the psr
	!	%l1 and %l2 have PC and nPC
	.proc 04
	PROCEDURE(atomic_disable)
#ifdef ATOMIC_DISABLE_DEBUG
	KPUTC(65, %l3, %l6)
	KPUTC(68, %l3, %l6)
	KPUTC(10, %l3, %l6)
	KPUTC(13, %l3, %l6)
#endif
	mov	%psr, %i1		! save current psr in %i1
	or	%i1, PSR_PIL, %l3	! or in 0xf00 to get new psr
	mov	%l3, %psr		! set the new psr
	nop; nop; nop;			! play it safe - wait
	and	%i1, PSR_PIL, %i1	! mask off everything but old PIL bits
#ifdef ATOMIC_DISABLE_DEBUG
	KPUTC(68, %l3, %l6)
	KPUTC(65, %l3, %l6)
	KPUTC(10, %l3, %l6)
	KPUTC(13, %l3, %l6)
#endif
	jmp 	%l2			! old nPC
	rett 	%l2 + 4			! old npC + 4
	nop				! this ^ jumps past the ta instruction


	!
	! restore(ps) - Restore PIL field of PSR to old value
	!		Called by restore(ps) where ps is  int *ps;
	!		We are a leaf node - %o0 has ps
	!
	.proc 04
	PROCEDURE(restore)
	ld	[%o0], %o1		! mov old PIL to %o1 for atomic_restore
	nop				! make sure load is done
	ta	U_TRAP_RESTORE		! trap to atomic_restore
	nop
	retl
	nop

	!
	! atomic_restore()
	!	This routine atomically reads, then writes the PSR and
	!	is called via a ta 0x1 instruction.  It restores interrupts
	!	to the old PSR value in %i1 (see restore)
	!	Note: the interrupt pushed us into the interrupt window
	!	(i.e. one window past the window restore() was running in)
	!	%l0 has the psr
	!	%l1 and %l2 have PC and nPC
	.proc 04
	PROCEDURE(atomic_restore)
	and	%i1, PSR_PIL, %l3	! make sure non-PIL fields are zero
	mov	%psr, %l4		! save current psr in %l4
	andn	%l4, PSR_PIL, %l4	! %l4 = %l4 & ~PSR_PIL
	or	%l4, %l3, %l4		! fill in old PIL value
	mov	%l4, %psr		! set the new psr
	nop; nop; nop			! play it safe - wait
	jmp 	%l2			! old nPC
	rett 	%l2 + 4			! old npC + 4
	nop				! this ^ jumps past the ta instruction

	!
	! enable() - Set PIL field of PSR to Level 0
	!	   - We are a leaf node 
	!	   - This routine assumes interrupts are disabled, so we
	!	     do NOT need to issue a Ticc instruction to atomically
	!	     read and write the PSR
	!
	.proc 04
	PROCEDURE(enable)
	mov	%psr, %o2		! save current psr in %02
	andn	%o2, PSR_PIL, %o2	! %o2 = %o2 & ~PSR_PIL
	mov	%o2, %psr		! set the new psr
	nop; nop; nop			! play it safe - wait

	! clear the "disable_all" state by		--- MARGARET
	! setting the rightmost bit in INTERRUPT_REG
	! *((unsigned char *) INTERRUPT_REG) |=  1;
	set	INTERRUPT_REG, %o2
	ldub	[%o2], %o3
	or	%o3, 1, %o3
	stb	%o3, [%o2]

	retl
	nop




	!
	! change_to_user_mode() - Set the Supervisor Bit of the PSR to 0
	!		We are a leaf node - %o0 has val
	!
	.proc 04
	PROCEDURE(change_to_user_mode)
	ta	U_TRAP_TO_USER_MODE	! trap to atomic_change_to_user_mode
	nop
	retl
	nop

	!
	! atomic_change_to_user_mode()
	!	This routine atomically reads, then writes the PSR and
	!	is called via a ta instruction.  It sets the PSR S bit to 0.
	!	We do this by setting PS = 0 and then returning from the trap.
	!	Note: the interrupt pushed us into the interrupt window
	!	(i.e. 1 window past the window change_to_user_mode()
	!	 was running in)
	!	%l0 has the psr
	!	%l1 and %l2 have PC and nPC
	.proc 04
	PROCEDURE(atomic_change_to_user_mode)
	mov	%psr, %l4		! save current psr in %l4
	andn	%l4, PSR_PS, %l4	! %l4 = %l4 & ~PSR_PS
	mov	%l4, %psr		! write the new value into the psr
	nop; nop; nop			! play it safe - wait
	jmp 	%l2			! old nPC
	rett 	%l2 + 4			! old npC + 4
	nop				! this ^ jumps past the ta instruction

	!
	! getpsr() - returns the current PSR value (for debugging only)
	!
	.proc 04
	PROCEDURE(getpsr)
	mov	%psr, %o0		! save current psr in %o0
	nop; nop; nop			! play it safe - wait
	retl
	nop


	!
	! unsigned int get_system_enable_reg()
	!
	.proc 04
	PROCEDURE(get_system_enable_reg)
	clr	%o0
	set	SYSTEM_ENABLE, %o1
	retl
	lduba	[ %o1 ]ASI_CTL, %o0


	!
	! void set_system_enable_register( unsigned int )
	!
	.proc 04
	PROCEDURE(set_system_enable_reg)
	set	SYSTEM_ENABLE, %o1
	retl
	stba	%o0, [ %o1 ]ASI_CTL


#ifdef DEBUG
	! Margaret
	.proc 04
	PROCEDURE(get_int_reg)
	set 0xFFD0A000, %o0
	ld  [%o0], %o0
	retl
	nop

	.proc 04
	PROCEDURE(get_ret_addr)
	retl
	mov %i7, %o0

	.proc 04
	PROCEDURE(get_psr)
	rd %psr, %o0
	nop; nop; nop
	retl
	nop

	.proc 04
	PROCEDURE(get_tbr)
	rd %tbr, %o0
	nop; nop; nop
	retl
	nop

#endif

	.proc 04
	PROCEDURE(ClockTrap)
	jmp     %l1
	rett    %l2
	nop

