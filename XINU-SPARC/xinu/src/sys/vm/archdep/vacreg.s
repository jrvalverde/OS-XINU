/* vacreg.s - routines to manipulate the Virtual Address Cache on the SPARC */
/*		- see also vac.c */


#include <asl.h>
#include <vmem.h>

#define TMP_SAVE_GLOBALS			\
	set _vac_sav_g1, %o1; st %g1, [%o1];	\
	set _vac_sav_g2, %o1; st %g2, [%o1];	\
	set _vac_sav_g3, %o1; st %g3, [%o1];	\
	set _vac_sav_g4, %o1; st %g4, [%o1];	\
	set _vac_sav_g5, %o1; st %g5, [%o1];	\
	set _vac_sav_g6, %o1; st %g6, [%o1];	\
	set _vac_sav_g7, %o1; st %g7, [%o1]

#define TMP_RESTORE_GLOBALS			\
	set _vac_sav_g1, %o1; ld [%o1], %g1;	\
	set _vac_sav_g2, %o1; ld [%o1], %g2;	\
	set _vac_sav_g3, %o1; ld [%o1], %g3;	\
	set _vac_sav_g4, %o1; ld [%o1], %g4;	\
	set _vac_sav_g5, %o1; ld [%o1], %g5;	\
	set _vac_sav_g6, %o1; ld [%o1], %g6;	\
	set _vac_sav_g7, %o1; ld [%o1], %g7

/*
 * Initialize the cache by invalidating all the cache tags.
 * DOES NOT turn on cache enable bit in the enable register.
 *
 * void vac_clearTags()
 *
 * taken from /usr/src/sun4.1/sys/sun4c/map.s vac_flushall()
 */
	PROCEDURE(vac_clearTags)
	set	VAC_SIZE, %o1		! cache size
	set	VAC_LINESIZE, %o2	! line size
	set	CACHE_TAGS, %o3		! address of cache tags in CTL space

1:	subcc	%o1, %o2, %o1		! done yet?
	bg	1b
	sta	%g0, [ %o3+%o1 ]ASI_CTL ! clear tag

	retl
	nop

/*
 * Flush a context from the cache.
 * To flush a context we must cycle through all lines of the
 * cache issuing a store into alternate space command for each
 * line whilst the context register remains constant.
 *
 * void vac_flushContext()
 */
PROCEDURE(vac_flushContext)
	TMP_SAVE_GLOBALS
	set	VAC_SIZE, %o1		! cache size
	set	VAC_LINESIZE, %o2	! line size
	add	%o2, %o2, %o3		! LS * 2
	add	%o2, %o3, %o4		! LS * 3
	add	%o2, %o4, %o5		! LS * 4
	add	%o2, %o5, %g1		! LS * 5
	add	%o2, %g1, %g2		! LS * 6
	add	%o2, %g2, %g3		! LS * 7
	add	%o2, %g3, %g4		! LS * 8

1:	subcc	%o1, %g4, %o1
	sta	%g0, [%o1 + %g0]ASI_FCC
	sta	%g0, [%o1 + %o2]ASI_FCC
	sta	%g0, [%o1 + %o3]ASI_FCC
	sta	%g0, [%o1 + %o4]ASI_FCC
	sta	%g0, [%o1 + %o5]ASI_FCC
	sta	%g0, [%o1 + %g1]ASI_FCC
	sta	%g0, [%o1 + %g2]ASI_FCC
	bg	1b
	sta	%g0, [%o1 + %g3]ASI_FCC

	TMP_RESTORE_GLOBALS
	retl
	nop

/*
 * Flush a PMEG from the cache.
 * To flush the argument PMEG from the cache we hold the bits that
 * specify the PMEG in the address constant and issue a store into
 * alternate space command for each line.
 *
 * void vac_flushPMEG( void * vaddr )
 */
PROCEDURE(vac_flushPMEG)
	TMP_SAVE_GLOBALS
	! get segment part
	srl	%o0,LOG2_PGSIZ+LOG2_VM_PGS_PER_SEG,%o0
	sll	%o0,LOG2_PGSIZ+LOG2_VM_PGS_PER_SEG,%o0
	set	VAC_SIZE, %o1		! cache size
	set	VAC_LINESIZE, %o2	! line size
	add	%o2, %o2, %o3		! LS * 2
	add	%o2, %o3, %o4		! LS * 3
	add	%o2, %o4, %o5		! LS * 4
	add	%o2, %o5, %g1		! LS * 5
	add	%o2, %g1, %g2		! LS * 6
	add	%o2, %g2, %g3		! LS * 7
	add	%o2, %g3, %g4		! LS * 8

