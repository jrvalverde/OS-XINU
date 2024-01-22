/* user_winint.s - register window trap handlers routines */

#include <asl.h>
#include <reg.h>
#include <vmem.h>

/*#define OVERFLOW_DEBUG*/
/*#define UNDERFLOW_DEBUG*/
/*#define DEBUG_TOUGH_OFLOWS*/
/*#define DEBUG_TOUGH_UFLOWS*/

/* Define this if you know that your stacks are going to be	*/
/* aligned on 8 byte boundaries.  Then this routine will use	*/
/* ldd and std to make coping faster.				*/
#define CAN_USE_DOUBLE_WORD_OPS

/*---------------------------------------------------------------------------
 * user_winint_overflow - handle register window overflows
 *			Upon entry %l0=psr, %l1=pc, %l2=npc
 *---------------------------------------------------------------------------
 */
	.seg	"text"
	.align	4
	PROCEDURE(user_winint_overflow)

#ifdef OVERFLOW_DEBUG
	KPUTC(LTR_O, %l3, %l4)
	KPUTC(LTR_F, %l3, %l4)
	KPUTC(LTR_SP, %l3, %l4)
#endif

	/* temporarily save away %g6 and %g7 */
	mov	%g6, %l4
	mov	%g7, %l5

	/* move to the window to be saved */
	save

	/* see if the region begins on a 4/8 byte boundary */
#ifdef CAN_USE_DOUBLE_WORD_OPS
	and 	%sp, 0x7, %g6		! stack not aligned on 8 byte boundary
#else
	and 	%sp, 0x3, %g6		! stack not aligned on 4 byte boundary
#endif

	/* see if beginning of the region is mapped and writable */
	lda	[%sp]ASI_PM, %g7		! get page table entry
	srl	%g7, 29, %g7
	sub	%g7, 0x06, %g7			! check valid and writable

	/* Did any of the previous two checks turn up an error? */
	or	%g7, %g6, %g6

	/* see if end of the region is mapped and writable */
	add	%sp, WINDOW_SAVE_SIZE - 4, %g7
	lda	[ %g7 ]ASI_PM, %g7		! get page table entry
	srl	%g7, 29, %g7
	sub	%g7, 0x06, %g7			! check valid and writable

	/* Did any of the previous checks turn up an error? */
	orcc	%g7, %g6, %g6

	bne	_ovflw_too_hard_too_handle_simply
	nop

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

	/* restore %g6 and %g7 to their original values */
	mov	%l4, %g6
	mov	%l5, %g7

#ifdef OVERFLOW_DEBUG
	KPUTC(LTR_F, %l3, %l4)
	KPUTC(LTR_O, %l3, %l4)
	KPUTC(LTR_LF, %l3, %l4)
	KPUTC(LTR_CR, %l3, %l4)
#endif

	jmp	%l1
	rett	%l2
	nop

_ovflw_too_hard_too_handle_simply:
#ifdef DEBUG_TOUGH_OFLOW
	KPUTC(LTR_B, %g6, %g7)
	KPUTC(LTR_A, %g6, %g7)
	KPUTC(LTR_D, %g6, %g7)
	KPUTC(LTR_O, %g6, %g7)
	KPUTC(LTR_F, %g6, %g7)
	KPUTC(LTR_SP, %g6, %g7)
#endif
	/* we can't handle it, get back to orig window and call genericint() */
	mov	%wim, %g6
	mov	%g0, %wim
	mov	%sp, %g7
	nop; nop				! only 2 nops necessary
	restore
	mov	%g6, %wim

	/* store the stack pointer in %ls for genericint() */
	mov	%g7, %l3

	/* restore %g6 and %g7 to their original values */
	mov	%l4, %g6
	mov	%l5, %g7

