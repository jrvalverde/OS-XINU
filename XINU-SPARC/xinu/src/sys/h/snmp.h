/* snmp.h - strequ, oidequ */

#define	SNMPD		snmpd		/* SNMP server code 		*/
extern  int     	SNMPD();	/* SNMP server daemon 		*/
#define	SNMPSTK		8192		/* SNMP server stack size 	*/
#define	SNMPPRI		20		/* SNMP server priority 	*/
#define SNMPDNAM	"snmpd"		/* SNMP server daemon name 	*/

#define	SMAXOBJID	32		/* max # of sub object ids	*/
#define	OBJSUBIDTYPE	unsigned short	/* type of sub object ids	*/

/* strequ - return TRUE if strings x and y are the same			*/
#define	strequ(x,y)	(strcmp((x), (y)) == 0)	

#define	u_char		unsigned char

struct oid {				/* object identifier		*/
	OBJSUBIDTYPE	id[SMAXOBJID];	/* array of sub-identifiers 	*/
	int		len;		/* length of this object id	*/
};

/*
 * oidequ - check if the lengths of the oid's are the same, then check
 * 	    the contents of the oid's
 */
#define oidequ(x,y)	((x)->len == (y)->len && \
			 blkequ((x)->id, (y)->id, (y)->len * 2))

/* Structure that contains the value of an SNMP variable. 		*/
struct snval {
	unsigned char 	sv_type;	/* variable type		*/
	union {				/* value of var is one of these	*/
		int 	sv_int;		/* variable is one of: integer, */
					/* counter, gauge, timeticks	*/
		struct {		/* variable is a (octet) string	*/
			char *sv_str;	/* string's contents		*/
			int sv_len;	/* string's length		*/
		} sv_str;
		struct 	oid sv_oid;	/* variable is an object id	*/
		IPaddr	sv_ipaddr;	/* variable is an IP address	*/
	} sv_val;
};

/* Functions to access parts of the above snval structure		*/
#define SVTYPE(bl)	((bl)->sb_val.sv_type)
#define SVINT(bl)	((bl)->sb_val.sv_val.sv_int)
#define SVSTR(bl)	((bl)->sb_val.sv_val.sv_str.sv_str)
#define SVSTRLEN(bl)	((bl)->sb_val.sv_val.sv_str.sv_len)
#define SVOID(bl)	((bl)->sb_val.sv_val.sv_oid.id)
#define SVOIDLEN(bl)	((bl)->sb_val.sv_val.sv_oid.len)
#define SVIPADDR(bl)	((bl)->sb_val.sv_val.sv_ipaddr)

/*
 * Each snblist node contains an SNMP binding in one of 2 forms: ASN.1
 * encoded form or internal form.  The bindings list is doubly-linked
 */
struct snbentry {
	struct 	oid sb_oid;		/* object id in internal form	*/
	struct 	snval sb_val;		/* value of the object		*/
	u_char 	*sb_a1str;		/* ASN.1 string containing the 	*/
					/* object id and its value	*/
	int	sb_a1slen;		/* length of the ASN.1 string	*/
	struct 	snbentry *sb_next;	/* next node in the bind list	*/
	struct 	snbentry *sb_prev;	/* previous node in the list	*/
};

/* Structure that holds a complete description of an SNMP request 	*/
struct req_desc {
	u_char	reqtype;		/* request type			*/
	u_char	reqid[10];		/* request identifier		*/
	int	reqidlen;		/* length of the identifier	*/
	u_char	err_stat;		/* error status			*/
	u_char	err_idx;		/* error index			*/
	int	err_stat_pos;		/* position of error status in	*/
					/* the ASN.1 encoding		*/
	int	err_idx_pos;		/* position of error index	*/
	int	pdutype_pos;		/* position of pdu type		*/
	struct 	snbentry *bindlf;	/* front of bindings list	*/
	struct	snbentry *bindle;	/* end of bindings list 	*/
};

#define SNMPMAXSZ	U_MAXLEN 	/* max SNMP request size	*/
#define SNMPPORT	161		/* SNMP server UDP port		*/

/* SNMP error types							*/
#define SNMP_OK		0		/* no error			*/
#define	SERR_TOO_BIG	1		/* reply would be too long 	*/
#define SERR_NO_SUCH 	2		/* no such object id exists	*/
#define SERR_BAD_VALUE	3		/* bad value in a set operation	*/

#define SVERS_LEN	1		/* SNMP version is 1 byte long	*/
#define SVERSION	0		/* current SNMP version		*/

#define SCOMM_STR	"public"	/* SNMP community string	*/

/* operations to be applied to an SNMP object 				*/
#define	SOP_GET		1		/* get operation 		*/
#define SOP_GETN	2		/* getnext operation		*/
#define SOP_GETF	3		/* get first operation		*/
#define SOP_SET		4		/* set operation		*/

/* standard version and community string -- backwards */
#define SNVCBACK_CHARS 	\
	'c', 'i', 'l', 'b', 'u', 'p', 0x06, 0x04 /* ASN1_OCTSTR	*/,	\
	SVERSION, SVERS_LEN, 0x02		 /* ASN1_INT	*/

/* SNMP client return codes						*/
#define SCL_OK		0	/* good response -- no errors 		*/
#define SCL_OPENF	1	/* open fails 				*/
#define SCL_WRITEF	2	/* write fails 				*/
#define SCL_NORESP	3	/* no response from server		*/
#define SCL_READF	4	/* read fails				*/
#define SCL_BADRESP	5	/* bad response				*/
#define SCL_BADREQ	6	/* bad request				*/

/* Table specific constants						*/
#define SNUMF_AETAB	4	/* 4 fields in an Addr Entry 		*/
#define SNUMF_ATTAB	3	/* 3 fields in an Addr Transl. Entry	*/
#define SNUMF_IFTAB	21	/* 21 fields in an Interface Entry	*/
#define SNUMF_RTTAB	10	/* 10 fields in a Route Table Entry	*/
#define SNUMF_TCTAB	5	/* 5 fields in a TCP Connection Entry	*/

#define	SAE_OIDLEN	3	/* all sae variables start with 4.20.1	*/
#define SAT_OIDLEN	3	/* all sat variables start with 3.1.1	*/
#define SIF_OIDLEN	3	/* all sif variables start with 2.2.1	*/
#define SRT_OIDLEN	3	/* all srt variables start with 4.21.1	*/
#define STC_OIDLEN	3	/* all stc variables start with 6.13.1	*/
