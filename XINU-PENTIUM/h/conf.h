/* conf.h (GENERATED FILE; DO NOT EDIT) */

#define	NULLPTR	(char *)0

/* Device table declarations */
struct	devsw	{			/* device table entry */
	int	dvnum;
	char	*dvname;
	int	(*dvinit)();
	int	(*dvopen)();
	int	(*dvclose)();
	int	(*dvread)();
	int	(*dvwrite)();
	int	(*dvseek)();
	int	(*dvgetc)();
	int	(*dvputc)();
	int	(*dvcntl)();
	int	dvcsr;
	int	dvivec;
	int	dvovec;
	int	(*dviint)();
	int	(*dvoint)();
	char	*dvioblk;
	int	dvminor;
	};

extern	struct	devsw devtab[];		/* one entry per device */


/* Device name definitions */

#define	CONSOLE     0			/* type tty      */
#define	SERIAL0     1			/* type serial   */
#define	SERIAL1     2			/* type serial   */
#define	KBMON       3			/* type kbmon    */
#define	TTY0        4			/* type tty      */
#define	TTY1        5			/* type tty      */
#define	TTY2        6			/* type tty      */
#define	ETHER       7			/* type eth      */
#define	UDP         8			/* type dgm      */
#define	DGRAM0      9			/* type dg       */
#define	DGRAM1      10			/* type dg       */
#define	DGRAM2      11			/* type dg       */
#define	DGRAM3      12			/* type dg       */
#define	DGRAM4      13			/* type dg       */
#define	DGRAM5      14			/* type dg       */
#define	DGRAM6      15			/* type dg       */
#define	DGRAM7      16			/* type dg       */
#define	DGRAM8      17			/* type dg       */
#define	DGRAM9      18			/* type dg       */
#define	DGRAMA      19			/* type dg       */
#define	DGRAMB      20			/* type dg       */
#define	DGRAMC      21			/* type dg       */
#define	DGRAMD      22			/* type dg       */
#define	DGRAME      23			/* type dg       */
#define	DGRAMF      24			/* type dg       */
#define	TCP         25			/* type tcpm     */
#define	TCP0        26			/* type tcp      */
#define	TCP1        27			/* type tcp      */
#define	TCP2        28			/* type tcp      */
#define	TCP3        29			/* type tcp      */
#define	TCP4        30			/* type tcp      */
#define	TCP5        31			/* type tcp      */
#define	TCP6        32			/* type tcp      */
#define	TCP7        33			/* type tcp      */
#define	TCP8        34			/* type tcp      */
#define	TCP9        35			/* type tcp      */
#define	TCPA        36			/* type tcp      */
#define	TCPB        37			/* type tcp      */
#define	TCPC        38			/* type tcp      */
#define	TCPD        39			/* type tcp      */
#define	TCPE        40			/* type tcp      */
#define	TCPF        41			/* type tcp      */
#define	RFILSYS     42			/* type rfm      */
#define	RFILE1      43			/* type rf       */
#define	RFILE2      44			/* type rf       */
#define	RFILE3      45			/* type rf       */
#define	RFILE4      46			/* type rf       */
#define	NAMESPACE   47			/* type nam      */

/* Control block sizes */

#define	Nkbmon	1
#define	Nserial	2
#define	Ntty	4
#define	Neth	1
#define	Ndgm	1
#define	Ndg	16
#define	Ntcpm	1
#define	Ntcp	16
#define	Nrfm	1
#define	Nrf	4
#define	Nnam	1

#define	NDEVS	48

/* Declarations of I/O routines referenced */

