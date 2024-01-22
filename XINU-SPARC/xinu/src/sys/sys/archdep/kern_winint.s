
/* kern_winint.s - register window trap handlers routines */

#include <asl.h>
#include <reg.h>
#include <vmem.h>
#include <psl.h>

/*#define OVERFLOW_DEBUG*/
/*#define UNDERFLOW_DEBUG*/
/*#define DEBUG_TOUGH_OFLOWS*/
/*#define DEBUG_TOUGH_UFLOWS*/

/*#define CHECK_FOR_KERN_STK_ERRORS*/

/* Define this if you know that your stacks are going to be	*/
/* aligned on 8 byte boundaries.  Then this routine will use	*/
/* ldd and std to make coping faster.				*/
#define CAN_USE_DOUBLE_WORD_OPS

/*---------------------------------------------------------------------------
 * kern_winint_overflow - handle register window overflows
 *			Upon entry %l0=psr, %l1=pc, %l2=npc
 *---------------------------------------------------------------------------
 */
	.seg	"text"
	.align	4
	PROCEDURE(kern_winint_overflow)

#ifdef OVERFLOW_DEBUG
	KPUTC(LTR_O, %l3, %l4)
	KPUTC(LTR_F, %l3, %l4)
	KPUTC(LTR_SP, %l3, %l4)
#endif

#ifdef CHECK_FOR_KERN_STK_ERRORS
	/* temporarily save away %g6 and %g7 */
	mov	%g6, %l4
	mov	%g7, %l5
#endif

	save

#ifdef CHECK_FOR_KERN_STK_ERRORS
	/* see if the begging of the region begins on a 4/8 byte boundary */
#ifdef CAN_USE_DOUBLE_WORD_OPS
	and 	%sp, 0x7, %g6		! stack not aligned on 8 byte boundary
#else
	and 	%sp, 0x3, %g6		! stack not aligned on 4 byte boundary
#endif

	/* see if beginning of the region is mapped and writable */
	lda	[%sp]ASI_PM, %g7		! get page table entry
	srl	%g7, 30, %g7
	sub	%g7, 0x03, %g7			! check valid and writable

	/* Did any of the previous two checks turn up an error? */
	or	%g7, %g6, %g6

	/* see if end of the region is mapped and writable */
	add	%sp, WINDOW_SAVE_SIZE - 4, %g7
	lda	[ %g7 ]ASI_PM, %g7		! get page table entry
	srl	%g7, 30, %g7
	sub	%g7, 0x03, %g7			! check valid and writable

	/* Did any of the previous checks turn up an error? */
	orcc	%g7, %g6, %g6

	bne	_ovflw_too_hard_too_handle_simply
	nop

#endif CHECK_FOR_KERN_STK_ERRORS

	!
	! save window
	!
#ifdef CAN_USE_DOUBLE_WORD_OPS
	SAVE_WINDOW_USING_DOUBLES(%sp)
#else
	SAVE_WINDOW(%sp)
#endif
	ROTATE_WIM_RIGHT(%l4,%l5)

	restore

#ifdef CHECK_FOR_KERN_STK_ERRORS
	/* restore %g6 and %g7 to their original values */
	mov	%l4, %g6
	mov	%l5, %g7
#endif

#ifdef OVERFLOW_DEBUG
	KPUTC(LTR_F, %l3, %l4)
	KPUTC(LTR_O, %l3, %l4)
	KPUTC(LTR_LF, %l3, %l4)
	KPUTC(LTR_CR, %l3, %l4)
#endif

	jmp	%l1
	rett	%l2
	nop

#ifdef CHECK_FOR_KERN_STK_ERRORS

/* If we reach this routine, then there is a major error, cause we are	*/
/* already (supposed to be) running on a locked down stack, and there	*/
/* is no reason why the stack pointer should not be aligned properly.	*/
/* So, just switch to a new stack, and call win_oflow_error().		*/
_ovflw_too_hard_too_handle_simply:
#ifdef DEBUG_TOUGH_OFLOWS
	KPUTC(LTR_K, %g6, %g7)
	KPUTC(LTR_S, %g6, %g7)
	KPUTC(LTR_T, %g6, %g7)
	KPUTC(LTR_K, %g6, %g7)
	KPUTC(LTR_O, %g6, %g7)
	KPUTC(LTR_V, %g6, %g7)
	KPUTC(LTR_F, %g6, %g7)
	KPUTC(LTR_L, %g6, %g7)
	KPUTC(LTR_SP, %g6, %g7)
#endif
	/* we can't handle it, get back to orig window and call genericint() */
	mov	%wim, %g6
	mov	%g0, %wim
	mov	%sp, %g7
	nop; nop				! only 2 nops necessary
	restore
	mov	%g6, %wim

	/* save stack pointer in %o2 for win_oflow_error() */
	mov	%g7, %o2

	/* restore %g6 and %g7 to their original values */
	mov	%l4, %g6
	mov	%l5, %g7

	/* save the pc and npc for win_uflow_error() */
	mov	%l1, %o0
	mov	%l2, %o1

	/* change to a temporary stack */
	set	_stacktop, %sp
	sub	%sp, WINDOW_SAVE_SIZE, %sp

	/* enable traps */
	or 	%l0, PSR_PIL | PSR_ET, %g1
	mov 	%g1, %psr
	nop; nop; nop

	

