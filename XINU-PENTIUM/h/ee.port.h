/* ee.h - H2IO, IO2H */

/* 
 * Author: John C. Lin (lin@cs.purdue.edu).
 * Modified from the version by Dave L. Stevens (dls@cc.purdue.edu).
 * Date: 02/09/95
 *
 * Ref: 1. For i82586: Intel LAN Components User's manual, March 1984.
 *         ISBN: 08359-4098-5
 *      2. EtherExpress 16 Family LAN Adapters - External Product Specification
 *	   04/12/93
 *      3. Linux eexpress.c (driver by Donald Becker kecker@super.org.
 *	4. Linux AT&T/NCR WaveLAN driver by Bruce Janson
 *					    (bruce@staff.cs.su.oz.au)
 */

/*
 * Constants for the Intel EtherExpress16 Ethernet device
 */

#define	MAX_FRAME_SIZE	1520
#define EE_DRAM_SIZE	16384	/* 16 K on-board DRAM			*/

/* registers */
#define	DXREG		0x0	/* data transfer			*/
#define	WRPTR		0x2	/* write pointer			*/
#define	RDPTR		0x4	/* read pointer				*/
#define	CAC		0x6	/* channel attention			*/
#define	SIRQ		0x7	/* IRQ select				*/
#define	SMB		0x8	/* Shadow memory ptrs			*/
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

/* EEPROM commands */
#define	EEX_READ	0x06

/* EEPROM addresses */
#define	EE_BOARD	0x00	/* board configuration info		*/
#define	EE_MEM1		0x01	/* memory map (MEMC & MEMPC)		*/
#define	EE_EADDR	0x02	/* Ethernet address base (word offset)	*/
#define	EE_MEM2		0x06	/* memory map (MEMDEC)			*/

#define	EE_ROMSIZE	64	/* 16-bit words				*/

/* buffer memory map */
#define	MEMSIZE		0x10000
#define	ISCP		0x0		/* ISCP offset	*/
#define	SCB_OFFSET	0x0008		/* SCB offset	*/
#define	SCP		0xfff6		/* SCP offset	*/

#define	SYSBUS_16	0x0
#define	SYSBUS_8	0x1

/* SCP (System Configuration pointer) structure */
struct scp {
	unsigned short	 scp_sysbus;	/* bus width: 0:16, 1:8 bits	*/
	unsigned short	 scp_pad1;
	unsigned short	 scp_pad2;
	unsigned short	 scp_iscpl;	/* pointer to ISCP - low	*/
	unsigned short	 scp_iscph;	/* pointer to ISCP - high	*/
};

/* ISCP (Intermediate System Configuration pointer) structure */
struct iscp {
	unsigned short	 iscp_busy;	/* 01h -> chip is busy	*/
	unsigned short	 iscp_scboff;  	/* SCB offset 	        */ 
	unsigned short	 iscp_scbbasel; /* SCB base - low	*/
	unsigned short	 iscp_scbbaseh; /* SCB base - high	*/
};

/* system control block (SCB) */
struct scb {
	unsigned short	scb_status;	/* 82586 status			*/
#define	SCBSTAT_CX	0x8000		/* action command completed	*/
#define	SCBSTAT_FR	0x4000		/* received a frame		*/
#define	SCBSTAT_CNA	0x2000		/* CU left active state		*/
#define	SCBSTAT_RNR	0x1000		/* receiver not ready		*/
#define	SCBSTAT_CUS	0x0100		/* CU is suspended		*/
#define	SCBSTAT_CUA	0x0200		/* CU is active			*/
#define	SCBSTAT_RNORS	0x0020		/* RU NO resources		*/
#define	SCBSTAT_RUR	0x0040		/* RU is ready			*/
	unsigned short	scb_cmd;	/* command word			*/
#define	SCBCMD_CUS	0x0100		/* start command unit		*/
#define	SCBCMD_RUS	0x0010		/* start receive unit		*/
#define	SCBCMD_RESET	0x0080		/* RESET			*/
	unsigned short	scb_cbloff;	/* CBL offset		*/
	unsigned short	scb_rfaoff;	/* RFA offset		*/
	unsigned short	scb_ecrc;	/* # CRC errors		*/
	unsigned short	scb_ealign;	/* # alignment error	*/
	unsigned short	scb_eresource;	/* # resource errors	*/
	unsigned short	scb_eover;	/* # over-run errors	*/
};

/* offsets to access SCB directly */
#define SCB_STATUS	0xc008
#define SCB_COMMAND	0xc00a
#define SCB_CBL		0xc00c
#define SCB_RFA		0xc00e

/* NOP command block */
struct nopcbl {
    unsigned short	cbl_status;
    unsigned short	cbl_cmd;
    unsigned short	cbl_next;	/* next command		*/
};

/* TX command block */
struct txcbl {
    unsigned short cbl_status;
#define	TXCBL_NC	0x0400		/* no carrier		*/
    unsigned short cbl_cmd;
    unsigned short cbl_next;		/* next command		*/
    unsigned short cbl_tbd;		/* tx buffer descriptor */
};

