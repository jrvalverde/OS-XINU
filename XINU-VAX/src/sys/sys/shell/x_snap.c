/* x_snap.c - x_snap */

#include <conf.h>
#include <kernel.h>
#include <shell.h>
#include <coreX.h>

extern	int	maxaddr;
LOCAL	int	snapreg;
#define	SNAPSIZ	512

/*------------------------------------------------------------------------
 *  x_snap  -  (command snap) write snapshot of memory to a core file
 *------------------------------------------------------------------------
 */
COMMAND	x_snap(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	int	dev, iPkt, len;
	struct	coreX	hdr;
	char	*p;

	if (nargs > 2) {
		fprintf(stderr,"usage: snap [core-file]\n");
		return(SYSERR);
	}
	p = nargs==1 ? "coreX" : args[1];
	if ((dev=open(NAMESPACE, p, "w")) == SYSERR) {
		fprintf(stderr,"snap: cannot open %s for writing\n", p);
		return(SYSERR);
	}

	/* make up a core image using coreX structure heading format */

	hdr.c_magic = COREMAGIC;
	hdr.c_size = hdr.c_zero1 = hdr.c_zero2 = hdr.c_zero3 =
		hdr.c_zero4 = hdr.c_zero5 = 0;
	/* Capture machine registers */

	asm("mfpr $0x0, _snapreg"); hdr.c_regs[KSP] = snapreg;
	asm("mfpr $0x1, _snapreg"); hdr.c_regs[ESP] = snapreg;
	asm("mfpr $0x2, _snapreg"); hdr.c_regs[SSP] = snapreg;
	asm("mfpr $0x3, _snapreg"); hdr.c_regs[SSP] = snapreg;
	asm("movl r0,_snapreg");  hdr.c_regs[R0]  = snapreg;
	asm("movl r1,_snapreg");  hdr.c_regs[R1]  = snapreg;
	asm("movl r2,_snapreg");  hdr.c_regs[R2]  = snapreg;
	asm("movl r3,_snapreg");  hdr.c_regs[R3]  = snapreg;
	asm("movl r4,_snapreg");  hdr.c_regs[R4]  = snapreg;
	asm("movl r5,_snapreg");  hdr.c_regs[R5]  = snapreg;
	asm("movl r6,_snapreg");  hdr.c_regs[R6]  = snapreg;
	asm("movl r7,_snapreg");  hdr.c_regs[R7]  = snapreg;
	asm("movl r8,_snapreg");  hdr.c_regs[R8]  = snapreg;
	asm("movl r9,_snapreg");  hdr.c_regs[R9]  = snapreg;
	asm("movl r10,_snapreg"); hdr.c_regs[R10] = snapreg;
	asm("movl r11,_snapreg"); hdr.c_regs[R11] = snapreg;
	asm("movl ap,_snapreg");  hdr.c_regs[AP] = snapreg;
	asm("movl fp,_snapreg");  hdr.c_regs[FP] = snapreg;
	asm("sneakpc: moval sneakpc, _snapreg");  hdr.c_regs[PC] = snapreg;
	asm("movpsl _snapreg");   hdr.c_psw = hdr.c_regs[PSL] = snapreg;
	asm("mfpr $0x8, _snapreg"); hdr.c_regs[P0BR] = snapreg;
	asm("mfpr $0x9, _snapreg"); hdr.c_regs[P0LR] = snapreg;
	asm("mfpr $0xa, _snapreg"); hdr.c_regs[P1BR] = snapreg;
	asm("mfpr $0xb, _snapreg"); hdr.c_regs[P1LR] = snapreg;
	fprintf(stderr, "Writing core image");
	if ( write(dev, &hdr, sizeof(struct coreX)) == SYSERR ) {
		fprintf(stderr,"snap: can't write core header, quitting\n");
		close(dev);
		return(SYSERR);
	}

	/* Add contents of real memory to core image */

	iPkt = 0;
	for (p= (char *)0 ; p < (char *)(maxaddr+sizeof(int)) ; p += len) {
		if (++iPkt == 25) {
			iPkt = 0;
			putc(stderr, '.');
		}
		len = (int) maxaddr - (int) p + sizeof(int);
		if (len > (unsigned) SNAPSIZ)
			len = SNAPSIZ;
		if (write(dev, p, SNAPSIZ) == SYSERR) {
			fprintf(stderr,
				"\nsnap: write error, quitting\n");
			close(dev);
			return(SYSERR);
		}
	}
	putc(stderr, '\n');
	close(dev);
	return(OK);
}
