/* ether.h */

/* Ethernet definitions and constants */

#define	EP_MINLEN	  60	/* minimum packet length		*/
#define	EP_DLEN		1500	/* length of data field ep		*/
#define	EP_HLEN		  20	/* size of extended Ethernet header	*/
#define EP_EHLEN	  14	/* size of Ethernet header		*/
#define	EP_CRC		   4	/* ether CRC (trailer)			*/
#define	EP_MAXLEN	EP_HLEN+EP_DLEN
#define	EP_ALEN		   6	/* number of octets in physical address	*/
typedef	char	Eaddr[EP_ALEN];	/* length of physical address (48 bits)	*/
#define	EP_RETRY	   3	/* number of times to retry xmit errors	*/
#define	EP_BRC	"\377\377\377\377\377\377"/* Ethernet broadcast address	*/
#define EP_RTO           300	/* time out in seconds for reads	*/

#define EP_NUMRCV	16	/* number LANCE recv buffers (power 2)	*/
#define EP_NUMRCVL2	4	/* log2 of the number of recv buffers	*/
#define EP_NUMXMIT	1	/* number LANCE transmit buffers (power 2)*/
#define EP_NUMXMITL2	0	/* log2 of the number of transmit buffers*/
     
#define	ETINQSZ		EP_NUMRCV
#define	ETOUTQSZ	ETINQSZ

#define	EPT_LOOP	0x0060		/* type: Loopback		*/
#define	EPT_ECHO	0x0200		/* type: Echo			*/
#define	EPT_PUP		0x0400		/* type: Xerox PUP		*/
#define	EPT_IP		0x0800		/* type: Internet Protocol	*/
#define	EPT_ARP		0x0806		/* type: ARP			*/
#define	EPT_RARP	0x8035		/* type: Reverse ARP		*/

struct	eh {			/* ethernet header			*/
	Eaddr	eh_dst;		/* destination host address		*/
	Eaddr	eh_src;		/* source host address			*/
	unsigned short	eh_type;/* Ethernet packet type (see below)	*/
};

struct	ep	{		/* complete structure of Ethernet packet*/
	IPaddr	ep_nexthop;	/* niput() uses this			*/
	short	ep_len;		/* length of the packet			*/
	struct	eh ep_eh;	/* the ethernet header			*/
	char	ep_data[EP_DLEN];	/* data in the packet		*/
};

/* these allow us to pretend it's all one big happy structure */

#define	ep_dst	ep_eh.eh_dst
#define	ep_src	ep_eh.eh_src
#define	ep_type	ep_eh.eh_type

/* structure for handling a buffer ring entry for the LANCE */
struct bre {
    struct le_md *pmd;          /* pointer to the message descriptor    */
    char *buf;                  /* pointer to the buffer for this desc  */
};

/* Ethernet control block descriptions */
struct	etblk	{
	struct	le_device *etle;/* address of device csr		*/
	struct	le_init_block *etib;/* initialization block		*/
	struct	devsw	*etdev;	/* Address of device switch table entry	*/
	Eaddr	etpaddr;	/* Ethernet physical device address	*/
	Eaddr	etbcast;	/* Ethernet broadcast address		*/
	char	etfilter[8];	/* Ethernet logical address filter	*/
	int	etinq;		/* queue of incoming buffers		*/
	int 	etoutq;		/* queue of outgoing buffers		*/
	int	etoutqlen;	/* outgoing queue length		*/
	int	etrpid;		/* process trying to read, if any	*/
	int	etrpending;	/* indicates receive is pending	        */
	int	etrsem;		/* semaphore to protect readers		*/
	int	etintf;		/* our interface number			*/
	char    *etwbuf;        /* pointer to current transmit buffer   */
	short	etnextbuf;	/* for checking buffers round robin	*/
	int     etwtry;         /* num. of times to retry xmit errors   */
	char	*etdescr;	/* text description of the device	*/
	struct	bre etbrt[EP_NUMXMIT]; /* ring descriptor for transmits	*/
	struct	bre etbrr[EP_NUMRCV];/* ring descriptor for receives	*/
};


/* ethernet function codes */

#define	EPC_PROMON	1		/* turn on promiscuous mode	*/
#define	EPC_PROMOFF	2		/* turn off promiscuous mode	*/
#define	EPC_MULTICAST	3		/* set multicast address	*/

extern	struct	etblk	eth[];

