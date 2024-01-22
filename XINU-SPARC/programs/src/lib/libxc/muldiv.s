	.seg	"text"
	.align	4
	.global .urem
.urem:
ba	.rem + 0x24
clr	%g1

	.global .rem
.rem:
orcc	%o1, %o0, %g0
bge	.rem + 0x24
mov	%o0, %g1
orcc	%g0, %o1, %g0
bge	.rem + 0x20
orcc	%g0, %o0, %g0
bge	.rem + 0x24
sub	%g0, %o1, %o1
sub	%g0, %o0, %o0
orcc	%o1, %g0, %o5
bne	.rem + 0x38
mov	%o0, %o3
ba	.rem + 0x2a0
nop
cmp	%o3, %o5
blu	.rem + 0x28c
clr	%o2
sethi	%hi(0x8000000),	%g2
cmp	%o3, %g2
blu	.rem + 0xe8
clr	%o4
cmp	%o5, %g2
bgeu	.rem + 0x8c
mov	0x1, %g3
sll	%o5, 0x4, %o5
ba	.rem + 0x54
add	%o4, 0x1, %o4
addcc	%o5, %o5, %o5
bgeu	.rem + 0x8c
add	%g3, 0x1, %g3
sll	%g2, 0x4, %g2
srl	%o5, 0x1, %o5
add	%o5, %g2, %o5
ba	.rem + 0xa0
sub	%g3, 0x1, %g3
cmp	%o5, %o3
blu	.rem + 0x6c
nop
be	.rem + 0xa0
nop
subcc	%g3, 0x1, %g3
bl	.rem + 0x278
nop
sub	%o3, %o5, %o3
mov	0x1, %o2
ba,a	.rem + 0xd8
sll	%o2, 0x1, %o2
bl	.rem + 0xd0
srl	%o5, 0x1, %o5
sub	%o3, %o5, %o3
ba	.rem + 0xd8
add	%o2, 0x1, %o2
add	%o3, %o5, %o3
sub	%o2, 0x1, %o2
subcc	%g3, 0x1, %g3
bge	.rem + 0xb8
orcc	%g0, %o3, %g0
ba,a	.rem + 0x278
sll	%o5, 0x4, %o5
cmp	%o5, %o3
bleu	.rem + 0xe8
addcc	%o4, 0x1, %o4
be	.rem + 0x28c
sub	%o4, 0x1, %o4
orcc	%g0, %o3, %g0
sll	%o2, 0x4, %o2
bl	.rem + 0x1c4
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
bl	.rem + 0x170
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
bl	.rem + 0x14c
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
bl	.rem + 0x140
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
ba	.rem + 0x278
add	%o2, 0xf, %o2
addcc	%o3, %o5, %o3
ba	.rem + 0x278
add	%o2, 0xd, %o2
addcc	%o3, %o5, %o3
bl	.rem + 0x164
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
ba	.rem + 0x278
add	%o2, 0xb, %o2
addcc	%o3, %o5, %o3
ba	.rem + 0x278
add	%o2, 0x9, %o2
addcc	%o3, %o5, %o3
bl	.rem + 0x1a0
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
bl	.rem + 0x194
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
ba	.rem + 0x278
add	%o2, 0x7, %o2
addcc	%o3, %o5, %o3
ba	.rem + 0x278
add	%o2, 0x5, %o2
addcc	%o3, %o5, %o3
bl	.rem + 0x1b8
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
ba	.rem + 0x278
add	%o2, 0x3, %o2
addcc	%o3, %o5, %o3
ba	.rem + 0x278
add	%o2, 0x1, %o2
addcc	%o3, %o5, %o3
bl	.rem + 0x224
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
bl	.rem + 0x200
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
bl	.rem + 0x1f4
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
ba	.rem + 0x278
add	%o2, -0x1, %o2
addcc	%o3, %o5, %o3
ba	.rem + 0x278
add	%o2, -0x3, %o2
addcc	%o3, %o5, %o3
bl	.rem + 0x218
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
ba	.rem + 0x278
add	%o2, -0x5, %o2
addcc	%o3, %o5, %o3
ba	.rem + 0x278
add	%o2, -0x7, %o2
addcc	%o3, %o5, %o3
bl	.rem + 0x254
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
bl	.rem + 0x248
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
ba	.rem + 0x278
add	%o2, -0x9, %o2
addcc	%o3, %o5, %o3
ba	.rem + 0x278
add	%o2, -0xb, %o2
addcc	%o3, %o5, %o3
bl	.rem + 0x26c
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
ba	.rem + 0x278
add	%o2, -0xd, %o2
addcc	%o3, %o5, %o3
ba	.rem + 0x278
add	%o2, -0xf, %o2
subcc	%o4, 0x1, %o4
bge	.rem + 0x104
orcc	%g0, %o3, %g0
bl,a	.rem + 0x28c
add	%o3, %o1, %o3
orcc	%g0, %g1, %g0
bl,a	.rem + 0x298
sub	%g0, %o3, %o3
jmp	%o7 + 0x8
mov	%o3, %o0
ta	0x2
jmp	%o7 + 0x8
clr	%o0

	.global .udiv
