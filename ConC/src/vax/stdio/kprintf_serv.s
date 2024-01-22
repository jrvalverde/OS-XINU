	.text
	.align	1
	.globl	_kprintf_serv
_kprintf_serv:
	.word	07777

	pushl	$-1073741825
	calls	$1,_sys_sigsetmask
	movl	r0,ps

	addl3	(ap),ap,r11
	addl3	(ap),r11,r0
	movl	r0,r11
	addl3	(ap),r11,r0
	movl	r0,r11
	addl3	(ap),r11,r0
	movl	r0,r11
L1:
	cmpl	r11,ap
	jleq	L9
L2:
	pushl	(r11)
	subl3	$4,r11,r0
	movl	r0,r11
	jbr	L1
L9:
	pushl	$buf
	addl3	$1,(ap),r11
	calls	r11,_sprintf

	pushl	$buf
	calls	$1,_strlen
	pushl	r0
	pushl	$buf
	pushl	$1
	calls	$3,_sys_write

	pushl	ps
	calls	$1,_sys_sigsetmask

	ret
	.data	1
ps:
	.byte	1
	.byte	1
	.byte	1
	.byte	1

buf:	.ascii	"                                                  "
	.ascii	"                                                  "
	.ascii	"                                                  "
	.ascii	"                                                  "
	.ascii	"                                                  "
	.ascii	"                                                  "
	.ascii	"                                                  "
	.ascii	"                                                  "
	.ascii	"                                                  "
	.ascii	"                                                  "
	.ascii	"                                                  "
	.ascii	"                                                  "
	.ascii	"                                                  "
	.ascii	"                                                  "
	.ascii	"                                                  "
	.ascii	"                                                  "
	.ascii	"                                                  "
	.ascii	"                                                  "
	.ascii	"                                                  "
	.ascii	"                                                  "
