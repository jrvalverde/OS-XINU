/* slu.h */

#define SVECTOR 0x0070		/* location of zilog interrupt vector (12) */
#define ZSCC_PCLK 4915200
#define EVEC_LEVEL12     0x070

/* SERIAL0_BASE was defined here and in cpu.addrs.h in the Sun 3 version */
/* I think it should be in cpu.addrs.h only - not here (jng)	*/
/*#define SERIAL0_BASE    ((struct zscc_device *)         0X0FFD02d000)*/

/*
 * Delay units are in microseconds.
 */
#define	DELAY(n)	\
{ \
        extern int cpudelay; \
	register int N = (((n)<<4) >> cpudelay); \
 \
	while (--N > 0) ; \
}



