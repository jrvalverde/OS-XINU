/* lowcore.s -- interrupt and exception vectors in low part of memory */

#include <procreg.h>
#include <conf.h>

/*------------------------------------------------------------------------
 *  absolute location 0 -- fixed interrupt and exception vectors
 *------------------------------------------------------------------------
*/
				/* declare external references		*/
	.globl	panic0
	.globl	panvec0
	.globl	panvec1
	.globl	panvec2
	.globl	panvec3
	.globl	panvec4
	.globl	panvec5
	.globl	panvec6
	.globl	panvec7
	.globl	panvec8
	.globl	panvec9
	.globl	panvec10
	.globl	panvec11
	.globl	panvec12
	.globl	panvec13
	.globl	panvec14
	.globl	panvec15
	.align	2		/* must align on longword boundary	*/
	.text			/* allow loader to place at beginning of*/
	.globl	lowcore		/* text segment				*/
lowcore:
	brw	pan0		/* on branch to loc0, run panic0	*/
	.align	2		/* align on longword boundary		*/
				/* location	exception		*/
	.long	panvec0+1	/* 	 4	machine check		*/
	.long	panvec1+1	/* 	 8	kernel stack not valid	*/
	.long	panvec2		/* 	 c	power fail		*/
	.long	panvec3		/* 	10	reserved instruction	*/
	.long	panvec3		/*	14	reserved instruction	*/
	.long	panvec14	/*	18	reserved operand	*/
	.long	panvec15	/*	1c	reserved addressing mode*/
	.long	panvec4		/*	20	access control violation*/
	.long	panvec4		/*	24	translation not valid	*/
	.long	panvec5		/*	28	trace pending		*/
	.long	panvec6		/*	2c	breakpoint instruction	*/
	.long	panvec7		/*	30	compatibility mode	*/
	.long	panvec8		/*	34	arithmetic trap		*/
	.long	panvec9		/*	38	unused vector		*/
	.long	panvec9		/*	3c	unused vector		*/
	.long	panvec13	/*	40	CHMK instruction trap	*/
	.long	panvec13	/*	44	CHME instruction trap	*/
	.long	panvec13	/*	48	CHMS instruction trap	*/
	.long	panvec13	/*	4c	CHMU instruction trap	*/
	.long	panvec9		/*	50	unused vector		*/
	.long	panvec10	/*	54	corrected memory read	*/
	.long	panvec9		/*	58	unused vector		*/
	.long	panvec9		/*	5c	unused vector		*/
	.long	panvec11	/*	60	write bus timeout	*/
	.long	panvec9		/*	64	unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*	bc	unused vector		*/
	.long	panvec12	/*	c0	interval timer		*/
	.long	panvec9		/*	c4	unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*		unused vector		*/
	.long	panvec9		/*	e8	unused vector		*/
	.long	panvec9		/*	ec	unused vector		*/
	.long	panvec12	/*	f0	unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*		unknown device		*/
	.long	panvec12	/*	3fc	unknown device		*/
pan0:
	jmp	panic0		/* on jump to location 0, run panic0	*/
