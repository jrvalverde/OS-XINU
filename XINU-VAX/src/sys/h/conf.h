/* conf.h (GENERATED FILE; DO NOT EDIT) */

#define	NULLPTR	(char *)0

/* Device table declarations */
#ifndef	ASM
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
#endif


/* Device name definitions */

#define	CONSOLE     0			/* type tty      */
#define	ETHER       1			/* type eth      */
#define	INTERNET    2			/* type dgm      */
#define	DGRAM1      3			/* type dg       */
#define	DGRAM2      4			/* type dg       */
#define	DGRAM3      5			/* type dg       */
#define	DGRAM4      6			/* type dg       */
#define	DGRAM5      7			/* type dg       */
#define	DGRAM6      8			/* type dg       */
#define	RFILSYS     9			/* type rfm      */
#define	RFILE1      10			/* type rf       */
#define	RFILE2      11			/* type rf       */
#define	RFILE3      12			/* type rf       */
#define	RFILE4      13			/* type rf       */
#define	RFILE5      14			/* type rf       */
#define	RFILE6      15			/* type rf       */
#define	RFILE7      16			/* type rf       */
#define	RFILE8      17			/* type rf       */
#define	RFILE9      18			/* type rf       */
#define	RFILE10     19			/* type rf       */
#define	NAMESPACE   20			/* type nsys     */

/* Control block sizes */

#define	Ntty	1
#define	Neth	1
#define	Ndgm	1
#define	Ndg	6
#define	Nrfm	1
#define	Nrf	10
#define	Nnsys	1

#define	NDEVS	21

/* Declarations of I/O routines referenced */

#ifndef	ASM
extern	int	vcuinit();
extern	int	ttyopen();
extern	int	ionull();
extern	int	ttyread();
extern	int	vcuwrite();
extern	int	ioerr();
extern	int	vcucntl();
extern	int	ttygetc();
extern	int	vcuputc();
extern	int	vcuiin();
extern	int	vcuoin();
extern	int	ethinit();
extern	int	ethread();
extern	int	ethwrite();
extern	int	ethinter();
extern	int	dgmopen();
extern	int	dgmcntl();
extern	int	dginit();
extern	int	dgclose();
extern	int	dgread();
extern	int	dgwrite();
extern	int	dgcntl();
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
#endif


/* Configuration and Size Constants */

#define	NPROC	100			/* number of user processes	*/
#define	NSEM	200			/* total number of semaphores	*/
#define	MEMMARK				/* define if memory marking used*/
#define	RTCLOCK				/* system has a real-time clock	*/
#define	STKCHK				/* resched checks stack overflow*/
#define	NETDAEMON			/* Ethernet network daemon runs	*/
#define	GATEWAY     128,10,2,70		/* Gateway address		*/
#define	TSERVER     "128.10.2.3:37"	/* Time server address		*/
#define	RSERVER     "128.10.2.8:2001"	/* Remote file server address	*/
#define	DSERVER     "128.10.2.8:53"	/* Domain Name server address	*/
#define	VERSION     "7.6 (03/08/87)"	/* label printed at startup	*/
					/* (vers7: Diskless Workstation)*/
/************************************************************************/
/*			Target machine type				*/
/* To reconfigure for a different target VAX system, change the		*/
/* following statement to define one of:				*/
/*									*/
/*	VAX8600 VAX785 VAX780 VAX750 VAX730 VAX725 UVAXII UVAXI		*/
/*									*/
/* Yes folks, we actually downloaded Xinu into an 8600 (but there's	*/
/* no Unibus Ethernet driver.						*/
/*									*/
/************************************************************************/

#define	UVAX				/* machine type			*/

#ifdef	UVAX
#define	SYSTEM		"MicroVAX"	/* name printed at startup	*/
#endif
#ifdef	VAX725
#define	SYSTEM		"VAX 11/725"
#endif
#ifdef	VAX730
#define	SYSTEM		"VAX 11/730"
#endif
#ifdef	VAX750
#define	SYSTEM		"VAX 11/750"
#endif
#ifdef	VAX780
#define	SYSTEM		"VAX 11/780"
#endif
#ifdef	VAX785
#define	SYSTEM		"VAX 11/785"
#endif
#ifdef	VAX8600
#define	SYSTEM		"VAX 8600"
#endif