1:	subcc	%o1, %g4, %o1
	sta	%g0, [%o0 + %g0]ASI_FCS
	sta	%g0, [%o0 + %o2]ASI_FCS
	sta	%g0, [%o0 + %o3]ASI_FCS
	sta	%g0, [%o0 + %o4]ASI_FCS
	sta	%g0, [%o0 + %o5]ASI_FCS
	sta	%g0, [%o0 + %g1]ASI_FCS
	sta	%g0, [%o0 + %g2]ASI_FCS
	sta	%g0, [%o0 + %g3]ASI_FCS
	bg	1b
	add	%o0, %g4, %o0

	TMP_RESTORE_GLOBALS
	retl
	nop

/*
 * Flush a page from the cache.
 * To flush the page containing the argument virtual address from
 * the cache we hold the bits that specify the page constant and
 * issue a store into alternate space command for each line.
 *
 * void vac_flushPage( void * vaddr )
 */
PROCEDURE(vac_flushPage)
	TMP_SAVE_GLOBALS
	! get segment part
	srl	%o0, LOG2_PGSIZ, %o0
	sll	%o0, LOG2_PGSIZ, %o0
	set	PGSIZ, %o1		! page size
	set	VAC_LINESIZE, %o2	! line size
	add	%o2, %o2, %o3		! LS * 2
	add	%o2, %o3, %o4		! LS * 3
	add	%o2, %o4, %o5		! LS * 4
	add	%o2, %o5, %g1		! LS * 5
	add	%o2, %g1, %g2		! LS * 6
	add	%o2, %g2, %g3		! LS * 7
	add	%o2, %g3, %g4		! LS * 8

1:	subcc	%o1, %g4, %o1
	sta	%g0, [%o0 + %g0]ASI_FCP
	sta	%g0, [%o0 + %o2]ASI_FCP
	sta	%g0, [%o0 + %o3]ASI_FCP
	sta	%g0, [%o0 + %o4]ASI_FCP
	sta	%g0, [%o0 + %o5]ASI_FCP
	sta	%g0, [%o0 + %g1]ASI_FCP
	sta	%g0, [%o0 + %g2]ASI_FCP
	sta	%g0, [%o0 + %g3]ASI_FCP
	bg	1b
	add	%o0, %g4, %o0

	TMP_RESTORE_GLOBALS
	retl
	nop

/*
 * Flush a range of addresses.
 * void vac_flush( void * vaddr, unsigned int nbytes )
 */
PROCEDURE(vac_flush)
	TMP_SAVE_GLOBALS
	set	VAC_SIZE, %o3		! cache size
	set	VAC_LINESIZE, %o2	! line size
	sub	%o2, 1, %o4		! convert to mask (assumes power of 2 )
	add	%o0, %o1, %o1		! add start to length
	andn	%o0, %o4, %o0		! round down start
	add	%o4, %o1, %o1		! round up end
	andn	%o1, %o4, %o1		! and mask off
	sub	%o1, %o0, %o1		! and subtract start
	cmp	%o1, %o3		! if (nbytes > vac_size)
	bgu,a	1f			! ...
	mov	%o3, %o1		! 	nbytes = vac_size

#ifdef OLD_WAY

1:	subcc	%o1, %o2, %o1
	bg	1b
	sta	%g0, [%o0 + %o1]ASI_FCP

#else

	add	%o2, %o2, %o3		! LS * 2
	add	%o2, %o3, %o4		! LS * 3
	add	%o2, %o4, %o5		! LS * 4
	add	%o2, %o5, %g1		! LS * 5
	add	%o2, %g1, %g2		! LS * 6
	add	%o2, %g2, %g3		! LS * 7
	add	%o2, %g3, %g4		! LS * 8

1:	subcc	%o1, %g4, %o1
	sta	%g0, [%o0 + %g0]ASI_FCP
	sta	%g0, [%o0 + %o2]ASI_FCP
	sta	%g0, [%o0 + %o3]ASI_FCP
	sta	%g0, [%o0 + %o4]ASI_FCP
	sta	%g0, [%o0 + %o5]ASI_FCP
	sta	%g0, [%o0 + %g1]ASI_FCP
	sta	%g0, [%o0 + %g2]ASI_FCP
	sta	%g0, [%o0 + %g3]ASI_FCP
	bg	1b
	add	%o0, %g4, %o0

#endif

	TMP_RESTORE_GLOBALS
	retl
	nop

