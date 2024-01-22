
/* 
 * asl-machine-dependent.h - architecture specific information
 *			   - note: this may be included in .s file
 *			     so put structs in #ifndef ASM
 */

#if defined (SPARC_SLC) || defined (SPARC_IPC)
#define V_ROMP          0xffe80010      /* address of romp              */
#define V_PRINTF        0xffe80078      /* address of romp->v_printf    */
#define V_PUTCHAR       0xffe80064      /* address of romp->v_putchar   */
#define V_RETMON        0xffe80084      /* address of romp->v_exit_to_mon */
#define V_PHYSMEM_LIST  0xffe80028      /* address of romp->v_availmemory */
#endif

#if defined (SPARC_ELC)
#define V_ROMP          0xffe8f3e0      /* address of romp              */
#define V_PRINTF        0xffe8f448      /* address of romp->v_printf    */
#define V_PUTCHAR       0xffe8f434      /* address of romp->v_putchar   */
#define V_RETMON        0xffe8f454      /* address of romp->v_exit_to_mon */
#define V_PHYSMEM_LIST  0xffe8f3f8      /* address of romp->v_availmemory */
#endif


#define kprintf1(sb)		\
	set	sb, %o0;	\
	set	V_PRINTF, %g1;	\
	ld	[%g1], %g2;	\
	call	%g2;		\
	nop

#define kprintf2(sb1, sb2)	\
	set	sb2, %o1;	\
	set	sb1, %o0;	\
	set	V_PRINTF, %g1;	\
	ld	[%g1], %g2;	\
	call	%g2;		\
	nop			

#define kputc1(c)		\
    	set	c, %o0;		\
    	set	V_PUTCHAR, %g1;	\
    	ld	[%g1],%g2;	\
    	call	%g2;		\
    	nop


#define HALT	\
	set	V_RETMON, %g1;	\
	ld	[%g1], %g2;	\
	call	%g2;		\
	nop

#define knewline()		\
	kputc1(0xa);		\
	kputc1(0xd)


