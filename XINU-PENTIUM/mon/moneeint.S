#include <icu.s>

		.text
		.globl	mon_ee_int
mon_ee_int:
		cli
		pushal

		movb	$EOI,%al	/* re-enable the device */
		outb	%al,$OCW1_2

		call	mon_ee_intr

		popal
		sti
		iret
