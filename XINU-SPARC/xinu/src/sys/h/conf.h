/* conf.h (GENERATED FILE; DO NOT EDIT) */

#define	NULLPTR	(char *)0

/* Device table declarations */
#ifndef ASM
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

#endif ASM

/* Device name definitions */

#define	CONSOLE     0			/* type tty      */
#define	ETHER       1			/* type eth      */
#define	NAMESPACE   2			/* type nam      */
#define	NULLDEV     3			/* type nulldev  */
#define	TCP         4			/* type tcpm     */
#define	TCP1        5			/* type tcp      */
#define	TCP2        6			/* type tcp      */
#define	TCP3        7			/* type tcp      */
#define	TCP4        8			/* type tcp      */
#define	TCP5        9			/* type tcp      */
#define	TCP6        10			/* type tcp      */
#define	TCP7        11			/* type tcp      */
#define	TCP8        12			/* type tcp      */
#define	TCP9        13			/* type tcp      */
#define	TCP10       14			/* type tcp      */
#define	RFILSYS     15			/* type rfm      */
#define	RFILE1      16			/* type rf       */
#define	RFILE2      17			/* type rf       */
#define	RFILE3      18			/* type rf       */
#define	RFILE4      19			/* type rf       */
#define	UDP         20			/* type dgm      */
#define	DGRAM1      21			/* type dg       */
#define	DGRAM2      22			/* type dg       */
#define	DGRAM3      23			/* type dg       */
#define	DGRAM4      24			/* type dg       */
#define	DGRAM5      25			/* type dg       */
#define	DGRAM6      26			/* type dg       */
#define	DGRAM7      27			/* type dg       */
#define	DGRAM8      28			/* type dg       */
#define	DGRAM9      29			/* type dg       */
#define	DGRAM10     30			/* type dg       */
#define	DGRAM11     31			/* type dg       */
#define	DGRAM12     32			/* type dg       */
#define	DGRAM13     33			/* type dg       */
#define	DGRAM14     34			/* type dg       */
#define	DGRAM15     35			/* type dg       */
#define	DGRAM16     36			/* type dg       */
#define	DGRAM17     37			/* type dg       */
#define	DGRAM18     38			/* type dg       */
#define	DGRAM19     39			/* type dg       */
#define	DGRAM20     40			/* type dg       */
#define	DGRAM21     41			/* type dg       */
#define	DGRAM22     42			/* type dg       */
#define	DGRAM23     43			/* type dg       */
#define	DGRAM24     44			/* type dg       */
#define	DGRAM25     45			/* type dg       */
#define	DGRAM26     46			/* type dg       */
#define	DGRAM27     47			/* type dg       */
#define	DGRAM28     48			/* type dg       */
#define	DGRAM29     49			/* type dg       */
#define	DGRAM30     50			/* type dg       */
#define	RPC         51			/* type rpcm     */
#define	RPC1        52			/* type rpc      */
#define	RPC2        53			/* type rpc      */
#define	RPC3        54			/* type rpc      */
#define	RPC4        55			/* type rpc      */
#define	RPC5        56			/* type rpc      */
#define	RPC6        57			/* type rpc      */
#define	RPC7        58			/* type rpc      */
#define	RPC8        59			/* type rpc      */
#define	RPC9        60			/* type rpc      */
#define	RPC10       61			/* type rpc      */
#define	RPC11       62			/* type rpc      */
#define	RPC12       63			/* type rpc      */
#define	RPC13       64			/* type rpc      */
#define	RPC14       65			/* type rpc      */
#define	RPC15       66			/* type rpc      */
#define	RPC16       67			/* type rpc      */
#define	RPC17       68			/* type rpc      */
#define	RPC18       69			/* type rpc      */
#define	RPC19       70			/* type rpc      */
#define	RPC20       71			/* type rpc      */
#define	NFS         72			/* type nfsm     */
#define	NFS1        73			/* type nfs      */
#define	NFS2        74			/* type nfs      */
#define	NFS3        75			/* type nfs      */
#define	NFS4        76			/* type nfs      */
#define	NFS5        77			/* type nfs      */
#define	NFS6        78			/* type nfs      */
#define	NFS7        79			/* type nfs      */
#define	NFS8        80			/* type nfs      */
#define	NFS9        81			/* type nfs      */
#define	NFS10       82			/* type nfs      */
#define	NFS11       83			/* type nfs      */
#define	NFS12       84			/* type nfs      */
#define	NFS13       85			/* type nfs      */
#define	NFS14       86			/* type nfs      */
#define	NFS15       87			/* type nfs      */

