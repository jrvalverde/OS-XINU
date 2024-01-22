/* daemon.h */

/* FINGERD process info */

#define	FINGERD		fingerd		/* FINGERD process		*/
extern	int		FINGERD();
#define	FNGDSTK		4096		/* stack size for FINGERD	*/
#define	FNGDPRI		20		/* FINGERD priority		*/
#define	FNGDNAM		"fingerd"	/* name of FINGERD process	*/
#define	FNGDARGC	0		/* count of args to FINGERD	*/

/* TCP ECHOD process info */

#define	TCPECHOD	tcpechod	/* TCP ECHOD daemon		*/
#define	TCPECHOP	tcpechop	/* TCP ECHO process		*/
extern	int		TCPECHOD();
extern	int		TCPECHOP();
#define	TCPECHOSTK	4096		/* stack size for TCP ECHOD	*/
#define	TCPECHOPRI	20		/* ECHOD priority		*/
#define	TCPECHODNAM	"tcpechod"	/* name of TCP ECHOD process	*/
#define	TCPECHOPNAM	"tcpechop"	/* name of TCP ECHOP process	*/

/* Xinu logind process info */

#define	XLOGIND		xlogind		/* XLOGIN daemon		*/
extern	int		XLOGIND();
#define	XLOGINSTK	4096		/* stack size for xlogin	*/
#define	XLOGINPRI	20		/* xlogin priority		*/
#define	XLOGINDNAM	"xlogind"	/* name of xlogind process	*/

/* UDP echo server process info */

#define	UDPECHOD	udpechod	/* UDPECHOD daemon		*/
extern	int		UDPECHOD();
#define	UDPECHODSTK	4096		/* stack size for udpecho	*/
#define	UDPECHODPRI	50		/* udpecho priority		*/
#define	UDPECHODNAM	"udpechod"	/* name of udpecho process	*/

/* UDP discard server process info */

#define	UDPDISCARD	udpdiscard	/* UDP discard daemon		*/
extern	int		UDPDISCARD();
#define	UDPDISCARDSTK	4096		/* stack size for udpdiscard	*/
#define	UDPDISCARDPRI	50		/* udpdiscd priority		*/
#define	UDPDISCARDNAM	"udpdiscard"	/* name of udpecho process	*/
