/* procreg.h -- constants associated with DEC processor register mnemonics */

#define ASM				/* mark for other .h files */
#include <conf.h>

/* VAX privileged processor registers -- accessed via mtpr and mfpr */
#define	KSP	0x0			/* kernel stack pointer */
#define	ESP	0x1			/* executive stack pointer */
#define	SSP	0x2			/* supervisor stack pointer */
#define	USP	0x3			/* user stack pointer */
#define	ISP	0x4			/* interrupt stack pointer */
#define	P0BR	0x8			/* p0 base register */
#define	P0LR	0x9			/* p0 length register */
#define	P1BR	0xa			/* p1 base register */
#define	P1LR	0xb			/* p1 length register */
#define	SBR	0xc			/* system base register */
#define	SLR	0xd			/* system length register */
#define	PCBB	0x10			/* process control block base */
#define	SCBB	0x11			/* system control block base */
#define	IPL	0x12			/* interrupt priority level */
#define	ASTLVL	0x13			/* asynchronous system trap level */
#define	SIRR	0x14			/* software interrupt request */
#define	SISR	0x15			/* software interrupt summary */
#define	CMIER	0x17			/* CMI error (750/751) */
#define	MCSR	0x17			/* machine check status register */
#define	ICCS	0x18			/* interval clock control/status */
#define	NICR	0x19			/* next interval count */
#define	ICR	0x1a			/* interval count */
#define	TODR	0x1b			/* time of day */
#define	CSRS	0x1c			/* console storage receive/status */
#define	CSDR	0x1d			/* console storage receive data */
#define	CSTS	0x1e			/* console storage transmit/status */
#define	CSTD	0x1f			/* console storage transmit data */
#define	RXCS	0x20			/* console receive control/status */
#define	RXDB	0x21			/* console receive data buffer */
#define	TXCS	0x22			/* console transmit control/status */
#define	TXDB	0x23			/* console transmit data buffer */
#define	TBDR	0x24			/* translation buffer disable */
#define	CADR	0x25			/* cache disable */
#define	MCESR	0x26			/* machine check error summary */
#define	CAER	0x27			/* cache error */
#define	ACCS	0x28			/* accelerator control/status */
#define	IUR	0x37			/* init unibus or Q-bus register */
#define	MAPEN	0x38			/* memory management enable */
#define	TBIA	0x39			/* translation buffer invalidate all*/
#define	TBIS	0x3a			/* trans. buffer invalidate single */
#define	TB	0x3b			/* translation buffer */
#define	PMR	0x3d			/* performance monitor enable */
#define	SID	0x3e			/* system identification */
#define	TBCHK	0x3f			/* translation buffer check */

#if defined(VAX785)||defined(VAX780)
#define	ACCR	0x29			/* accelerator maintenance */
#define	WCSA	0x2c			/* WCS address */
#define	WCSD	0x2d			/* WCS data */
#define	SBIFS	0x30			/* SBI fault and status */
#define	SBIS	0x31			/* SBI silo */
#define	SBISC	0x32			/* SBI silo comparator */
#define	SBIMT	0x33			/* SBI maintenance */
#define	SBIER	0x34			/* SBI error register */
#define	SBITA	0x35			/* SBI timeout address */
#define	SBIQC	0x36			/* SBI quadword clear */
#define	MBRK	0x3c			/* microcode breakpoint */
#endif

#if defined(VAX8600)
#define	PAMACC	0x40			/* physical address memory access */
#define	PAMLOC	0x41			/* physical address memory location */
#define	CSWP	0x42			/* cache sweep */
#define	MDECC	0x43			/* mbox data ecc */
#define	MENA	0x44			/* mbox error enable */
#define	MDCTL	0x45			/* mbox data control */
#define	MCCTL	0x46			/* mbox mcc control */
#define	MERG	0x47			/* mbox error generator */
#define CRBT	0x48			/* console reboot */
#define DFI	0x49			/* diagnostic fault insertion */
#define	EHSR	0x4a			/* error handling status */
#define	STXCS	0x4c			/* storage transmit control/status */
#define	STXDB	0x4d			/* storage transmit data buffer */
#define	ESPA	0x4e			/* ebox scratchpad address */
#define	ESPD	0x4f			/* ebox scratchpad data */
#endif

/* masks for processor registers */
#define	DISABLE 0x1f			/* IPL to disable interrupts */
#define	ENABLE 0x0			/* IPL to enable interrupts */
#define NOMAP 0x0			/* value to disable memory mapping */
#define	MAP 0x1				/* value to enable memory mapping */
#define MCESRCLR 0xd			/* value to clear MCESR	*/
#define IURSET 0x1			/* value to init bus */
#define IURUNSET 0x0			/* value to uninit bus */

#if defined(VAX780) || defined(VAX785)
#define SBIFSCLR 0			/* value to clear SBIFS	*/
#endif

#if defined(VAX8600)
#define	CACHCLR	0xb			/* flush cache, no write-through   */
#endif

#define LOWREGS	0x3f			/* mask for saving registers r5-r0 */
#define	NLOWREG	6			/* number of regs saved w LOWREGS  */
