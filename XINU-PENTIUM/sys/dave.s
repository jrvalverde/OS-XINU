# 1 "startup.S"
 





































 






# 1 "/usr/include/linux/limits.h" 1 3

















# 46 "startup.S" 2

 




	.data
	.globl	_gdt
	.globl	_gdtr
	.globl	_idt
	.globl	_idtr
_gdt:	.space	64	# must equal NGD*8 (64 = 8 segments)
_gdtr:	.word	63	# sizeof _gdt -1 (in bytes)
	.long	_gdt
_idt:	.space	384	# must equal NID*8 (384 == 48 vectors)
_idtr:	.word	383	# size of _idt -1 (in bytes)
	.long	_idt

	.globl _cpudelay

_cpudelay:	.long	1
	.text
	.align 2
	.globl	_end
	.globl	_edata
	.globl	_nulluser
	.globl	_bootdev
	.globl	_cyloffset
	.globl	_lidt

	.globl	__zero			# artificial value for entry point
	.set	__zero,0

# 135 "startup.S"

_start:

	 

# 186 "startup.S"

	movl	%esp,%esi


	 


	movl	$64*1024,%ebx
	movl	$_end,%eax	# should be movl $_end-_edata but ...
	subl	$_edata,%eax
	addl	%ebx,%eax
	pushl	%eax
	pushl	$_edata
	call	_bzero

	 


	movl	%esi,%eax
	subl	%ebx,%eax
	subl	$5*4,%ebx
	pushl	%ebx
	pushl	%eax
	call	_bzero








	movl	%esi,%esp

	pushl	$0
	popf

	movl	%esp,%ebp
	call	_setsegs
	lgdt	_gdtr
	 



	ljmp	$0x8,$gdt1	 
gdt1:
	movl	$0x10,%eax	 
	movw	%ax,%ds
	movw	%ax,%es
	movl	$0x18,%eax	 
	movw	%ax,%ss
	movl	_initsp,%esp	 
	movl	%esp,%ebp

	call	_nulluser
	call	_halt
	jmp	1b

	.data
_bootdev:	.long	0
_cyloffset:	.long	0
savearea:	.long	0,0	# sp & bp to return to
	.text
	.globl _delay

	.globl	_inb
_inb:	movl	4(%esp),%edx
	xorl	%eax,%eax	# clr eax
	inb	%dx,%al
	ret

	.globl	_inw
_inw:	movl	4(%esp),%edx
	xorl	%eax,%eax	# clr eax
	inw	%dx,%ax
	ret

	.globl	_outb
_outb:	movl	4(%esp),%edx
	movl	8(%esp),%eax
	outb	%al,%dx
	ret

	.globl	_outw
_outw:	movl	4(%esp),%edx
	movl	8(%esp),%eax
	outw	%ax,%dx
	ret


	.globl	_rtcin
_rtcin:	movl	4(%esp),%eax
	outb	%al,$0x70
	subl	%eax,%eax	# clr eax
	inb	$0x71,%al
	ret


	.globl ___udivsi3
___udivsi3:
	movl 4(%esp),%eax
	xorl %edx,%edx
	divl 8(%esp)
	ret

	.globl ___divsi3
___divsi3:
	movl 4(%esp),%eax
	xorl %edx,%edx
	cltd
	idivl 8(%esp)
	ret

	
	#bzero (base,cnt)
	

	.globl _bzero
_bzero:
	pushl	%edi
	movl	8(%esp),%edi
	movl	12(%esp),%ecx
	movb	$0x00,%al
	cld
	rep
	stosb
	popl	%edi
	ret

	
	#bcopy(src, dst, count)
	

	.globl	_bcopy
_bcopy:
	pushl	%esi
	pushl	%edi
	movl	12(%esp),%esi
	movl	16(%esp),%edi
L1:
	movl	20(%esp),%ecx
	cld
	rep
	movsb
	popl	%edi
	popl	%esi
	ret

	#insw(port,addr,cnt)
	.globl	_insw
_insw:
	pushl	%edi
	movw	8(%esp),%dx
	movl	12(%esp),%edi
	movl	16(%esp),%ecx
	cld
	.byte 0x66,0xf2,0x6d	# rep insw
	movl	%edi,%eax
	popl	%edi
	ret

	#outsw(port,addr,cnt)
	.globl	_outsw
_outsw:
	pushl	%esi
	movw	8(%esp),%dx
	movl	12(%esp),%esi
	movl	16(%esp),%ecx
	cld
	.byte 0x66,0xf2,0x6f	# rep outsw
	movl	%esi,%eax
	popl	%esi
	ret

	
	#lidt() - load interrupt descriptor table from _idtr
	
_lidt:
	lidt	_idtr
	ret
