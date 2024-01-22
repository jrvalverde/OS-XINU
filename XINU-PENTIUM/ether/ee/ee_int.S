/* ee_int.s - _ee_int */

#include <icu.s>

		.text
		.globl	ee_int
ee_int:
		cli
		pushal

		movb	$EOI,%al	/* re-enable the device */
		outb	%al,$OCW1_2

		call	ee_intr

		popal
		sti
		iret
