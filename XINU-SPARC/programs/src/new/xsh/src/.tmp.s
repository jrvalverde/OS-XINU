# 1 "setjmp.s" 



SC_ONSTACK	= (0*4)	! offsets in sigcontext structure
SC_MASK		= (1*4)
SC_SP		= (2*4)
SC_PC		= (3*4)
SC_NPC		= (4*4)
SC_PSR		= (5*4)
SC_G1		= (6*4)
SC_O0		= (7*4)

SS_SP		= (0*4)	! offset in sigstack structure
SS_ONSTACK	= (1*4)
SIGSTACKSIZE	= (2*4)














	.global _setjmp; _setjmp:
	clr	[%o0 + SC_ONSTACK]	! clear onstack, sigmask
	clr	[%o0 + SC_MASK]
	st	%sp, [%o0 + SC_SP]	! save caller's sp
	add	%o7, 8, %g1		! comupte return pc
	st	%g1, [%o0 + SC_PC]	! save pc
	inc	4, %g1			! npc = pc + 4
	st	%g1, [%o0 + SC_NPC]
	clr	[%o0 + SC_PSR]		! psr (icc), g1 = 0 (paranoid)
	clr	[%o0 + SC_G1]		!   o0 filled in by longjmp
	retl
	clr	%o0			! return (0)














	.global _longjmp; _longjmp:
					! flush all reg windows to the stack.
					! Issue lots in case of many windows
	save; save; save; save
	save; save; save; save
	restore; restore; restore; restore
	restore; restore; restore; restore
	sub	%sp, (16*4), %sp	! establish new save area before
	ld	[%o0 + SC_SP], %fp	!  adjusting fp to new stack frame
	ld	[%o0 + SC_PC], %g1	! get new return pc
	tst	%o1			! is return value 0?
	bne	1f			! no - leave it alone
	sub	%g1, 8, %o7		! normalize return (for adb) (dly slot)
	mov	1, %o1			! yes - set it to one
1:
	retl
	restore	%o1, 0, %o0		! return (val)
