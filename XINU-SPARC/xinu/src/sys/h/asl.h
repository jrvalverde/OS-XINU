/* asl.h - sparc assembly language header file	*/

/*----------------------------------------------------------------------*/
/*		Useful Assembly Language Macros 			*/
/*									*/
/* NOTE: .s files include this file, so only put #defines in here 	*/
/*----------------------------------------------------------------------*/

#define MAX_NUM_WINDOWS	8
/*
 * Stack size/alignment macros.
 */
#define GLOBAL_SAVE_SIZE	(32)	/* (8*4), 8 %g registers + %y reg */
#define LOCAL_SAVE_SIZE		(32)	/* (8*4), 8 %l registers */
#define IN_SAVE_SIZE		(32)	/* (8*4), 8 %i registers */
#define WINDOW_SAVE_SIZE	(64)	/* (16*4), 8 %i and 8 %l registers */
#define ARGPUSHSIZE		(24)	/* (6*4), register arg save area */
#define ARGPUSH		(WINDOW_SAVE_SIZE+4)	/* arg dump area offset */
#define MINFRAME	(WINDOW_SAVE_SIZE+ARGPUSHSIZE+4) /* min frame */
#define UARTABASE 	0xf0000004
#define STACK_ALIGN	8
#define SA(X)		(((X)+(STACK_ALIGN-1)) & ~(STACK_ALIGN-1))

#define SPARCMINFRAME	SA(MINFRAME)

#define CWP_MASK	0x0000001f	/* mask off the CWP	*/
#define MYPUTC_ASI_CTL         2       	/* from vmem.sparc.h for MYPUTC() */

#define MYPUTC(ch, rch, rx)		\
        set	UARTABASE, rx;		\
        lduba   [rx]MYPUTC_ASI_CTL, rx;	\
        andcc   rx, 0x04, %g0;		\
        be      .-16;			\
        nop;				\
					\
        set     UARTABASE+2, rx;	\
	mov	ch,rch;			\
        stba    rch, [rx]MYPUTC_ASI_CTL;	\
					\
        set	UARTABASE, rx;		\
        lduba   [rx]MYPUTC_ASI_CTL, rx;	\
        andcc   rx, 0x04, %g0;		\
        be      .-16;			\
        nop
#define KPUTC(ch, r1, r2)	\
	set 	0xf0000004, r1;	\
	lduba 	[r1] 0x2, r2;	\
	andcc 	r2, 0x04, %g0;	\
	bz	.-8;		\
	nop;			\
		\
	mov	ch, r2;		\
	inc 	2, r1;		\
	stba 	r2 , [r1]0x2;	\
		\
	set 	0xf0000004, r1;	\
	lduba 	[r1] 0x2, r2;	\
	andcc 	r2, 0x04, %g0;	\
	bz	.-8;		\
	nop
    
#define KPUTN(rn,r0,r1,r2)	\
	mov	rn,r0;		\
	srl	r0, 30, r1;	\
	sll	r0, 2, r0;	\
	add	r1, 0x30, r1;	\
	KPUTC(r1,r1,r2);	\
	srl	r0, 29, r1;	\
	sll	r0, 3, r0;	\
	add	r1, 0x30, r1;	\
	KPUTC(r1,r1,r2);	\
	srl	r0, 29, r1;	\
	sll	r0, 3, r0;	\
	add	r1, 0x30, r1;	\
	KPUTC(r1,r1,r2);	\
	srl	r0, 29, r1;	\
	sll	r0, 3, r0;	\
	add	r1, 0x30, r1;	\
	KPUTC(r1,r1,r2);	\
	srl	r0, 29, r1;	\
	sll	r0, 3, r0;	\
	add	r1, 0x30, r1;	\
	KPUTC(r1,r1,r2);	\
	srl	r0, 29, r1;	\
	sll	r0, 3, r0;	\
	add	r1, 0x30, r1;	\
	KPUTC(r1,r1,r2);	\
    	srl	r0, 29, r1;	\
	sll	r0, 3, r0;	\
	add	r1, 0x30, r1;	\
	KPUTC(r1,r1,r2);	\
	srl	r0, 29, r1;	\
	sll	r0, 3, r0;	\
	add	r1, 0x30, r1;	\
	KPUTC(r1,r1,r2);	\
	srl	r0, 29, r1;	\
	sll	r0, 3, r0;	\
	add	r1, 0x30, r1;	\
	KPUTC(r1,r1,r2);	\
	srl	r0, 29, r1;	\
	sll	r0, 3, r0;	\
	add	r1, 0x30, r1;	\
	KPUTC(r1,r1,r2);	\
	srl	r0, 29, r1;	\
	sll	r0, 3, r0;	\
	add	r1, 0x30, r1;	\
	KPUTC(r1,r1,r2)

