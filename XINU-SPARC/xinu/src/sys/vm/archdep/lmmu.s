/* lmmu.s - low-level routines to manipulate the mmu */

#include <asl.h>
#include <vmem.h>

	.seg	"text"
	.align  4
	
	!
	! get_context() - return the current mmu context
	!
	.proc 04
	PROCEDURE(get_context)
	clr	%o0
	set	CONTEXT_REG, %o1
	retl
	lduba	[%o1]ASI_CTL, %o0

	!
	! set_context(newctx) - set the mmu context
	!
	.proc 4
	PROCEDURE(set_context)
	set	CONTEXT_REG, %o1
	retl
	stba	%o0, [%o1]ASI_CTL

	!
	! int switch_context(unsigned int) - returns the current context,
        !     and switch to the context passed as argument.
	.proc 4
	PROCEDURE(switch_context)
	clr	%o2
	set	CONTEXT_REG, %o1
	lduba	[ %o1 ]ASI_CTL, %o2
	stba	%o0, [ %o1 ]ASI_CTL
	retl
	mov	%o2, %o0

	!
	! get_smeg(vaddr) - return the smeg entry at vaddr
	!
	.proc 4
	PROCEDURE(get_smeg)
	mov	%o0, %o1
	clr 	%o0
	retl
	lduba	[%o1]ASI_SM, %o0

	!
	! set_smeg(vaddr, val) - set smeg entry at vaddr to val
	!
	.proc 4
	PROCEDURE(set_smeg)
	retl
	stba	%o1, [%o0]ASI_SM

	!
	! get_pmeg(vaddr) - return the pmeg entry at vaddr
	!
	.proc 4
	PROCEDURE(get_pmeg) 
	mov 	%o0, %o1
	clr 	%o0
	retl
	lda	[%o1]ASI_PM, %o0


	!
	! set_pmeg(vaddr, val) - set pmeg entry at vaddr to val
	!
	.proc 4
	PROCEDURE(set_pmeg)
	retl
	sta	%o1, [%o0]ASI_PM


	!
	! GetPageFaultType() - get and then clear pg fault error reg
	!			- SYNC_ERROR_REG contains the type
	!			  of pg fault taken (e.g., r, w, or tlb miss)
	!
	.proc 4
	PROCEDURE(GetPageFaultType)
	set	SYNC_ERROR_REG, %o1
	retl
	lda	[ %o1 ]ASI_CTL, %o0	! get sync error reg (clears reg)


	!
	! GetPageFaultAddress() - return the vaddr that caused the pg fault
	! 
	.proc 4
	PROCEDURE(GetPageFaultAddress)
	set	SYNC_VA_REG, %o1
	retl
	lda	[ %o1 ]ASI_CTL, %o0


