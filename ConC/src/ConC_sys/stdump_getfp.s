	.text
	.align	1
	.globl	_stdump_getfp
_stdump_getfp:
	.word	0
	movl	56(sp),r0
	
	ret
