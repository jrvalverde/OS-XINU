
/* $Header: /xinu/xinux/kernel/xinu/src/sys/h/RCS/bootp.h,v 1.1 1991/09/29 18:28:31 sdo Exp $ */

/* 
 * bootp.h - header definitions for bootp protocol
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Mon Jun 11 16:32:12 1990
 *
 * Copyright (c) 1990 Cypress Network Operations Center
 *
 */

/*
 * $Log: bootp.h,v $
 * Revision 1.1  1991/09/29  18:28:31  sdo
 * Initial revision
 *
 * Revision 1.1  90/10/01  21:58:54  sjm
 * Initial revision
 * 
 * Revision 1.6  90/07/10  15:50:51  smb
 * Cleaned up and commented.
 * 
 * Revision 1.5  90/07/10  10:06:24  smb
 * Added external declaration of bpmsg.
 * 
 * Revision 1.4  90/07/03  15:08:40  smb
 * cc11 does not like unsigned chars so removed the unsigned keyword.
 * 
 * Revision 1.3  90/06/27  09:52:49  smb
 * Made definitions of bootp protocol names and ports conditional on DEBUG
 * 
 * Revision 1.2  90/06/20  16:28:49  smb
 * Changed name of magic cookies from MAGIC to RFC1084 and XMAGIC to XINU.
 * 
 * Revision 1.1  90/06/11  16:33:10  smb
 * Initial revision
 * 
 */

/* BOOTP_RECV process info */
#define BOOTP_RECV_PROC bootp_recv
extern	BOOTP_RECV_PROC();
#define BOOTP_RECV_STK  4096
#define BOOTP_RECV_PRIO 100	/* high priority while running */
#define BOOTP_RECV_NAM	"bootp_recv"
#define BOOTP_RECV_ARGS	0,0


#define BOOTPS	"bootps"
#define BOOTPC	"bootpc"
#define NBOOTPS	67
#define NBOOTPC	68

#define SZCHADDR	16    		/* size of client haddr field	*/
#define SZSNAME		64    		/* size of server name field	*/
#define SZFNAME		128    		/* size of file name field	*/
#define SZVENDOR	64    		/* size of vendor spec. field	*/

/* values for op		*/
#define BOOTREQUEST	1
#define BOOTREPLY	2

/* values for htype and hlen	*/
#define ETHERNET	1
#define ELEN		6

/* structure of a BOOTP message */
struct bootp_msg {
    char op;				/* request or reply 		*/
    char htype;				/* hardware type		*/
    char hlen;				/* hardware address length	*/
    char hops;				/* set to zero			*/
    long xid;				/* transaction id		*/
    unsigned short secs;		/* time client has been trying	*/
    short unused;			/* unused			*/
    long ciaddr;			/* client IP address		*/
    long yiaddr;			/* your (client) IP address	*/
    long siaddr;			/* server IP address		*/
    long giaddr;			/* gateway IP address		*/
    char chaddr[SZCHADDR];		/* client hardware address	*/
    char sname[SZSNAME];		/* server host name		*/
    char file[SZFNAME];			/* boot file name		*/
    char vend[SZVENDOR];
};

#define RFC1084		0x63825363	/* vendor magic cookie from 1084*/
#define XINU		0x78696e75	/* vendor magic cookie "xinu"	*/

struct vendor {
    long v_magic;			/* vendor magic cookie		*/
    char v_data[1];
};

struct sub_vendor {			/* vendor sub-field		*/
    char s_tag;				/* tag octet			*/
    char s_length;			/* length			*/
    char s_data[1];			/* length bytes of data		*/
};

/* tags */
#define PAD			0
#define SUBNETMASK		1
#define TIME_OFFSET		2
#define GATEWAY			3
#define TIME_SERVER		4
#define IEN_NAMESERVER		5
#define DOMAIN_NAMESERVER	6
#define LOG_SERVER		7
#define COOKIE_SERVER		8
#define LPR_SERVER		9
#define IMPRESS_SERVER		10
#define RLP_SERVER		11
#define HOSTNAME		12
#define BOOT_FILE_SIZE		13	/* in 512 octet blocks	*/

#define BEGIN_RESERVED		128	/* these may define 	*/
#define END_RESERVED		254	/* site-specific items	*/

#define END			255

#define BOOTP_SIZE	(sizeof(struct bootp_msg))

extern struct bootp_msg *bpmsg;

#define	BOOTP_RESEND	1	/* resend if no reply in 1 sec		*/
#define	BOOTP_MAXRETRY	3	/* give up after a ~8 seconds		*/
