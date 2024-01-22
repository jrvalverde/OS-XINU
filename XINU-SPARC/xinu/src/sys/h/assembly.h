/*#define DOUBLES*/

/*
 * Processor Status Register Mask
 */
#define PSR_INT_LEVEL   0x00000F00      /* take interrupts of 15 or higher */
#define PSR_CWP         0x0000001F      /* current window pointer */
#define PSR_ET          0x00000020      /* enable traps */
#define PSR_PS          0x00000040      /* previous supervisor mode */
#define PSR_S           0x00000080      /* supervisor mode */
#define PSR_PIL         0x00000F00      /* processor interrupt level */
#define PSR_EF          0x00001000      /* enable floating point unit */
#define PSR_EC          0x00002000      /* enable coprocessor */
#define PSR_RSV         0x000FC000      /* reserved */
#define PSR_ICC         0x00F00000      /* integer condition codes */
#define PSR_C           0x00100000      /* carry bit */
#define PSR_V           0x00200000      /* overflow bit */
#define PSR_Z           0x00400000      /* zero bit */
#define PSR_N           0x00800000      /* negative bit */
#define PSR_VER         0x0F000000      /* mask version */
#define PSR_IMPL        0xF0000000      /* implementation */
#define PSR_INT_CLKONLY 0x00000E00      

/*
 * Address space identifiers.
 */
#define ASI_CTL         2       /* control space */
#define ASI_PMEG        3       /* pmeg entry */
#define ASI_PTE         4       /* page table entry */
#define ASI_FCS_HW      5       /* flush cache segment (HW assisted) */
#define ASI_FCP_HW      6       /* flush cache page (HW assisted) */
#define ASI_FCC_HW      7       /* flush cache context (HW assisted) */
#define ASI_UP          8       /* user program */
#define ASI_SP          9       /* supervisor program */
#define ASI_UD          10      /* user data */
#define ASI_SD          11      /* supervisor data */
#define ASI_FCS         12      /* flush cache segment */
#define ASI_FCP         13      /* flush cache page */
#define ASI_FCC         14      /* flush cache context */
#define ASI_FCU_HW      15      /* flush cache unconditional (HW assisted) */

#define UARTABASE 0xf0000004
/*
 * ASI_CTL addresses
 */
#define CONTEXT_REG             0x30000000


#define SPARCMINFRAME	96

#define MYPUTC(ch, rch, rx)		\
        set	UARTABASE, rx;		\
        lduba   [rx]ASI_CTL, rx;	\
        andcc   rx, 0x04, %g0;		\
        be      .-16;			\
        nop;				\
					\
        set     UARTABASE+2, rx;	\
	mov	ch,rch;			\
        stba    rch, [rx]ASI_CTL;	\
					\
        set	UARTABASE, rx;		\
        lduba   [rx]ASI_CTL, rx;	\
        andcc   rx, 0x04, %g0;		\
        be      .-16;			\
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

/*
 * This code sequence has to be exactly 4 *real* instructions long.
 */
#define TRAP(addr)			\
        sethi	%hi(addr), %l4;		\
        jmp	%l4 + %lo(addr);	\
        mov	%psr, %l0;		\
	nop

#define GENERIC_TRAP() TRAP(_genericTrapHandler)

/*
 * These only work if you can guarantee that your stacks stay aligned
 * on 8 byte boundaries.
 */
#ifdef DOUBLES

#define SAVE_LOCALS_TO_FRAME(SP) \
        std      %l0, [SP + (0*4)]; \
        std      %l2, [SP + (2*4)]; \
        std      %l4, [SP + (4*4)]; \
        std      %l6, [SP + (6*4)]

#define SAVE_INS_TO_FRAME(SP) \
        std      %i0, [SP + (8*4)]; \
        std      %i2, [SP + (10*4)]; \
        std      %i4, [SP + (12*4)]; \
        std      %i6, [SP + (14*4)]

#define RESTORE_LOCALS_FROM_FRAME(SP) \
        ldd     [SP + (0*4)], %l0; \
        ldd     [SP + (2*4)], %l2; \
        ldd     [SP + (4*4)], %l4; \
        ldd     [SP + (6*4)], %l6

#define RESTORE_INS_FROM_FRAME(SP) \
        ldd     [SP + (8*4)], %i0; \
        ldd     [SP + (10*4)], %i2; \
        ldd     [SP + (12*4)], %i4; \
        ldd     [SP + (14*4)], %i6

#define SAVE_GLOBALS_AND_Y(STRTADDR, RX)	\
        std	%g0, [STRTADDR + (0*4)];	\
        std	%g2, [STRTADDR + (2*4)];	\
        std	%g4, [STRTADDR + (4*4)];	\
        std	%g6, [STRTADDR + (6*4)];	\
        mov	%y, RX;				\
	st	RX, [STRTADDR + (0*4)]

#define RESTORE_GLOBALS_AND_Y(STRTADDR)		\
	ld	[STRTADDR + (0*4)], %g1;	\
        mov	%g1, %y;			\
        ldd	[STRTADDR + (0*4)], %g0;	\
        ldd	[STRTADDR + (2*4)], %g2;	\
        ldd	[STRTADDR + (4*4)], %g4;	\
        ldd	[STRTADDR + (6*4)], %g6

#else

