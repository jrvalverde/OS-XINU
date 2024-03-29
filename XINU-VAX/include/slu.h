/* slu.h */

/* standard serial line unit device constants */

#define	SLUENABLE	0100		/* device interrupt enable bit	*/
#define	SLUREADY	0200		/* device ready bit		*/
#define	SLUDISABLE	0000		/* device interrupt disable mask*/
#define	SLUTBREAK	0001		/* transmitter break-mode bit	*/
#define	SLUERMASK	0170000		/* mask for error flags on input*/
#define	SLUCHMASK	0377		/* mask for input character	*/

/* SLU device register layout and correspondence to vendor's names	*/

struct	csr	{
	short	crstat;		/* receiver control and status	(RCSR)	*/
	short	crbuf;		/* receiver data buffer		(RBUF)	*/
	short	ctstat;		/* transmitter control & status (XCSR)	*/
	short	ctbuf;		/* transmitter data buffer	(XBUF)	*/
};
