#include <./mon/monnetwork.h>
#include <./mon/monitor.h>
#include <./mon/moni386.h>
#include <./mon/monee.h>

#define	ROMDELAY()	{ DELAY(1); }

/*------------------------------------------------------------------------
 * mon_ee_romread - read PROM data
 *------------------------------------------------------------------------
 */
unsigned short
mon_ee_romread(csr, from)
int		csr;		/* base I/O address		*/
int		from;		/* EEPROM offset (words)	*/
{
    unsigned char	data, out;
    unsigned short	rv;
    int		i, j;

    outb(csr + EEC, EEC_586R);	/* reset 586		*/
    ROMDELAY();
    outb(csr + EEC, EEC_586R|EEC_GAR);
    DELAY(500);
    outb(csr + EEC, EEC_586R);
    ROMDELAY();
    outb(csr + EEC, 0);

    out = EEC_586R|EEC_CS;
    outb(csr + EEC, out);		/* select PROM		*/
    ROMDELAY();

    mon_ee_romwrite(csr, EEX_READ, 3);
    mon_ee_romwrite(csr, from, 6);	/* max 64 16-bit regs	*/
    rv = 0;
    for (i=0; i<16; ++i) {
	outb(csr+EEC, out | EEC_SK);
	ROMDELAY();
	data = inb(csr+EEC);
	data &= EEC_DO;
	rv <<= 1;
	rv |= ((unsigned short)data >> 3);
	outb(csr+EEC, out & ~EEC_SK);
	ROMDELAY();
    }

    outb(csr + EEC, 0);	/* clear 586 reset */
    return rv;
}

/*------------------------------------------------------------------------
 * mon_ee_romwrite - write PROM data
 *------------------------------------------------------------------------
 */
mon_ee_romwrite(csr, offset, nbits)
int     csr;
int     offset;
int     nbits;
{
    unsigned char       x, eec;
    unsigned short      mask;

    x = EEC_586R | EEC_CS;
    for (mask = 1 << (nbits - 1); mask != 0; mask >>= 1) {
        x &= 0xFB;
        if ((offset & mask) != 0)
            x |= EEC_DI;
        outb(csr+EEC, x);
        ROMDELAY();

        /* clock */
        outb(csr+EEC, x | EEC_SK);
        ROMDELAY()
            outb(csr+EEC, x & ~EEC_SK);
        ROMDELAY();
    }
    outb(csr+EEC, x & 0xFB);
}