#define SAVE_LOCALS_TO_FRAME(SP) \
        st      %l0, [SP + (0*4)]; \
        st      %l1, [SP + (1*4)]; \
        st      %l2, [SP + (2*4)]; \
        st      %l3, [SP + (3*4)]; \
        st      %l4, [SP + (4*4)]; \
        st      %l5, [SP + (5*4)]; \
        st      %l6, [SP + (6*4)]; \
        st      %l7, [SP + (7*4)]

#define SAVE_INS_TO_FRAME(SP) \
        st      %i0, [SP + (8*4)]; \
        st      %i1, [SP + (9*4)]; \
        st      %i2, [SP + (10*4)]; \
        st      %i3, [SP + (11*4)]; \
        st      %i4, [SP + (12*4)]; \
        st      %i5, [SP + (13*4)]; \
        st      %i6, [SP + (14*4)]; \
        st      %i7, [SP + (15*4)]

#define RESTORE_LOCALS_FROM_FRAME(SP) \
        ld     [SP + (0*4)], %l0; \
        ld     [SP + (1*4)], %l1; \
        ld     [SP + (2*4)], %l2; \
        ld     [SP + (3*4)], %l3; \
        ld     [SP + (4*4)], %l4; \
        ld     [SP + (5*4)], %l5; \
        ld     [SP + (6*4)], %l6; \
        ld     [SP + (7*4)], %l7

#define RESTORE_INS_FROM_FRAME(SP) \
        ld     [SP + (8*4)], %i0; \
        ld     [SP + (9*4)], %i1; \
        ld     [SP + (10*4)], %i2; \
        ld     [SP + (11*4)], %i3; \
        ld     [SP + (12*4)], %i4; \
        ld     [SP + (13*4)], %i5; \
        ld     [SP + (14*4)], %i6; \
        ld     [SP + (15*4)], %i7

#define SAVE_GLOBALS_AND_Y(STRTADDR, RX)	\
        st	%g0, [STRTADDR + (0*4)];	\
        st	%g1, [STRTADDR + (1*4)];	\
        st	%g2, [STRTADDR + (2*4)];	\
        st	%g3, [STRTADDR + (3*4)];	\
        st	%g4, [STRTADDR + (4*4)];	\
        st	%g5, [STRTADDR + (5*4)];	\
        st	%g6, [STRTADDR + (6*4)];	\
        st	%g7, [STRTADDR + (7*4)];	\
        mov	%y, RX;				\
	st	RX, [STRTADDR + (0*4)]

#define RESTORE_GLOBALS_AND_Y(STRTADDR)		\
	ld	[STRTADDR + (0*4)], %g1;	\
        mov	%g1, %y;			\
        ld	[STRTADDR + (0*4)], %g0;	\
        ld	[STRTADDR + (1*4)], %g1;	\
        ld	[STRTADDR + (2*4)], %g2;	\
        ld	[STRTADDR + (3*4)], %g3;	\
        ld	[STRTADDR + (4*4)], %g4;	\
        ld	[STRTADDR + (5*4)], %g5;	\
        ld	[STRTADDR + (6*4)], %g6;	\
        ld	[STRTADDR + (7*4)], %g7

#endif

!#define ROTATE_WIM_LEFT(RWIM,RX)			\
	!sethi	%hi(_nwindowsminus), RX;		\
	!ld	[RX + %lo(_nwindowsminus)], RX;		\
        !srl	RWIM, RX, RX;				\
        !sll	RWIM, 1, RWIM;				\
        !or	RWIM, RX, RWIM

!#define ROTATE_REAL_WIM_LEFT(RWIM,RX)			\
	!rd	%wim, RWIM;				\
	!sethi	%hi(_nwindowsminus), RX;		\
	!ld	[RX + %lo(_nwindowsminus)], RX;		\
        !srl	RWIM, RX, RX;				\
        !sll	RWIM, 1, RWIM;				\
        !wr	RWIM, RX, %wim

!#define ROTATE_WIM_RIGHT(RWIM,RX)			\
	!sethi	%hi(_nwindowsminus), RX;		\
	!ld	[RX + %lo(_nwindowsminus)], RX;	\
        !sll	RWIM, RX, RX;				\
        !srl	RWIM, 1, RWIM;				\
        !or	RWIM, RX, RWIM

!#define ROTATE_REAL_WIM_RIGHT(RWIM,RX)			\
	!rd	%wim, RWIM;				\
	!sethi	%hi(_nwindowsminus), RX;		\
	!ld	[RX + %lo(_nwindowsminus)], RX;	\
        !sll	RWIM, RX, RX;				\
        !srl	RWIM, 1, RWIM;				\
        !wr	RWIM, RX, %wim

#define GLOBAL_SAVE_SIZE 32
#define EXCEPTIONFRAME_SIZE 64
#define SPARCMINFRAME 96

/*
 * Trap Table Mask
 */
#define TBR_TIC_MASK            (0x00000FF0)
#define TBR_TIC_SHIFT           (0x04)

#define COUNTER_ADDR 0xffd06000
#define CNTR_LIMIT14 12

 /* Stack Frame Layout - known offsets */
#define	IREG_0_OFFSET		32	/* %i0 offset 			*/
#define	LREG_0_OFFSET		0	/* %l0 offset 			*/
#define HIDE_VAL_OFFSET		64	/* hidden parameter offset	*/
#define SAV_IREG_0_OFFSET	68	/* saved %i0 value's offset	*/
#define SAV_IREG_6_OFFSET	92	/* saved %i6 value's offset	*/