#define PROCEDURE(x) \
	.global	_/**/x; \
	_/**/x:

					/* This assumes %g1 is not is use */
#define DELAYLOOP(count)		\
    	set	count, %g1;		\
1:;					\
	dec	%g1;			\
	cmp	%g1, %g0;		\
	bne	1b;			\
	nop


/*---------------------------------------------------------------------------
 * Macros for saving/restoring register windows
 *---------------------------------------------------------------------------
 */

#define SAVE_WINDOW(SBP) \
	SAVE_LOCALS(SBP); \
	SAVE_INS(SBP)

#define SAVE_LOCALS(SBP) \
	st	%l0, [SBP + (0*4)]; \
	st	%l1, [SBP + (1*4)]; \
	st	%l2, [SBP + (2*4)]; \
	st	%l3, [SBP + (3*4)]; \
	st	%l4, [SBP + (4*4)]; \
	st	%l5, [SBP + (5*4)]; \
	st	%l6, [SBP + (6*4)]; \
	st	%l7, [SBP + (7*4)]

#define SAVE_INS(SBP) \
	st	%i0, [SBP + (8*4)]; \
	st	%i1, [SBP + (9*4)]; \
	st	%i2, [SBP + (10*4)]; \
	st	%i3, [SBP + (11*4)]; \
	st	%i4, [SBP + (12*4)]; \
	st	%i5, [SBP + (13*4)]; \
	st	%i6, [SBP + (14*4)]; \
	st	%i7, [SBP + (15*4)]

#define SAVE_G_REGS(ADDR) \
        st      %g1, [ADDR + (1*4)]; \
        st      %g2, [ADDR + (2*4)]; \
        st      %g3, [ADDR + (3*4)]; \
        st      %g4, [ADDR + (4*4)]; \
        st      %g5, [ADDR + (5*4)]; \
        st      %g6, [ADDR + (6*4)]; \
        st      %g7, [ADDR + (7*4)]; \
        mov     %y, %g1;             \
        st      %g1, [ADDR + (0*4)]; \
	ld	[ADDR + (1*4)], %g1

#define RESTORE_WINDOW(SBP) \
	RESTORE_LOCALS(SBP); \
	RESTORE_INS(SBP)
    
#define RESTORE_LOCALS(SBP) \
	ld	[SBP + (0*4)], %l0; \
	ld	[SBP + (1*4)], %l1; \
	ld	[SBP + (2*4)], %l2; \
	ld	[SBP + (3*4)], %l3; \
	ld	[SBP + (4*4)], %l4; \
	ld	[SBP + (5*4)], %l5; \
	ld	[SBP + (6*4)], %l6; \
	ld	[SBP + (7*4)], %l7

#define RESTORE_INS(SBP) \
	ld	[SBP + (8*4)], %i0; \
	ld	[SBP + (9*4)], %i1; \
	ld	[SBP + (10*4)], %i2; \
	ld	[SBP + (11*4)], %i3; \
	ld	[SBP + (12*4)], %i4; \
	ld	[SBP + (13*4)], %i5; \
	ld	[SBP + (14*4)], %i6; \
	ld	[SBP + (15*4)], %i7
    
#define RESTORE_G_REGS(ADDR) \
        ld      [ADDR + (0*4)], %g1; \
        mov     %g1, %y;             \
        ld      [ADDR + (1*4)], %g1; \
        ld      [ADDR + (2*4)], %g2; \
        ld      [ADDR + (3*4)], %g3; \
        ld      [ADDR + (4*4)], %g4; \
        ld      [ADDR + (5*4)], %g5; \
        ld      [ADDR + (6*4)], %g6; \
        ld      [ADDR + (7*4)], %g7;

#define SAVE_WINDOW_USING_DOUBLES(SBP) \
	SAVE_LOCALS_USING_DOUBLES(SBP); \
	SAVE_INS_USING_DOUBLES(SBP)

