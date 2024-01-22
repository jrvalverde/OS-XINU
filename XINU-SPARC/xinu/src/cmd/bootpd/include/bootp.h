
/* $Header: /usr/src/local/etc/bootpd/include/RCS/bootp.h,v 1.5 90/07/27 10:34:49 trinkle Exp $ */

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
 * $Log:	bootp.h,v $
 * Revision 1.5  90/07/27  10:34:49  trinkle
 * Xinu additions from smb.
 * 
 * Revision 1.4  90/07/12  09:58:35  smb
 * Commented.
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

#ifdef DEBUG

#define BOOTPS	"tbootps"
#define BOOTPC	"tbootpc"
#define NBOOTPS	2067
#define NBOOTPC	2068

#else

#define BOOTPS	"bootps"
#define BOOTPC	"bootpc"
#define NBOOTPS	67
#define NBOOTPC	68

#endif

#define SZCHADDR	16		/* size of client hw addr field	*/
#define SZSNAME		64    		/* size of server name field	*/
#define SZFNAME		128    		/* size of file name field	*/
#define SZVENDOR	64    		/* size of vendor specific field*/

/* values for op		*/
#define BOOTREQUEST	1
#define BOOTREPLY	2

/* values for htype and hlen	*/
#define ETHERNET	1
#define ELEN		6

/* structure of a BOOTP message */
struct bootp_msg {
    unsigned char op;			/* request or reply 		*/
    unsigned char htype;		/* hardware type		*/
    unsigned char hlen;			/* hardware address length	*/
    unsigned char hops;			/* set to zero			*/
    unsigned long xid;			/* transaction id		*/
    unsigned short secs;		/* time client has been trying	*/
    short unused;			/* unused			*/
    unsigned long ciaddr;		/* client IP address		*/
    unsigned long yiaddr;		/* your (client) IP address	*/
    unsigned long siaddr;		/* server IP address		*/
    unsigned long giaddr;		/* gateway IP address		*/
    unsigned char chaddr[SZCHADDR];	/* client hardware address	*/
    char sname[SZSNAME];		/* server host name		*/
    char file[SZFNAME];			/* boot file name		*/
    char vend[SZVENDOR];
};

/* definitions for magic cookie values */
#define RFC1084		0x63825363	/* vendor magic cookie from 1084*/
#define XINU		0x78696e75	/* vendor magic cookie "xinu"	*/

struct vendor {
    unsigned long magic;		/* vendor magic cookie		*/
    char data[1];
};

struct vendor_subfield {		/* vendor sub-field		*/
    unsigned char tag;			/* tag octet			*/
    unsigned char length;		/* length			*/
    char data[1];			/* length bytes of data		*/
};

/* RFC 1084 tags */
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

/* Xinu tags
 *   these are defined for xboxes to get needed information - whatever else
 *   is desired can be gotten from an RFC1084 BOOTP after the xbox code is
 *   running.
 *
 *   Note that all but tag 4 supply only one piece of information; tag 4
 *	supplies 4 (unused octets should be 0).
 *
 *   All tags (except 0 and 255) will be followed by one data length octet
 *   as the tags from RFC1084 are.
 */
#define X_PAD			0	/* pad 				*/
#define X_ETHER_SUBNETMASK	1	/* ethernet subnet mask		*/
#define X_ETHER_GATEWAY		2	/* gateway/router on ethernet	*/
#define X_CYPRESS_SUBNETMASK	3	/* cypress subnet mask		*/
#define X_CYPRESS_ADDRESS	4	/* cypress IP address		*/
#define X_CYPRESS_IMPLETS	5	/* up to 4 implet ids for lines	*/
#define X_TIME_OFFSET		6	/* seconds from UTC		*/
#define X_TIME_SERVER		7	/* one time server IP address	*/
#define X_END			255	/* end of list			*/

#define BOOTP_SIZE	(sizeof(struct bootp_msg))
