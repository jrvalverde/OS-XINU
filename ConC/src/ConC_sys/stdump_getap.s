	.text
	.align	1
	.globl	_stdump_getap
_stdump_getap:
	.word	0
	movl	52(sp),r0
	
	ret