extern	int	ttyinit();
extern	int	ttyopen();
extern	int	ionull();
extern	int	ttyread();
extern	int	ttywrite();
extern	int	ioerr();
extern	int	ttycntl();
extern	int	ttygetc();
extern	int	ttyputc();
extern	int	ttyiin();
extern	int	ttyoin();
extern	int	cominit();
extern	int	comwrite();
extern	int	comcntl();
extern	int	comgetc();
extern	int	computc();
extern	int	comint();
extern	int	kbminit();
extern	int	kbmread();
extern	int	kbmwrite();
extern	int	kbmcntl();
extern	int	kbmgetc();
extern	int	kbmputc();
extern	int	kbmint();
extern	int	ee_init();
extern	int	ee_read();
extern	int	ee_write();
extern	int	ee_cntl();
extern	int	ee_intr();
extern	int	dgmopen();
extern	int	dgmcntl();
extern	int	dginit();
extern	int	dgclose();
extern	int	dgread();
extern	int	dgwrite();
extern	int	dgcntl();
extern	int	tcpmopen();
extern	int	tcpmcntl();
extern	int	tcpinit();
extern	int	tcpclose();
extern	int	tcpread();
extern	int	tcpwrite();
extern	int	tcpcntl();
extern	int	tcpgetc();
extern	int	tcpputc();
extern	int	rfopen();
extern	int	rfcntl();
extern	int	rfinit();
extern	int	rfclose();
extern	int	rfread();
extern	int	rfwrite();
extern	int	rfseek();
extern	int	rfgetc();
extern	int	rfputc();
extern	int	naminit();
extern	int	namopen();


/* Configuration and Size Constants */

#define	LITTLE_ENDIAN	0x1234
#define	BIG_ENDIAN	0x4321

#define	BYTE_ORDER	LITTLE_ENDIAN

#define	CONTACT     "dds@cs.purdue.edu"	/* administrative contact	*/
#define	LOCATION    "PC Lab"		/* Host's physical location	*/

#define	NPROC	    50			/* number of user processes	*/
#define	NSEM	    100			/* number of semaphores		*/
#define	MEMMARK				/* define if memory marking used*/
#define	RTCLOCK				/* now have RTC support		*/
#define	STKCHK				/* resched checks stack overflow*/
#define	NETDAEMON			/* Ethernet network daemon runs	*/
/*#define NETMON			/* define for network monitor	*/
/*#define OSPF				/* define if using OSPF		*/
/*#define RIP				/* define if using RIP		*/
/*#define MULTICAST*/			/* define if using multicasting	*/

#ifdef	OSPF
#define	MULTICAST			/* OSPF uses multicasting	*/
#endif	/* OSPF */

#define	DEFAULT_IPADDR1	"128.211.1.243" /* my IP address		*/
#define	DEFAULT_ROUTER	"128.211.1.250" /* default router		*/
#define	DEFAULT_TSERVER	"128.211.1.10:37"/* Time server address		*/
#define	DEFAULT_RSERVER	"128.10.15.64:2001"/* Remote file server address*/
#define	DEFAULT_NSERVER	"128.211.1.10:53"  /* Domain Name server address*/
#define	DEFAULT_NETMASK	 0xffffff00	/* default subnet mask		*/
#ifdef WIRELESS
#define	DEFAULT_WIRELESSIP "128.211.1.99" /* wrieless I/F IP address	*/
#endif

#define TSERVERPORT    37
#define RSERVERPORT  2001
#define NSERVERPORT    53

#define	BINGID	9			/* Othernet simlated net param. */

#define	SMALLMTU	400		/* for OTHER2; sim. small MTU	*/

#define	BPMAXB	8192			/* max buffer size for mkpool	*/
#define BPMAXN	128			/* max # buffers in a buf pool	*/

#define	TCPSBS	4096			/* TCP send buffer sizes	*/
#define	TCPRBS	8192			/* TCP receive buffer sizes	*/

#define	NPORTS	100

#define	TIMERGRAN	1		/* timer granularity, secs/10	*/

#define	NAREA		1		/* max number of OSPF Areas	*/

#ifndef	Noth
#define	Noth	0
#endif	!Noth
#ifndef	Ndg
#define	Ndg	0
#endif
