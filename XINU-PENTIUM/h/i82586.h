/* i82586.h */


#define	I82586_ISCP	0x0		/* ISCP offset	*/
#define	I82586_SCB	0x8		/* SCB offset	*/
#define	I82586_SCP	0xfff6		/* SCP offset	*/

/* SCP (System Configuration pointer) structure */
struct scp {
    unsigned short	scp_sysbus;	/* bus width: 16 or 8 bits	*/
#define	SYSBUS_16	0x0
#define	SYSBUS_8	0x1
    unsigned short	scp_pad1;
    unsigned short	scp_pad2;
    unsigned long	scp_iscp;	/* points to ISCP		*/
};

/* ISCP (Intermediate System Configuration pointer) structure */
struct iscp {
    unsigned short	iscp_busy;	/* 01h -> chip is busy	*/
    unsigned short	iscp_scboff;	/* SCB offset 	        */
    unsigned long	iscp_scbbase;	/* SCB base		*/
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
#define	SCBSTAT_RUNORS	0x0020		/* RU NO resources		*/
#define	SCBSTAT_RUSUS	0x0010		/* RU suspended			*/
#define	SCBSTAT_RUR	0x0040		/* RU is ready			*/    
    unsigned short	scb_cmd;	/* command word			*/
#define	SCBCMD_CUS	0x0100		/* start command unit		*/
#define	SCBCMD_RUS	0x0010		/* start receive unit		*/
#define	SCBCMD_RESET	0x0080		/* RESET			*/    
    unsigned short	scb_cbloff;	/* CBL offset			*/
    unsigned short	scb_rfaoff;	/* RFA offset			*/
    unsigned short	scb_ecrc;	/* # CRC errors			*/
    unsigned short	scb_ealign;	/* # alignment error		*/
    unsigned short	scb_eresource;	/* # resource errors		*/
    unsigned short	scb_eover;	/* # over-run errors		*/
};

/* NOP command block */
struct nopcbl {
    unsigned short	cbl_status;
    unsigned short	cbl_cmd;
    unsigned short	cbl_next;	/* next command			*/
};

/* TX command block */
struct txcbl {
    unsigned short cbl_status;
#define	TXCBL_NC	0x0400		/* no carrier			*/
    unsigned short cbl_cmd;
    unsigned short cbl_next;		/* next command			*/
    unsigned short cbl_tbd;		/* tx buffer descriptor 	*/
};

#define	CONFIG_LEN	12		/* configuration info: 12 octets */
#define DUMP_SIZE	170		/* dump area for the DUMP command*/
#define DUMP_OFFSET	8		/* relative offset to dump buffer*/

/* command block list (CBL) */
struct cbl {
    unsigned short	cbl_status;
#define CUCMD_COMPLETE	0x8000	/* action command completed		*/
#define CUCMD_OK	0x2000	/* action command OK			*/
    unsigned short	cbl_cmd;
#define	CUCMD_NOP	0	/* no operation				*/
#define	CUCMD_IASET	1	/* set the individual Ethernet address	*/
#define	CUCMD_CONFIG	2	/* configure LAN parameters		*/
#define	CUCMD_MCSET	3	/* set multicast address table		*/
#define	CUCMD_TX	4	/* transmit a packet			*/
#define	CUCMD_DUMP	6	/* dump					*/
#define	CUCMD_DIAG	7	/* self diagnostic			*/
#define	CUCMD_EL	0x8000	/* is last on the list			*/
#define	CUCMD_S		0x4000	/* suspend after completion		*/
#define	CUCMD_I		0x2000	/* interrupt after completion		*/
#define	CUCMD_MASK	0x7	/* mask bits for basic command		*/
    unsigned short	cbl_next;/* next command			*/
    union {
	unsigned char	cbu_addr[EP_ALEN];
	struct {
	    unsigned short	cbm_alen;
	    Eaddr		cbm_mca[EP_MAXMULTI];
	} cbu_mcdata;
	struct {
	    unsigned short 	dumpbuf_off;
	    unsigned char	dumpbuf[DUMP_SIZE];	/* dump buffer */
	} cbu_dump;
	unsigned char	cbu_cfg[CONFIG_LEN];
    } cbl_data;
};

#define	cbl_addr	cbl_data.cbu_addr
#define	cbl_cfg		cbl_data.cbu_cfg
#define	cbl_alen	cbl_data.cbu_mcdata.cbm_alen
#define	cbl_mca		cbl_data.cbu_mcdata.cbm_mca
#define	cbl_dumpbuf_off	cbl_data.cbu_dump.dumpbuf_off
#define	cbl_dumpbuf	cbl_data.cbu_dump.dumpbuf

/* transmit buffer descriptor (TBD) */
struct tbd {
    unsigned short	tbd_status;	/* data byte count		*/
#define			TBD_EOF	 	0x8000	/* last TBD mark	*/
#define 		TBD_ACNT_MASK 	0x3fff  /* Actual count of data */
    unsigned short	tbd_next;
    unsigned long	tbd_buf;	/* tx buffer			*/
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
#define RFD_S		0x4000		/* suspend CU			*/
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
#define RBD_OK		0xc000		/* a good frame			*/
#define RBD_COUNT_MASK	0x3fff		/* mask for getting ACT COUNT	*/
    unsigned short rbd_next;		/* next RBD offset		*/
    unsigned long  rbd_buf;		/* buffer address		*/
    unsigned short rbd_size;		/* END_OF_LIST:size		*/
#define RBD_EL		0x8000		/* EL: Last RBD			*/
};