#define SAVE_LOCALS_USING_DOUBLES(SBP) \
	std	%l0, [SBP + (0*4)]; \
	std	%l2, [SBP + (2*4)]; \
	std	%l4, [SBP + (4*4)]; \
	std	%l6, [SBP + (6*4)]

#define SAVE_INS_USING_DOUBLES(SBP) \
	std	%i0, [SBP + (8*4)]; \
	std	%i2, [SBP + (10*4)]; \
	std	%i4, [SBP + (12*4)]; \
	std	%i6, [SBP + (14*4)]

#define SAVE_G_REGS_USING_DOUBLES(ADDR) \
        std	%g0, [ADDR + (0*4)]; \
        std	%g2, [ADDR + (2*4)]; \
        std	%g4, [ADDR + (4*4)]; \
        std	%g6, [ADDR + (6*4)]; \
        mov	%y, %g1;             \
        st	%g1, [ADDR + (8*4)]; \
	ld	[ADDR + (1*4)], %g1

#define RESTORE_WINDOW_USING_DOUBLES(SBP) \
	RESTORE_LOCALS_USING_DOUBLES(SBP); \
	RESTORE_INS_USING_DOUBLES(SBP)
    
#define RESTORE_LOCALS_USING_DOUBLES(SBP) \
	ldd	[SBP + (0*4)], %l0; \
	ldd	[SBP + (2*4)], %l2; \
	ldd	[SBP + (4*4)], %l4; \
	ldd	[SBP + (6*4)], %l6

#define RESTORE_INS_USING_DOUBLES(SBP) \
	ldd	[SBP + (8*4)], %i0; \
	ldd	[SBP + (10*4)], %i2; \
	ldd	[SBP + (12*4)], %i4; \
	ldd	[SBP + (14*4)], %i6
    
#define RESTORE_G_REGS_USING_DOUBLES(ADDR) \
        ld	[ADDR + (0*4)], %g1; \
        mov     %g1, %y;             \
        ldd	[ADDR + (0*4)], %g0; \
        ldd	[ADDR + (2*4)], %g2; \
        ldd	[ADDR + (4*4)], %g4; \
        ldd	[ADDR + (6*4)], %g6

#define ZERO_LOCALS() \
	clr %l0; clr %l1; clr %l2; clr %l3; \
	clr %l4; clr %l5; clr %l6; clr %l7;

#define ZERO_INS() \
	clr %i0; clr %i1; clr %i2; clr %i3; \
	clr %i4; clr %i5; clr %i6; clr %i7;

#define ZERO_OUTS() \
	clr %o0; clr %o1; clr %o2; clr %o3; \
	clr %o4; clr %o5; clr %o6; clr %o7;

#define LOAD(r,addr) \
	sethi %hi( addr ), r; ld [r + %lo( addr ) ], r

#define STORE(value,addr,tmp_reg) \
	sethi %hi( addr ), tmp_reg; st value, [tmp_reg + %lo( addr ) ]

#define NWINDOWS(r1) \
	LOAD(r1, _nwindows)

#define NWINDOWS_MINUS_1(r1) \
	LOAD(r1, _nwindows_minus_1)

#define ROTATE_WIM_VALUE_ONE_LEFT(r, tmp_reg) \
	NWINDOWS_MINUS_1(tmp_reg); \
	srl r, tmp_reg, tmp_reg; \
	sll r, 1, r; \
	or  r, tmp_reg, r
	
#define ROTATE_WIM_VALUE_ONE_RIGHT(r, tmp_reg) \
	NWINDOWS_MINUS_1(tmp_reg); \
	sll r, tmp_reg, tmp_reg; \
	srl r, 1, r; \
	or  r, tmp_reg, r

#define ROTATE_WIM_LEFT(tmp_reg_1,tmp_reg_2)			\
	rd	%wim, tmp_reg_1;				\
	NWINDOWS_MINUS_1( tmp_reg_2 );				\
	srl	tmp_reg_1, tmp_reg_2, tmp_reg_2;		\
	sll	tmp_reg_1, 1, tmp_reg_1;			\
	wr	tmp_reg_1, tmp_reg_2, %wim;			\
    	nop; nop; nop		/* play it safe */

