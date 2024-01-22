/* iospace.h  */

#define	IOUVAXI		0x30000000	/* i/o space begins here,	*/
					/* bit 28==no cache,29==io space*/
#define	IOUVAXII	0x20000000

#define	IOVAX		0

#define	IOADDRM		0x1fff		/* mask for low bits of io address */
					/* (8K i/o space on current VAXen) */