#ifdef DEBUG_TOUGH_OFLOW
	KPUTC(LTR_G, %l3, %l4)
	KPUTC(LTR_I, %l3, %l4)
	KPUTC(LTR_V, %l3, %l4)
	KPUTC(LTR_U, %l3, %l4)
	KPUTC(LTR_P, %l3, %l4)
	KPUTC(LTR_LF, %l3, %l4)
	KPUTC(LTR_CR, %l3, %l4)
#endif
	ba	_user_genericint
	nop


/*---------------------------------------------------------------------------
 * user_winint_underflow - handle register window overflows
 *---------------------------------------------------------------------------
 */
	PROCEDURE(user_winint_underflow )

#ifdef UNDERFLOW_DEBUG
	KPUTC(LTR_U, %l3, %l4)
	KPUTC(LTR_F, %l3, %l4)
	KPUTC(LTR_SP, %l3, %l4)
#endif

	ROTATE_WIM_LEFT(%l4,%l5)
	mov	%g6, %l4
	mov	%g7, %l5			! only need two instructions

	restore
	restore

	/* see if the begging of the region begins on a 4/8 byte boundary */
#ifdef CAN_USE_DOUBLE_WORD_OPS
	and 	%sp, 0x7, %g6		! stack not aligned on 8 byte boundary
#else
	and 	%sp, 0x3, %g6		! stack not aligned on 4 byte boundary
#endif

	/* see if beginning of the region is mapped and writable */
	lda	[%sp]ASI_PM, %g7		! get page table entry
	srl	%g7, 29, %g7
	sub	%g7, 0x06, %g7			! check valid and writable

	/* Did any of the previous two checks turn up an error? */
	or	%g7, %g6, %g6

	/* see if end of the region is mapped and writable */
	add	%sp, WINDOW_SAVE_SIZE - 4, %g7
	lda	[ %g7 ]ASI_PM, %g7		! get page table entry
	srl	%g7, 29, %g7
	sub	%g7, 0x06, %g7			! check valid and writable

	/* Did any of the previous checks turn up an error? */
	orcc	%g7, %g6, %g6

	bne	_undrflw_too_hard_too_handle_simply
	nop

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

	/* restore %g6 and %g7 to their original values */
	mov	%l4, %g6
	mov	%l5, %g7

#ifdef UNDERFLOW_DEBUG
	KPUTC(LTR_F, %l3, %l4)
	KPUTC(LTR_U, %l3, %l4)
	KPUTC(LTR_LF, %l3, %l4)
	KPUTC(LTR_CR, %l3, %l4)
#endif


	jmp	%l1
	rett	%l2
	nop

_undrflw_too_hard_too_handle_simply:
#ifdef DEBUG_TOUGH_UFLOW
	KPUTC(LTR_B, %g6, %g7)
	KPUTC(LTR_A, %g6, %g7)
	KPUTC(LTR_D, %g6, %g7)
	KPUTC(LTR_U, %g6, %g7)
	KPUTC(LTR_F, %g6, %g7)
	KPUTC(LTR_SP, %g6, %g7)
#endif
	/* we can't handle the under flow easily, get back to interrupt */
	/* window and then call genericint()				*/
	mov	%sp, %g7
	save	%g0, %g0, %g0
	save	%g0, %g0, %g0

	/* save stack pointer in %l3 for genericint() */
	mov	%g7, %l3

	/* restore %g6 and %g7 */
	mov	%l4, %g6
	mov	%l5, %g7

	!
	! reset wim to original value
	!
	ROTATE_WIM_RIGHT(%l4,%l5)

#ifdef DEBUG_TOUGH_UFLOW
	KPUTC(LTR_G, %l3, %l4)
	KPUTC(LTR_I, %l3, %l4)
	KPUTC(LTR_V, %l3, %l4)
	KPUTC(LTR_U, %l3, %l4)
	KPUTC(LTR_P, %l3, %l4)
	KPUTC(LTR_LF, %l3, %l4)
	KPUTC(LTR_CR, %l3, %l4)
#endif
	ba	_user_genericint
	nop