#define ROTATE_WIM_RIGHT(tmp_reg_1,tmp_reg_2)			\
	rd	%wim, tmp_reg_1;				\
	NWINDOWS_MINUS_1( tmp_reg_2 );				\
	sll	tmp_reg_1, tmp_reg_2, tmp_reg_2;		\
	srl	tmp_reg_1, 1, tmp_reg_1;			\
	wr	tmp_reg_1, tmp_reg_2, %wim;			\
    	nop; nop; nop		/* play it safe */
     

/*---------------------------------------------------------------------------
 * Macros to fill in the Trap Table
 *   - by convention (in xinu), put %psr in %l0 before calling handler
 *---------------------------------------------------------------------------
 */
#define TRAP(addr)                      \
        sethi   %hi(addr), %l4;         \
        jmp     %l4 + %lo(addr);        \
        mov     %psr, %l0;              \
        nop
    
#define USER_GENERIC_TRAP()             \
        sethi %hi(_user_genericint), %l4;       \
        add %l4, %lo(_user_genericint), %l4;    \
        jmp %l4;                        \
        mov %psr, %l0

#define KERN_GENERIC_TRAP()		\
	sethi %hi(_kern_genericint), %l4; 	\
	add %l4, %lo(_kern_genericint), %l4;	\
	jmp %l4; 			\
	mov %psr, %l0

#define TBR_TIC_MASK		(0x00000FF0)
#define TBR_TIC_SHIFT		(0x04)
#define TBR_TIC_RANGE_MASK	(0x00000080)


/*---------------------------------------------------------------------------
 * A list of ASCII codes - useful for asm KPUTCs
 *---------------------------------------------------------------------------
 */
#define LTR_LF	0xa
#define LTR_CR	0xd
#define LTR_SP	0x20

#define LTR_A	0x41
#define LTR_B	0x42
#define LTR_C	0x43
#define LTR_D	0x44
#define LTR_E	0x45
#define LTR_F	0x46
#define LTR_G	0x47
#define LTR_H	0x48
#define LTR_I	0x49
#define LTR_J	0x4a
#define LTR_K	0x4b
#define LTR_L	0x4c
#define LTR_M	0x4d
#define LTR_N	0x4e
#define LTR_O	0x4f
#define LTR_P	0x50
#define LTR_Q	0x51
#define LTR_R	0x52
#define LTR_S	0x53
#define LTR_T	0x54
#define LTR_U	0x55
#define LTR_V	0x56
#define LTR_W	0x57
#define LTR_X	0x58
#define LTR_Y	0x59
#define LTR_Z	0x5a

#ifdef OLD

#define DUMPREGS(addr)		\
	set addr, %g1;	 	\
	st %i0, [%g1];		\
    	add %g1, 0x4, %g1; 	\
	st %i1, [%g1];		\
    	add %g1, 0x4, %g1;	\
	st %i2, [%g1];		\
    	add %g1, 0x4, %g1; 	\
	st %i3, [%g1];		\
    	add %g1, 0x4, %g1; 	\
	st %i4, [%g1];		\
    	add %g1, 0x4, %g1; 	\
	st %i5, [%g1];		\
    	add %g1, 0x4, %g1; 	\
	st %i6, [%g1];		\
    	add %g1, 0x4, %g1; 	\
	st %i7, [%g1];		\
    	add %g1, 0x4, %g1; 	\
	st %l0, [%g1];		\
    	add %g1, 0x4, %g1; 	\
	st %l1, [%g1];		\
    	add %g1, 0x4, %g1; 	\
	st %l2, [%g1];		\
    	add %g1, 0x4, %g1; 	\
	st %l3, [%g1];		\
    	add %g1, 0x4, %g1; 	\
	st %l4, [%g1];		\
    	add %g1, 0x4, %g1; 	\
	st %l5, [%g1];		\
    	add %g1, 0x4, %g1; 	\
	st %l6, [%g1];		\
    	add %g1, 0x4, %g1; 	\
	st %l7, [%g1];		\
    	add %g1, 0x4, %g1; 	\
	st %o0, [%g1];		\
    	add %g1, 0x4, %g1; 	\
	st %o1, [%g1];		\
    	add %g1, 0x4, %g1; 	\
	st %o2, [%g1];		\
    	add %g1, 0x4, %g1; 	\
	st %o3, [%g1];		\
    	add %g1, 0x4, %g1; 	\
	st %o4, [%g1];		\
    	add %g1, 0x4, %g1; 	\
	st %o5, [%g1];		\
    	add %g1, 0x4, %g1; 	\
	st %o6, [%g1];		\
    	add %g1, 0x4, %g1; 	\
	st %o7, [%g1];	
    
