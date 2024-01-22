/* kbminit.c -- kbminit */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>
#include <kbm.h>

struct tty	kbmtty;


/*------------------------------------------------------------------------
 *  kbminit -- initialize the PC keyboard
 *------------------------------------------------------------------------
 */
kbminit(pdev)
struct	devsw	*pdev;
{
	STATWORD	ps;
	int		kbmint();

	set_evec(pdev->dvivec, kbmint);

	while (inb(KB_CSR) & KBS_NRDY);	/* wait for input buffer empty */
	outb(KB_CSR, 0x60);		/* next write is a command */
	while (inb(KB_CSR) & KBS_NRDY);
	outb(KB_DAT, 0x69);
	while (inb(KB_CSR) & KBS_NRDY);
	outb(KB_DAT, 0xff);		/* reset */
	while ((inb(KB_CSR) & KBS_HASDAT) == 0);
	pdev->dvioblk = 0;
{
unsigned char c = inb(KB_DAT);
/*
	if (inb(KB_DAT) != 0xAA)
		panic("keyboard initialization error\n");
*/
}
}
