/* mib.h */

/* mib information structure: 1 per mib entry;  Initialized in snmib.c	*/
struct mib_info {
	char	*mi_name;	/* name of mib variable in English	*/
	char	*mi_prefix;	/* prefix in English (e.g., "tcp.")	*/
	struct 	oid mi_objid;	/* object identifier			*/
	int	mi_vartype;	/* type: integer, aggregate, octet str	*/
	Bool	mi_writable;	/* is this variable writable?		*/
	Bool	mi_varleaf;	/* is this a leaf with a single value	*/
	int	(*mi_func)();	/* function to implement get/set/getnext*/
	int	mi_param;	/* parameter used with function		*/
	struct 	mib_info *mi_next; /* pointer to next var. in lexi-order*/
};

extern 	struct mib_info mib[];	/* array with one entry per MIB variable*/
extern	int mib_entries;	/* number of entries in mib array	*/

/* Information about MIB tables.  Contains functions to implement 	*/
/* operations upon variables in the tables.				*/
struct tab_info {		
	int	(*ti_get)();	/* get operation			*/
	int	(*ti_getf)();	/* get first operation			*/
	int	(*ti_getn)();	/* getnext operation			*/
	int	(*ti_set)();	/* set operation			*/
	int	(*ti_match)();	/* match operation: is a given oid 	*/
				/*    found in the current table?	*/
	struct	mib_info *ti_mip; /* pointer to mib information record	*/
};

extern 	struct tab_info tabtab[]; /* table of MIB table information	*/

#define	LEAF		1	/* This MIB variable is a leaf.		*/
#define	NLEAF		0	/* This MIB variable is not a leaf.	*/

#define	T_TCPTABLE		0x0	/* var is the TCP conn. table	*/
#define	T_RTTABLE		0x1	/* var is the routing table	*/
#define	T_AETABLE		0x2	/* var is the address entry tbl	*/
#define	T_ATTABLE		0x3	/* var is the addr translat tbl	*/
#define	T_IFTABLE		0x4	/* var is the interface table	*/
/*#define T_EGPTABLE		0x5*/	/* var is the egp table		*/
#define	T_AGGREGATE		0x6	/* var is an aggregate		*/

/* this type specifies in mib.c that the object is a table.  The value is
   different than any of the ASN.1 types that SNMP uses. */
#define T_TABLE			01