#define SAVE_G_REGS(STRTADDR) \
        st      %g1, [STRTADDR + (1*4)]; \
        st      %g2, [STRTADDR + (2*4)]; \
        st      %g3, [STRTADDR + (3*4)]; \
        st      %g4, [STRTADDR + (4*4)]; \
        st      %g5, [STRTADDR + (5*4)]; \
        st      %g6, [STRTADDR + (6*4)]; \
        st      %g7, [STRTADDR + (7*4)]; \
        mov     %y, %g1; \
        st      %g1, [STRTADDR + (8*4)]

#define RESTORE_G_REGS(STRTADDR) \
        ld      [STRTADDR + (8*4)], %g1; \
        mov     %g1, %y; \
        ld      [STRTADDR + (1*4)], %g1; \
        ld      [STRTADDR + (2*4)], %g2; \
        ld      [STRTADDR + (3*4)], %g3; \
        ld      [STRTADDR + (4*4)], %g4; \
        ld      [STRTADDR + (5*4)], %g5; \
        ld      [STRTADDR + (6*4)], %g6; \
        ld      [STRTADDR + (7*4)], %g7;
 
#define SAVE_O_REGS(STRTADDR) \
        st      %i0, [STRTADDR + (0*4)]; \
        st      %i1, [STRTADDR + (1*4)]; \
        st      %i2, [STRTADDR + (2*4)]; \
        st      %i3, [STRTADDR + (3*4)]; \
        st      %i4, [STRTADDR + (4*4)]; \
        st      %i5, [STRTADDR + (5*4)]; \
        st      %i6, [STRTADDR + (6*4)]; \
        st      %i7, [STRTADDR + (7*4)];
 
#define RESTORE_O_REGS(STRTADDR) \
        ld      [STRTADDR + (0*4)], %i0; \
        ld      [STRTADDR + (1*4)], %i1; \
        ld      [STRTADDR + (2*4)], %i2; \
        ld      [STRTADDR + (3*4)], %i3; \
        ld      [STRTADDR + (4*4)], %i4; \
        ld      [STRTADDR + (5*4)], %i5; \
        ld      [STRTADDR + (6*4)], %i6; \
        ld      [STRTADDR + (7*4)], %i7;

#define SAVE_L_I_REGS(STRTADDR) \
	st	%l0, [STRTADDR + (0*4)]; \
	st	%l1, [STRTADDR + (1*4)]; \
	st	%l2, [STRTADDR + (2*4)]; \
	st	%l3, [STRTADDR + (3*4)]; \
	st	%l4, [STRTADDR + (4*4)]; \
	st	%l5, [STRTADDR + (5*4)]; \
	st	%l6, [STRTADDR + (6*4)]; \
	st	%l7, [STRTADDR + (7*4)]; \
	st	%i0, [STRTADDR + (8*4)]; \
	st	%i1, [STRTADDR + (9*4)]; \
	st	%i2, [STRTADDR + (10*4)]; \
	st	%i3, [STRTADDR + (11*4)]; \
	st	%i4, [STRTADDR + (12*4)]; \
	st	%i5, [STRTADDR + (13*4)]; \
	st	%i6, [STRTADDR + (14*4)]; \
	st	%i7, [STRTADDR + (15*4)]

#define RESTORE_L_I_REGS(STRTADDR) \
	ld	[STRTADDR + (0*4)], %l0; \
	ld	[STRTADDR + (1*4)], %l1; \
	ld	[STRTADDR + (2*4)], %l2; \
	ld	[STRTADDR + (3*4)], %l3; \
	ld	[STRTADDR + (4*4)], %l4; \
	ld	[STRTADDR + (5*4)], %l5; \
	ld	[STRTADDR + (6*4)], %l6; \
	ld	[STRTADDR + (7*4)], %l7; \
	ld	[STRTADDR + (8*4)], %i0; \
	ld	[STRTADDR + (9*4)], %i1; \
	ld	[STRTADDR + (10*4)], %i2; \
	ld	[STRTADDR + (11*4)], %i3; \
	ld	[STRTADDR + (12*4)], %i4; \
	ld	[STRTADDR + (13*4)], %i5; \
	ld	[STRTADDR + (14*4)], %i6; \
	ld	[STRTADDR + (15*4)], %i7

