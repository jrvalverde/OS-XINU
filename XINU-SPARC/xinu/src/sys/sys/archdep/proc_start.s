/* proc_start.s - start a newly created process */

/*#define DEBUG*/

#ifdef DEBUG
#include <asl-machine-dependent.h> ! machine dependent header info
#endif

#include <asl.h>
#include <asl-machine-dependent.h>
#include <vmem.h>

#define INIT_PS	0x0
#define PSR_S_BIT_MASK 0x00000080

	! 
	! proc_start - start newly create process, and kill it when its done
	!
	!	This procedure plays with the register windows and
	!	the stack to make sure the new process can find all
	!	of its parameters.  We assume that create() sets
	!	proctab[PC] = proc_start so that we will jump here
	!	from the context switch.  We also assume that create
	!	initialized the stack, and then context switch underflowed
	!	the stack values into the current %i0-%i7 and %l0-%l7.
	!	We assume %i0-%i5 contain the first six parameters
	!	for the new process.  We also assume that %l0 contains
	!	the new process's address.  We assume the userret() address
	!	is in %i7

	.seg	"data"
	.align	4
initps:	.word	INIT_PS

	.seg	"text"
	.proc 04
	PROCEDURE(proc_start)

	set	initps, %l3
	call	_restore	! enable interrupts for new process
	mov	%l3, %o0

	! Change to user mode if we are jumping to a user-level program
	! User-level programs have an entry point less than the kernel offset
	set	VM_KERNEL_OFFSET, %l3
	cmp	%l0, %l3
	bg	already_in_supervisor_mode
	nop

set_user_mode:
	!
	! switch to user trap tables
	!
	set	_u_traptable, %l3
	mov	%l3, %tbr			! %tbr <- _u_trabtable
	nop; nop; nop

	!
	! Set the Supervisor bit to 0
	!
	call	_change_to_user_mode
	nop
	
already_in_supervisor_mode:
	/* do nothing */
	
#ifdef DEBUG
	HALT
#endif

	/* passing arguments */
	mov	%i0, %o0	! copy ins to outs
	mov	%i1, %o1
	mov	%i2, %o2
	mov	%i3, %o3
	mov	%i4, %o4
	mov	%i5, %o5

	call	%l0		! call the user's function
	nop

	call	%i7		! this never returns
	nop

 	retl
	nop