#define	CONFIG_LEN	6		/* configuration info: 12 octets */
#define DUMP_SIZE	170		/* dump area for the DUMP command */

/* command block list (CBL) */
struct cbl {
    unsigned short	cbl_status;
#define CUCMD_COMPLETE	0x8000		/* action command completed	*/
#define CUCMD_OK	0x2000		/* action command OK		*/
    unsigned short	cbl_cmd;
#define	CUCMD_NOP	0	/* no operation				*/
#define	CUCMD_IASET	1	/* set the individual Ethernet address	*/
#define	CUCMD_CONFIG	2	/* configure LAN parameters		*/
#define	CUCMD_MCSET	3	/* set multicast address table		*/
#define	CUCMD_TX	4	/* transmit a packet			*/
#define	CUCMD_DUMP	6	/* dump					*/
#define	CUCMD_DIAG	7	/* self diagnostic			*/
#define	CUCMD_EL	0x8000	/* is last on the list		*/
#define	CUCMD_S		0x4000	/* suspend after completion	*/
#define	CUCMD_I		0x2000	/* interrupt after completion	*/
#define	CUCMD_MASK	0x7	/* mask bits for basic command		*/
    unsigned short	cbl_next;	/* next command	*/
    union {
	unsigned char	cbu_addr[EP_ALEN];
	struct {
	    unsigned short	cbm_alen;
	    Eaddr	cbm_mca[EP_MAXMULTI];
	} cbu_mcdata;
	unsigned char	cbu_dumpbuf[DUMP_SIZE];	/* dump buffer */
	unsigned short	cbu_cfg[CONFIG_LEN];
    } cbl_data;
};

#define	cbl_addr	cbl_data.cbu_addr
#define	cbl_cfg		cbl_data.cbu_cfg
#define	cbl_tbd		cbl_data.cbu_txdata.cbt_tbd
#define	cbl_alen	cbl_data.cbu_mcdata.cbm_alen
#define	cbl_mca		cbl_data.cbu_mcdata.cbm_mca
#define	cbl_dumpbuf	cbl_data.cbu_dumpbuf

#define	MCSTAT_A	0x1000		/* mcast command aborted	*/

/* transmit buffer descriptor (TBD) */
struct tbd {
    unsigned short	tbd_status;	/* data byte count		*/
#define			TBD_EOF	 	0x8000	/* last TBD mark	*/
#define 		TBD_ACNT_MASK 	0x3fff  /* Actual count of data */
    unsigned short	tbd_next;
    unsigned short	tbd_buf_lo;     /* Buffer address (low) 	*/
    unsigned short	tbd_buf_hi;     /* Buffer address (high) 	*/
};

/*
 * receive structures
 */

/* Receive Frame Descriptor (RFD) */
struct rfd {
	unsigned short	rfd_status;	/* receive frame status		*/
#define	RFD_C		0x8000		/* completed storing a frame	*/
#define	RFD_OK		0x2000		/* received frame ok		*/
#define	RFD_ERR		0x0fc0		/* received frame error	mask	*/
	unsigned short	rfd_cmd;	/* command word			*/
#define RFD_EL		0x8000		/* End of list bit		*/
	unsigned short	rfd_next;	/* next rfd offset		*/
	unsigned short	rfd_rbd;	/* RBD offset			*/
	Eaddr		rfd_dst;	/* destination Ether address	*/
	Eaddr		rfd_src;	/* source Ether address		*/
	unsigned short	rfd_len;	/* frame length			*/
};

/* Receive Buffer Descriptor (RBD) */
struct rbd {
    unsigned short rbd_count;		/* EOF:VALID:data byte count	*/
#define RBD_EOF		0x8000		/* EOF: last buffer		*/
#define RBD_VALID	0x4000		/* F: count field is valid	*/
#define RBD_COUNT_MASK	0x3fff		/* mask for getting ACT COUNT	*/
    unsigned short rbd_next;		/* next RBD offset		*/
    unsigned short rbd_bufl;		/* buffer address - LOW		*/
    unsigned short rbd_bufh;		/* buffer address - HIGH	*/
    unsigned short rbb_size;		/* END_OF_LIST:size		*/
#define RBD_EL		0x8000		/* EL: Last RBD			*/
};


/*
 * on-board 16k DRAM layout -- I defined them. :-)
 */
#define RAM_ISCP_OFFSET ISCP
#define RAM_TX_BEGIN	SCB_OFFSET + sizeof(struct scb) + sizeof(cbl)
#define RAM_TX_END	0x2000
#define RAM_RX_BEGIN	0x2000
#define RAM_TX_END	0x4000 

/*
 * TX and RX blocks
 */
#define TX_BLOCK_SIZE	(sizeof(struct txcbl)+sizeof(struct tbd)+
			 +sizeof(struct nopcbl)+MAX_FRAME_SIZE))
#define RX_BLOCK_SIZE	(sizeof(struct rfd)+sizeof(struct rbd)+MAX_FRAME_SIZE)