/*---------------------------------------------------------------------------
 * macros to save/restore register windows using double word copies
 *---------------------------------------------------------------------------
 */

#define SAVE_DBL_G_REGS(STRTADDR) \
	st	%g1, [STRTADDR + (1*4)]; \
	std	%g2, [STRTADDR + (2*4)]; \
	std	%g4, [STRTADDR + (4*4)]; \
	std	%g6, [STRTADDR + (6*4)]; \
	mov	%y, %g1; \
	st	%g1, [STRTADDR + (8*4)]

#define RESTORE_DBL_G_REGS(STRTADDR) \
	ld	[STRTADDR + (8*4)], %g1; \
	mov	%g1, %y; \
	ld	[STRTADDR + (1*4)], %g1; \
	ldd	[STRTADDR + (2*4)], %g2; \
	ldd	[STRTADDR + (4*4)], %g4; \
	ldd	[STRTADDR + (6*4)], %g6;

#define SAVE_DBL_OREGS(STRTADDR) \
	std	%i0, [STRTADDR + (0*4)]; \
	std	%i2, [STRTADDR + (2*4)]; \
	std	%i4, [STRTADDR + (4*4)]; \
	std	%i6, [STRTADDR + (6*4)];

#define RESTORE_DBL_O_REGS(STRTADDR) \
	ldd	[STRTADDR + (0*4)], %i0; \
	ldd	[STRTADDR + (2*4)], %i2; \
	ldd	[STRTADDR + (4*4)], %i4; \
	ldd	[STRTADDR + (6*4)], %i6;

#define SAVE_DBL_L_I_REGS(STRTADDR) \
	std	%l0, [STRTADDR + (0*4)]; \
	std	%l2, [STRTADDR + (2*4)]; \
	std	%l4, [STRTADDR + (4*4)]; \
	std	%l6, [STRTADDR + (6*4)]; \
	std	%i0, [STRTADDR + (8*4)]; \
	std	%i2, [STRTADDR + (10*4)]; \
	std	%i4, [STRTADDR + (12*4)]; \
	std	%i6, [STRTADDR + (14*4)];

#define RESTORE_DBL_L_I_REGS(STRTADDR) \
	ldd	[STRTADDR + (0*4)], %l0; \
	ldd	[STRTADDR + (2*4)], %l2; \
	ldd	[STRTADDR + (4*4)], %l4; \
	ldd	[STRTADDR + (6*4)], %l6; \
	ldd	[STRTADDR + (8*4)], %i0; \
	ldd	[STRTADDR + (10*4)], %i2; \
	ldd	[STRTADDR + (12*4)], %i4; \
	ldd	[STRTADDR + (14*4)], %i6;


/*
 * Macros for saving/restoring registers.
 */

#ifdef NODOUBLES

#define SAVE_GLOBALS(RP) \
        st      %g1, [RP + G1*4]; \
        st      %g2, [RP + G2*4]; \
        st      %g3, [RP + G3*4]; \
        st      %g4, [RP + G4*4]; \
        st      %g5, [RP + G5*4]; \
        st      %g6, [RP + G6*4]; \
        st      %g7, [RP + G7*4]; \
        mov     %y, %g1; \
        st      %g1, [RP + Y*4]

#define RESTORE_GLOBALS(RP) \
        ld      [RP + Y*4], %g1; \
        mov     %g1, %y; \
        ld      [RP + G1*4], %g1; \
        ld      [RP + G2*4], %g2; \
        ld      [RP + G3*4], %g3; \
        ld      [RP + G4*4], %g4; \
        ld      [RP + G5*4], %g5; \
        ld      [RP + G6*4], %g6; \
        ld      [RP + G7*4], %g7;
 
#define SAVE_OUTS(RP) \
        st      %i0, [RP + O0*4]; \
        st      %i1, [RP + O1*4]; \
        st      %i2, [RP + O2*4]; \
        st      %i3, [RP + O3*4]; \
        st      %i4, [RP + O4*4]; \
        st      %i5, [RP + O5*4]; \
        st      %i6, [RP + O6*4]; \
        st      %i7, [RP + O7*4];
 
