!---------------------------------------------------------------------------
! startup.s - for xinu on the sparc architecture
!---------------------------------------------------------------------------


/*#define DEBUG*/

!---------------------------------------------------------------------------
!			Header/Include Section
!---------------------------------------------------------------------------


#include <asl.h>	! handy sparc assembler macros - I wrote this - jng
#include <asl-machine-dependent.h> ! machine dependent header info
#include <psl.h>	! This is unix - should be xinu-fied sometime


! initial stack space
#define INITIALSTACKSIZE 	8192	/* 8 K */
! initial window stack space
#define INITSTKWINSIZE		96


!---------------------------------------------------------------------------
!			Data Section
!---------------------------------------------------------------------------

	!
	! Reserve space for global data structures
	!
	.seg	"data"
	.align	4


!	.global	_cpudelay		! _cpudelay is used in the DELAY macro
!_cpudelay:
!	.word	5			! we will set it to 5 for the moment 

idp:
	.word	0,0,0,0,0,0,0,0		! room for idprom
	.word	0,0,0,0,0,0,0,0
	.word	0,0,0,0,0,0,0,0
	.word	0,0,0,0,0,0,0,0

	.align	8
	.global _stacktop		! pointer to temporary stack
_initstack:
	.skip INITIALSTACKSIZE		! reserve some initial stack space
_stacktop:
/*_filler: .skip 512*/

	.align	4
	.global _romp			! pointer to ROM structure
_romp: 
	.word 0

#ifdef DEBUG
	!
	! various bogus messages like "Hello World"
	!
msg1:
	.asciz	"******** Xinu is alive! ************"
msg2:
	.asciz	"Basic Stack set-up complete"
msg3:
	.asciz	"Done zeroing the data region"
msg4:
	.asciz	"Done with startup.s - calling nulluser()"
#endif

!---------------------------------------------------------------------------
!			Text Section
!---------------------------------------------------------------------------

	!
	! Startup.s - start code begins here
	!
	.seg	"text"
	.align	4

	.global	_start		! where xinu starts
	.global	_k_traptable	! kernel trap table base register
	.global	_u_traptable	! user   trap table base register

	.global	_real_start	! where the xinu code really starts
	.global _restart	! point to jump to to restart xinu

	.global	_end		! end of the text area
	.global	_edata		! end of the data area

	.global	_main		! the address of main
	.global	__exit		! exit

/*
 * Since we havent figured out a better way to do it, we load the trap
 * tables as the first thing in our a.out file. This places them at 0x4000
 * and 0x5000 and guarantees the proper alignment for us. Unfortunately, the
 * BOOT prom wants 0x4000 to be the entry point. We handle this by
 * having the reset vector branch to our "real" entry point.
 */

_start:

_k_traptable:
#include <../sys/archdep/k_traptable.s>
k_traptableEnd:

_u_traptable:
#include <../sys/archdep/u_traptable.s>
u_traptableEnd:

	!
	! This is where things *really* start happening.
	!
_real_start:
	mov     %o0, %g3                ! holds the address of romp but
                                        ! we can't save it for real
                                        ! until after we map ourselves
                                        ! in high address.

_restart:
	set	PSR_S|PSR_PIL|PSR_ET, %g1	! supervisor mode,
						! enable traps,
						! disable interrupts
	mov	%g1, %psr
	nop; nop; nop			! wait for %psr to take effect

	!
	! setup wim mask(0x02) and cwp(0x0)
	!

	mov	0x0, %wim
	nop; nop; nop

	set	PSR_CWP, %g2
1:    	mov	%psr, %g1
	and	%g2, %g1, %g1
    	cmp	%g1, %g0
    	be	2f
	nop
	save	%g0,%g0,%g0
	ba	1b
	nop
2:
	mov	0x01, %wim		! cwp == 0x0, wim == 0x01
	nop; nop; nop;

	!
	! map high address space so we can jump to it
	!	(at least map context 0, do others later)
	! SetSegMap(VM_KERNEL_OFFSET,0);
	set	0xfd00000,%o0
	mov	0,%o1
	call	_set_smeg,2
	nop

	! SetSegMap(VM_KERNEL_OFFSET+SEGSIZ,1);
	set	0xfd40000,%o0
	mov	0x1,%o1
	call	_set_smeg,2
	nop

	! SetSegMap(VM_KERNEL_OFFSET+SEGSIZ+SEGSIZ,2);
	set	0xfd80000,%o0
	mov	0x2,%o1
	call	_set_smeg,2
	nop

	! SetSegMap(VM_KERNEL_OFFSET+SEGSIZ+SEGSIZ+SEGSIZ,3);
	set	0xfdc0000,%o0
	mov	0x3,%o1
	call	_set_smeg,2
	nop

	! SetSegMap(VM_KERNEL_OFFSET+SEGSIZ * 4, 4);
	set	0xfe00000,%o0
	mov	0x4,%o1
	call	_set_smeg,2
	nop

	!
	! Set the number of register windows _nwindows
	!    - this fills in _nwindows which is in high memory
	!      so don't call set_nwindows until you have mapped high mem
	!
	call	_set_nwindows
	nop

	!
	! Now jump into high memory
	!		(not necessary for version 7, but do it anyway)
	set	gobegin, %g1
	jmp	%g1
	nop

gobegin:
	!
	! At this point the kernel has been mapped in to high memory
	! and we are executing out of high memory
	!

	!
	! Temporarily run off a small temporary stack.
	! Leave space on the in the stack to store registers
	!
	set	_stacktop, %sp		! %sp <- _stacktop
	set	INITSTKWINSIZE, %g1
	sub	%sp,%g1,%sp

	!
	! Set initial frame pointer
	!
	mov	%g0,%fp			! fix pm trace (FP)


	!
	! setup kernel trap table (tbr)
	!
	set	_k_traptable, %g1
	mov	%g1, %tbr		! %tbr <- _k_trabtable
	nop;nop;nop

#ifdef DEBUG
kprintf1(msg1)
knewline()
#endif

	!
	! set _romp so that C can access the prom
     	! (g3 holds the address passed by the rom)
     	!
	set	_romp, %g2
	st	%g3,[%g2]
     
#ifdef DEBUG
kprintf1(msg2)
knewline()
#endif
	!
	! Zero out the bss region  (assume _edata and _end are word aligned)
	!
	set	_edata, %g1		! zero from _edata to _end
	set	_end, %g2
	cmp	%g1,%g2
	bgeu	2f
	nop
!	stb	%g0,[%g1]		! use if NOT word aligned
	st	%g0,[%g1]		! use if word aligned
1:					! tight loop to zero region
!	inc	%g1			! use if NOT word aligned
	add	%g1,0x4,%g1		! use if word aligned
	cmp	%g1,%g2
	bcs,a	1b
	stb	%g0,[%g1]
2:
#ifdef DEBUG
kprintf1(msg3)
knewline()
#endif
     
#define      INTREG_ADDR     0xFFFFA000

	! check what's in the INTREG
	!set 	INTREG_ADDR, %o0
	!ldub	[%o0], %o0

#ifdef DEBUG
kprintf1(msg4)
knewline()
#endif
	! start Xinu
	clr	%o0			! argv = 0 for now.
	clr	%o1			! argc = 0 for now.
	call	_nulluser
	nop

__exit:
	ret				! ret to prom
	restore