#ifdef DEBUG_TOUGH_OFLOWS
	KPUTC(LTR_E, %l3, %l4)
	KPUTC(LTR_X, %l3, %l4)
	KPUTC(LTR_I, %l3, %l4)
	KPUTC(LTR_T, %l3, %l4)
	KPUTC(LTR_LF, %l3, %l4)
	KPUTC(LTR_CR, %l3, %l4)
#endif
	ba	_win_oflow_error
	nop

#endif CHECK_FOR_KERN_STK_ERRORS


/*---------------------------------------------------------------------------
 * kern_winint_underflow - handle register window overflows
 *---------------------------------------------------------------------------
 */
	PROCEDURE(kern_winint_underflow)

#ifdef UNDERFLOW_DEBUG
	KPUTC(LTR_U, %l3, %l4)
	KPUTC(LTR_F, %l3, %l4)
	KPUTC(LTR_SP, %l3, %l4)
#endif

	/* move WIM out of the way, so we can restore window from stack */
	ROTATE_WIM_LEFT(%l4,%l5)

#ifdef CHECK_FOR_KERN_STK_ERRORS
	mov	%g6, %l4
	mov	%g7, %l5		! only need two instructions
#endif

	/* move to the window we want to restore */
	restore
	restore

#ifdef CHECK_FOR_KERN_STK_ERRORS
	/* see if the begging of the region begins on a 4/8 byte boundary */
#ifdef CAN_USE_DOUBLE_WORD_OPS
	and 	%sp, 0x7, %g6		! stack not aligned on 8 byte boundary
#else
	and 	%sp, 0x3, %g6		! stack not aligned on 4 byte boundary
#endif

	/* see if beginning of the region is mapped and writable */
	lda	[%sp]ASI_PM, %g7		! get page table entry
	srl	%g7, 30, %g7
	sub	%g7, 0x03, %g7			! check valid and writable

	/* Did any of the previous two checks turn up an error? */
	or	%g7, %g6, %g6

	/* see if end of the region is mapped and writable */
	add	%sp, WINDOW_SAVE_SIZE - 4, %g7
	lda	[ %g7 ]ASI_PM, %g7		! get page table entry
	srl	%g7, 30, %g7
	sub	%g7, 0x03, %g7			! check valid and writable

	/* Did any of the previous checks turn up an error? */
	orcc	%g7, %g6, %g6

	bne	_undrflw_too_hard_too_handle_simply
	nop

#endif CHECK_FOR_KERN_STK_ERRORS

	!
	! restore window
	!
#ifdef CAN_USE_DOUBLE_WORD_OPS
	RESTORE_WINDOW_USING_DOUBLES(%sp)
#else
	RESTORE_WINDOW(%sp)
#endif

	/* move back to the trap window */
	save
	save

#ifdef CHECK_FOR_KERN_STK_ERRORS
	/* restore %g6 and %g7 to their original values */
	mov	%l4, %g6
	mov	%l5, %g7
#endif

#ifdef UNDERFLOW_DEBUG
	KPUTC(LTR_F, %l3, %l4)
	KPUTC(LTR_U, %l3, %l4)
	KPUTC(LTR_LF, %l3, %l4)
	KPUTC(LTR_CR, %l3, %l4)
#endif

	jmp	%l1
	rett	%l2
	nop

#ifdef CHECK_FOR_KERN_STK_ERRORS

/* If we reach this routine, then there is a major error, cause we are	*/
/* already (supposed to be) running on a locked down stack, and there	*/
/* is no reason why the stack pointer should not be aligned properly.	*/
/* So, just switch to a new stack, and call win_uflow_error().		*/
_undrflw_too_hard_too_handle_simply:
#ifdef DEBUG_TOUGH_UFLOWS
	KPUTC(LTR_K, %g6, %g7)
	KPUTC(LTR_S, %g6, %g7)
	KPUTC(LTR_T, %g6, %g7)
	KPUTC(LTR_K, %g6, %g7)
	KPUTC(LTR_U, %g6, %g7)
	KPUTC(LTR_D, %g6, %g7)
	KPUTC(LTR_F, %g6, %g7)
	KPUTC(LTR_L, %g6, %g7)
	KPUTC(LTR_SP, %g6, %g7)
#endif
	/* get back to interrupt window and then call panic */
	mov	%sp, %g7
	save	%g0, %g0, %g0
	save	%g0, %g0, %g0

	/* save stack pointer in %o2 for win_uflow_error() */
	mov	%g7, %o2

	/* restore %g6 and %g7 */
	mov	%l4, %g6
	mov	%l5, %g7

	/* reset wim to original value */
	ROTATE_WIM_RIGHT(%l4,%l5)

	/* save the pc and npc for win_uflow_error() */
	mov	%l1, %o0
	mov	%l2, %o1

	/* change to a temporary stack - see startup.s */
	set	_stacktop, %sp
	sub	%sp, WINDOW_SAVE_SIZE, %sp

	/* enable traps */
	or 	%l0, PSR_PIL | PSR_ET, %g1
	mov 	%g1, %psr
	nop; nop; nop

#ifdef DEBUG_TOUGH_UFLOWS
	KPUTC(LTR_E, %l3, %l4)
	KPUTC(LTR_X, %l3, %l4)
	KPUTC(LTR_I, %l3, %l4)
	KPUTC(LTR_T, %l3, %l4)
	KPUTC(LTR_LF, %l3, %l4)
	KPUTC(LTR_CR, %l3, %l4)
#endif
	ba	_win_uflow_error
	nop

#endif CHECK_FOR_KERN_STK_ERRORS

