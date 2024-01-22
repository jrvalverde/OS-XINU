!----------------------------------------------------------------------------
!  systrap -- issue a Change to Kernel mode instruction
!----------------------------------------------------------------------------
!

/*#define DEBUG*/

#include <sys/asl.h>			/* xinu header file */
#include <sys/syscall.h>		/* xinu header file */
#include <sys/user_traps.h>		/* xinu header file */

	.seg	"text"
	.proc 04
	PROCEDURE(systrap)

	save %sp, -SPARCMINFRAME, %sp	! increment CWP
	
#ifdef DEBUG
	KPUTC(83, %l3, %l6)
	KPUTC(67, %l3, %l6)
	KPUTC(10, %l3, %l6)
	KPUTC(13, %l3, %l6)
#endif

	ta	U_TRAP_SYSCALL	! issue a trap (ticc) instruction
	nop; nop; nop

#ifdef DEBUG
	KPUTC(67, %l3, %l6)
	KPUTC(83, %l3, %l6)
	KPUTC(10, %l3, %l6)
	KPUTC(13, %l3, %l6)
#endif

	ret				! return
	restore				! decrement CWP