/* Control block sizes */

#define	Ntty	1
#define	Neth	1
#define	Ndgm	1
#define	Ndg	30
#define	Nrpcm	1
#define	Nrpc	20
#define	Nnfsm	1
#define	Nnfs	15
#define	Ntcpm	1
#define	Ntcp	10
#define	Nrfm	1
#define	Nrf	4
#define	Nnam	1
#define	Nnulldev	1

#define	NDEVS	88

/* Declarations of I/O routines referenced */

#ifndef ASM
extern	int	ttyinit();
extern	int	ionull();
extern	int	ttyread();
extern	int	ttywrite();
extern	int	ioerr();
extern	int	ttycntl();
extern	int	ttygetc();
extern	int	ttyputc();
extern	int	ethinit();
extern	int	ethread();
extern	int	ethwrite();
extern	int	ethcntl();
extern	int	ethinter();
extern	int	naminit();
extern	int	namopen();
extern	int	nullopen();
extern	int	nullread();
extern	int	nullwrite();
extern	int	nullcntl();
extern	int	nullgetc();
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
extern	int	dgmopen();
extern	int	dgmcntl();
extern	int	dginit();
extern	int	dgclose();
extern	int	dgread();
extern	int	dgwrite();
extern	int	dgcntl();
extern	int	rpcminit();
extern	int	rpcmopen();
extern	int	rpcmcntl();
extern	int	rpcinit();
extern	int	rpcclose();
extern	int	rpcread();
extern	int	rpcwrite();
extern	int	rpccntl();
extern	int	nfsminit();
extern	int	nfsmopen();
extern	int	nfsmcntl();
extern	int	nfsinit();
extern	int	nfsclose();
extern	int	nfsread();
extern	int	nfswrite();
extern	int	nfsseek();
extern	int	nfscntl();
extern	int	nfsgetc();
extern	int	nfsputc();
#endif ASM


/* Configuration and Size Constants */

#define	NPROC		30		/* number of user processes	*/
#define	NSEM		512		/* number of semaphores		*/
#define	MEMMARK				/* define if memory marking used*/
#define	RTCLOCK				/* now have RTC support		*/
#define	STKCHK				/* resched checks stack overflow*/
#define	NETDAEMON			/* Ethernet network daemon runs	*/
#define	NETBUFS		64		/* number of network buffers	*/

#define	VERSION	"VM-Xinu (Version 8.1)" /* printed at startup */


#define	NBPOOLS	50
#define BPMAXN	512
#define	BPMAXB	8500
/****************************************************************/
/* BPMAXB is an important number.  8500 was chosen so that 	*/
/* everyone would be able to stick some sort of header on a	*/
/* 8k buffer (8192) and still be less than this.  It is used	*/
/* to determine the size of a net.lrgpool network buffer, as 	*/
/* well as RPC buffering sizes, etc.				*/
/*								*/
/*		BE CAREFUL WHEN CHANGING IT			*/
/****************************************************************/


#define	TCPSBS	0x1000			/* TCP send buffer sizes	*/
#define	TCPRBS	0x4000			/* TCP receive buffer sizes	*/

#define	NPORTS	100

#define	TIMERGRAN	1		/* timer granularity, secs/10	*/

#define XSELECT		/* provide support for the xinu select call */

#define CPUUSAGE	/* gather cpu-usage stats */
#define PROFILING	/* execution profiling	*/
#undef INTERRUPT_PROFILING	/* voluntary interrupt profiling	*/

#define CLOCK_INTERRUPTS 256	/* interrupts/second */

/****************************************************************/
/*                                                              */
/*                   PAGING INFORMATION                         */
/*                                                              */
/* NOTE: MUST choose either one to do paging.                   */
/*                                                              */
/*       We recommend using NFS pagging in this release.        */
/*                                                              */
/****************************************************************/
#define NFS_PAGING		/* use NFS paging               */
/*#define XPP_PAGING              /* use remote memory server     */

#if !defined(NFS_PAGING) && !defined(XPP_PAGING)
FATAL: must define either "XPP_PAGING" or "NFS_PAGING"
#endif

/* Authentication info for talking to Unix systems (RPC & NFS) */
#define XINU_UID 32767
#define XINU_GID 32767
