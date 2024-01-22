	.text
	.align	1
	.globl	_stdump_getsp
_stdump_getsp:
	.word	0
	addl3	$76,sp,r0
	
	ret