.udiv:
ba	.div + 0x24
clr	%g1

	.global .div
.div:
orcc	%o1, %o0, %g0
bge	.div + 0x24
xor	%o1, %o0, %g1
orcc	%g0, %o1, %g0
bge	.div + 0x20
orcc	%g0, %o0, %g0
bge	.div + 0x24
sub	%g0, %o1, %o1
sub	%g0, %o0, %o0
orcc	%o1, %g0, %o5
bne	.div + 0x38
mov	%o0, %o3
ba	.div + 0x2a0
nop
cmp	%o3, %o5
blu	.div + 0x28c
clr	%o2
sethi	%hi(0x8000000),	%g2
cmp	%o3, %g2
blu	.div + 0xe8
clr	%o4
cmp	%o5, %g2
bgeu	.div + 0x8c
mov	0x1, %g3
sll	%o5, 0x4, %o5
ba	.div + 0x54
add	%o4, 0x1, %o4
addcc	%o5, %o5, %o5
bgeu	.div + 0x8c
add	%g3, 0x1, %g3
sll	%g2, 0x4, %g2
srl	%o5, 0x1, %o5
add	%o5, %g2, %o5
ba	.div + 0xa0
sub	%g3, 0x1, %g3
cmp	%o5, %o3
blu	.div + 0x6c
nop
be	.div + 0xa0
nop
subcc	%g3, 0x1, %g3
bl	.div + 0x278
nop
sub	%o3, %o5, %o3
mov	0x1, %o2
ba,a	.div + 0xd8
sll	%o2, 0x1, %o2
bl	.div + 0xd0
srl	%o5, 0x1, %o5
sub	%o3, %o5, %o3
ba	.div + 0xd8
add	%o2, 0x1, %o2
add	%o3, %o5, %o3
sub	%o2, 0x1, %o2
subcc	%g3, 0x1, %g3
bge	.div + 0xb8
orcc	%g0, %o3, %g0
ba,a	.div + 0x278
sll	%o5, 0x4, %o5
cmp	%o5, %o3
bleu	.div + 0xe8
addcc	%o4, 0x1, %o4
be	.div + 0x28c
sub	%o4, 0x1, %o4
orcc	%g0, %o3, %g0
sll	%o2, 0x4, %o2
bl	.div + 0x1c4
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
bl	.div + 0x170
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
bl	.div + 0x14c
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
bl	.div + 0x140
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
ba	.div + 0x278
add	%o2, 0xf, %o2
addcc	%o3, %o5, %o3
ba	.div + 0x278
add	%o2, 0xd, %o2
addcc	%o3, %o5, %o3
bl	.div + 0x164
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
ba	.div + 0x278
add	%o2, 0xb, %o2
addcc	%o3, %o5, %o3
ba	.div + 0x278
add	%o2, 0x9, %o2
addcc	%o3, %o5, %o3
bl	.div + 0x1a0
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
bl	.div + 0x194
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
ba	.div + 0x278
add	%o2, 0x7, %o2
addcc	%o3, %o5, %o3
ba	.div + 0x278
add	%o2, 0x5, %o2
addcc	%o3, %o5, %o3
bl	.div + 0x1b8
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
ba	.div + 0x278
add	%o2, 0x3, %o2
addcc	%o3, %o5, %o3
ba	.div + 0x278
add	%o2, 0x1, %o2
addcc	%o3, %o5, %o3
bl	.div + 0x224
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
bl	.div + 0x200
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
bl	.div + 0x1f4
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
ba	.div + 0x278
add	%o2, -0x1, %o2
addcc	%o3, %o5, %o3
ba	.div + 0x278
add	%o2, -0x3, %o2
addcc	%o3, %o5, %o3
bl	.div + 0x218
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
ba	.div + 0x278
add	%o2, -0x5, %o2
addcc	%o3, %o5, %o3
ba	.div + 0x278
add	%o2, -0x7, %o2
addcc	%o3, %o5, %o3
bl	.div + 0x254
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
bl	.div + 0x248
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
ba	.div + 0x278
add	%o2, -0x9, %o2
addcc	%o3, %o5, %o3
ba	.div + 0x278
add	%o2, -0xb, %o2
addcc	%o3, %o5, %o3
bl	.div + 0x26c
srl	%o5, 0x1, %o5
subcc	%o3, %o5, %o3
ba	.div + 0x278
add	%o2, -0xd, %o2
addcc	%o3, %o5, %o3
ba	.div + 0x278
add	%o2, -0xf, %o2
subcc	%o4, 0x1, %o4
bge	.div + 0x104
orcc	%g0, %o3, %g0
bl,a	.div + 0x28c
sub	%o2, 0x1, %o2
orcc	%g0, %g1, %g0
bl,a	.div + 0x298
sub	%g0, %o2, %o2
jmp	%o7 + 0x8
mov	%o2, %o0
ta	0x2
jmp	%o7 + 0x8
clr	%o0

	.global .mul
