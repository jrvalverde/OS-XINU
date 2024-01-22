/* ee.h - Intel EtherExpress */

#include "./mon/moni82586.h"

#define EE_IOBASE       0x250
#define EE_IRQ          5

/*
 * memory-mapped address to absulate address
 */
#define H2IO(ped, x)    ((unsigned short) (((unsigned long)(x) - ped->ed_iomem)&0xffff))
#define IO2H(ped, x)    ((unsigned long) (ped->ed_iomem + (int)(x)))

/*
 * Constants for the Intel EtherExpress16 Ethernet device
 */
#define EE_BOARD_ID	 0xbaba	/* EE board ID				*/
#define EE_16_1ST	 0xbaba	/* 1st generation of 16/16TP board	*/
#define EE_16_2ND	 0xbabb	/* 2nd generation of 16/16TP board	*/
#define EE_MCA	 	 0xbabc	/* MCA					*/
#define EE_RAM_SIZE	 16384	/* 16 K on-board DRAM			*/
#define	BIG_FRAME_SIZE	 1520	/* big receive frame buffer size	*/
#define SMALL_FRAME_SIZE  512	/* samll receive frame buffer size	*/

/*
 * offset to various registers
 */
#define ECR1_OFFSET	0x300e	/* R/W byte, Extended Control Register	*/
#define AUTOID_OFFSET	0x300f	/* R   byte, Shadow Auto ID		*/

/*
 * bits to select type of connector by setting ECR1 register
 * NOTE: EE 16/16TP 1st generation cards cannot use them.
 */
#define AUI_CONNECTOR	0x00	/* use AUI as connector			*/
#define BNC_CONNECTOR	0x80	/* use COAX as connector		*/
#define TPE_CONNECTOR	0x82	/* use TPE as connector			*/

#define BUS_TYPE	0x80	/* ECR1 bit 7 - MCA/ISA bus		*/

/* EtherExpress 16 registers */
#define	DXREG		0x0	/* data transfer			*/
#define	WRPTR		0x2	/* write pointer			*/
#define	RDPTR		0x4	/* read pointer				*/
#define	CAC		0x6	/* channel attention			*/
#define	SIRQ		0x7	/* IRQ select				*/
#define	SMB_PTR		0x8	/* Shadow Memory Bank pointer		*/
#define	MEMDEC		0xa	/* memory address decode		*/
#define	MEMC		0xb	/* memory mapped control		*/
#define	MEMPC		0xc	/* memcs16 page control			*/
#define	CONFIG		0xd	/* configuration and test		*/
#define	EEC		0xe	/* EEPROM control, reset		*/
#define	MEMEC		0xf	/* memory and control, e000 seg		*/
#define	AID		0xf	/* automatic ID				*/

/* Bit definitions */
#define	CONFIG_LINK	0x01	/* link integrity disable		*/
#define	CONFIG_LOOP	0x02	/* loop-back test enable		*/
#define	CONFIG_BUS16	0x04	/* adapter is in 16 bit slot		*/
#define	CONFIG_16EN	0x08	/* enable 16-bit if supported		*/
#define	CONFIG_ICLATE	0x10	/* activate IOCHRDY late		*/
#define	CONFIG_IOTE	0x20	/* IOCHRDY early/late test enable	*/
#define	CONFIG_TOGGLE	0x40	/* early/late test result		*/
#define	CONFIG_MCS16	0x80	/* test MEMCS16- settings		*/

#define	MEMC_MCS16	0x10	/* use MEMCS16- for 0xf0000 access	*/

#define	EEC_SK		0x01	/* EEPROM shift clock			*/
#define	EEC_CS		0x02	/* EEPROM chip select			*/
#define	EEC_DI		0x04	/* EEPROM data in			*/
#define	EEC_DO		0x08	/* EEPROM data out			*/
#define	EEC_GAR		0x40	/* reset ASIC				*/
#define	EEC_586R	0x80	/* reset 586				*/

#define	SIRQ_MASK	0x07	/* IRQ index				*/
#define	SIRQ_IEN	0x08	/* interrupt enable			*/

/* offsets to access SCB directly */
#define EE_SCB_STATUS      0xc008
#define EE_SCB_COMMAND     0xc00a
#define EE_SCB_CBL         0xc00c
#define EE_SCB_RFA         0xc00e

/* buffer memory map */
#define EE_MEMSIZE         0x10000
#define EE_ISCP            0x0             /* ISCP offset  */
#define EE_SCB_OFFSET      0x0008          /* SCB offset   */
#define EE_SCP             0xfff6          /* SCP offset   */

/* EEPROM commands */
#define	EEX_READ	0x06

/* EEPROM addresses */
#define	EE_BOARD	0x00	/* board configuration info		*/
#define	EE_MEM1		0x01	/* memory map (MEMC & MEMPC)		*/
#define	EE_EADDR	0x02	/* Ethernet address base (word offset)	*/
#define	EE_MEM2		0x06	/* memory map (MEMDEC)			*/

/*
 * utility macros
 */
#define EE_CA(iobase)			outb(iobase + CAC, 0)
#define EE_READ_SCB_STATUS(iobase)	inw(iobase + EE_SCB_STATUS)
#define EE_RESET(iobase)		outb(iobase + EEC, EEC_586R)

/*
 * important ROM bits
 */
/* ROM word 0 */
#define IOBASE_MASK	0x0f	/* bits: 0 - 3 selects base IO address	*/
#define AUISEL_MASK	0x1000	/* bit 12: AUI/BNC select		*/
#define INTSEL_MASK	0xe000	/* bits: 13 - 15 selects IRQ #		*/

/* ROM word 1 */
#define AUTOCNT_MASK	0x80	/* bit 7: 1: auto connector detect by driver*/

/* ROM word 5 */
#define TPETYPE_MASK	0x01	/* bit 0: TPE connection if set		*/
#define COMBOBIT_MASK	0x08	/* bit 3: 0: 2 connectors, 1: 3		*/

#define	EE_ROMSIZE	64	/* 16-bit words				*/

/*
 * RX blocks
 */
/*
 * number of receive frame descriptors and associated receive frame buffers.
 */
#define EE_RFD_NUM	9	/* number of receive frame descriptors */