#define RESTORE_OUTS(RP) \
        ld      [RP + O0*4], %i0; \
        ld      [RP + O1*4], %i1; \
        ld      [RP + O2*4], %i2; \
        ld      [RP + O3*4], %i3; \
        ld      [RP + O4*4], %i4; \
        ld      [RP + O5*4], %i5; \
        ld      [RP + O6*4], %i6; \
        ld      [RP + O7*4], %i7;

#define SAVE_WINDOW(SBP) \
	st	%l0, [SBP + (0*4)]; \
	st	%l1, [SBP + (1*4)]; \
	st	%l2, [SBP + (2*4)]; \
	st	%l3, [SBP + (3*4)]; \
	st	%l4, [SBP + (4*4)]; \
	st	%l5, [SBP + (5*4)]; \
	st	%l6, [SBP + (6*4)]; \
	st	%l7, [SBP + (7*4)]; \
	st	%i0, [SBP + (8*4)]; \
	st	%i1, [SBP + (9*4)]; \
	st	%i2, [SBP + (10*4)]; \
	st	%i3, [SBP + (11*4)]; \
	st	%i4, [SBP + (12*4)]; \
	st	%i5, [SBP + (13*4)]; \
	st	%i6, [SBP + (14*4)]; \
	st	%i7, [SBP + (15*4)]

#define RESTORE_WINDOW(SBP) \
	ld	[SBP + (0*4)], %l0; \
	ld	[SBP + (1*4)], %l1; \
	ld	[SBP + (2*4)], %l2; \
	ld	[SBP + (3*4)], %l3; \
	ld	[SBP + (4*4)], %l4; \
	ld	[SBP + (5*4)], %l5; \
	ld	[SBP + (6*4)], %l6; \
	ld	[SBP + (7*4)], %l7; \
	ld	[SBP + (8*4)], %i0; \
	ld	[SBP + (9*4)], %i1; \
	ld	[SBP + (10*4)], %i2; \
	ld	[SBP + (11*4)], %i3; \
	ld	[SBP + (12*4)], %i4; \
	ld	[SBP + (13*4)], %i5; \
	ld	[SBP + (14*4)], %i6; \
	ld	[SBP + (15*4)], %i7

#else NODOUBLES

#define SAVE_GLOBALS(RP) \
	st	%g1, [RP + G1*4]; \
	std	%g2, [RP + G2*4]; \
	std	%g4, [RP + G4*4]; \
	std	%g6, [RP + G6*4]; \
	mov	%y, %g1; \
	st	%g1, [RP + Y*4]

#define RESTORE_GLOBALS(RP) \
	ld	[RP + Y*4], %g1; \
	mov	%g1, %y; \
	ld	[RP + G1*4], %g1; \
	ldd	[RP + G2*4], %g2; \
	ldd	[RP + G4*4], %g4; \
	ldd	[RP + G6*4], %g6;

#define SAVE_OUTS(RP) \
	std	%i0, [RP + O0*4]; \
	std	%i2, [RP + O2*4]; \
	std	%i4, [RP + O4*4]; \
	std	%i6, [RP + O6*4];

#define RESTORE_OUTS(RP) \
	ldd	[RP + O0*4], %i0; \
	ldd	[RP + O2*4], %i2; \
	ldd	[RP + O4*4], %i4; \
	ldd	[RP + O6*4], %i6;

#define SAVE_WINDOW(SBP) \
	std	%l0, [SBP + (0*4)]; \
	std	%l2, [SBP + (2*4)]; \
	std	%l4, [SBP + (4*4)]; \
	std	%l6, [SBP + (6*4)]; \
	std	%i0, [SBP + (8*4)]; \
	std	%i2, [SBP + (10*4)]; \
	std	%i4, [SBP + (12*4)]; \
	std	%i6, [SBP + (14*4)];

#define RESTORE_WINDOW(SBP) \
	ldd	[SBP + (0*4)], %l0; \
	ldd	[SBP + (2*4)], %l2; \
	ldd	[SBP + (4*4)], %l4; \
	ldd	[SBP + (6*4)], %l6; \
	ldd	[SBP + (8*4)], %i0; \
	ldd	[SBP + (10*4)], %i2; \
	ldd	[SBP + (12*4)], %i4; \
	ldd	[SBP + (14*4)], %i6;

#endif NODOUBLES

#endif	/* OLD */