.mul:
mov	%o0, %y
andncc	%o0, 0xf, %o4
be	.mul + 0xe8
sethi	%hi(0xffff0000), %o5
andncc	%o0, 0xff, %o4
be,a	.mul + 0x124
mulscc	%o4, %o1, %o4
andncc	%o0, 0xfff, %o4
be,a	.mul + 0x16c
mulscc	%o4, %o1, %o4
andcc	%o0, %o5, %o4
be,a	.mul + 0x1c4
mulscc	%o4, %o1, %o4
andcc	%g0, %g0, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %g0, %o4
orcc	%g0, %o0, %g0
rd	%y, %o0
bge	.mul + 0xd0
orcc	%g0, %o0, %g0
sub	%o4, %o1, %o4
bge	.mul + 0xe0
mov	%o4, %o1
jmp	%o7 + 0x8
cmp	%o1, -0x1
jmp	%o7 + 0x8
addcc	%o1, %g0, %g0
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %g0, %o4
rd	%y, %o5
sll	%o4, 0x4, %o0
srl	%o5, 0x1c, %o5
orcc	%o5, %o0, %o0
bge	.mul + 0x11c
sra	%o4, 0x1c, %o1
jmp	%o7 + 0x8
cmp	%o1, -0x1
jmp	%o7 + 0x8
addcc	%o1, %g0, %g0
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %g0, %o4
rd	%y, %o5
sll	%o4, 0x8, %o0
srl	%o5, 0x18, %o5
orcc	%o5, %o0, %o0
bge	.mul + 0x164
sra	%o4, 0x18, %o1
jmp	%o7 + 0x8
cmp	%o1, -0x1
jmp	%o7 + 0x8
addcc	%o1, %g0, %g0
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %g0, %o4
rd	%y, %o5
sll	%o4, 0xc, %o0
srl	%o5, 0x14, %o5
orcc	%o5, %o0, %o0
bge	.mul + 0x1bc
sra	%o4, 0x14, %o1
jmp	%o7 + 0x8
cmp	%o1, -0x1
jmp	%o7 + 0x8
addcc	%o1, %g0, %g0
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %g0, %o4
rd	%y, %o5
sll	%o4, 0x10, %o0
srl	%o5, 0x10, %o5
orcc	%o5, %o0, %o0
bge	.mul + 0x224
sra	%o4, 0x10, %o1
jmp	%o7 + 0x8
cmp	%o1, -0x1
jmp	%o7 + 0x8
addcc	%o1, %g0, %g0


	.global .umul
.umul:
mov	%o0, %y
andncc	%o0, 0xf, %o4
be	.umul +	0xd8
sethi	%hi(0xffff0000), %o5
andncc	%o0, 0xff, %o4
be,a	.umul +	0x114
mulscc	%o4, %o1, %o4
andncc	%o0, 0xfff, %o4
be,a	.umul +	0x15c
mulscc	%o4, %o1, %o4
andcc	%o0, %o5, %o4
be,a	.umul +	0x1b4
mulscc	%o4, %o1, %o4
andcc	%g0, %g0, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %g0, %o4
orcc	%g0, %o1, %g0
bge	.umul +	0xcc
nop
add	%o4, %o0, %o4
rd	%y, %o0
jmp	%o7 + 0x8
addcc	%o4, %g0, %o1
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %g0, %o4
rd	%y, %o5
orcc	%g0, %o1, %g0
bge	.umul +	0x100
sra	%o4, 0x1c, %o1
add	%o1, %o0, %o1
sll	%o4, 0x4, %o0
srl	%o5, 0x1c, %o5
or	%o5, %o0, %o0
jmp	%o7 + 0x8
orcc	%g0, %o1, %g0
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %g0, %o4
rd	%y, %o5
orcc	%g0, %o1, %g0
bge	.umul +	0x148
sra	%o4, 0x18, %o1
add	%o1, %o0, %o1
sll	%o4, 0x8, %o0
srl	%o5, 0x18, %o5
or	%o5, %o0, %o0
jmp	%o7 + 0x8
orcc	%g0, %o1, %g0
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %g0, %o4
rd	%y, %o5
orcc	%g0, %o1, %g0
bge	.umul +	0x1a0
sra	%o4, 0x14, %o1
add	%o1, %o0, %o1
sll	%o4, 0xc, %o0
srl	%o5, 0x14, %o5
or	%o5, %o0, %o0
jmp	%o7 + 0x8
orcc	%g0, %o1, %g0
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %o1, %o4
mulscc	%o4, %g0, %o4
rd	%y, %o5
orcc	%g0, %o1, %g0
bge	.umul +	0x208
sra	%o4, 0x10, %o1
add	%o1, %o0, %o1
sll	%o4, 0x10, %o0
srl	%o5, 0x10, %o5
or	%o5, %o0, %o0
jmp	%o7 + 0x8
orcc	%g0, %o1, %g0


