/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#ifdef SYSLIBC_SCCS
_sccsid:.asciz	"@(#)Ovfork.c	5.3 (Berkeley) 3/9/86"
#endif SYSLIBC_SCCS

/*
 * @(#)vfork.s	4.1 (Berkeley) 12/21/80
 * C library -- sys_vfork
 */

/*
 * pid = sys_vfork();
 *
 * r1 == 0 in parent process, r1 == 1 in child process.
 * r0 == pid of child in parent, r0 == pid of parent in child.
 *
 * trickery here, due to keith sklower, uses ret to clear the stack,
 * and then returns with a jump indirect, since only one person can return
 * with a ret off this stack... we do the ret before we sys_vfork!
 */

	.set	sys_vfork,66
.globl	_sys_vfork

_sys_vfork:
	.word	0x0000
	movl	16(fp),r2
	movab	here,16(fp)
	ret
here:
	chmk	$sys_vfork
	bcc	vforkok
	jmp	verror
vforkok:
	tstl	r1		# child process ?
	bneq	child	# yes
	bcc 	parent		# if c-bit not set, fork ok
.globl	_errno
verror:
	movl	r0,_errno
	mnegl	$1,r0
	jmp	(r2)
child:
	clrl	r0
parent:
	jmp	(r2)
