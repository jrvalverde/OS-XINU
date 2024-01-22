
/* 
 * psv.h - The Xinu Page Sever header file
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Jul 27 17:04:08 1990
 *
 * Copyright (c) 1990 Jim Griffioen
 */

#define		LOGPATH		"log"			/* service log  */

/* NOTE: MAX_P_ENT, and MAX_G_ENT must be choosen in such a way that	*/
/* (MAX_P_ENT), (MAX_P_ENT-2), (MAX_G_ENT), (MAX_G_ENT-2) are all prime	*/
/* numbers.  This is crucial for the double hashing algorithm.		*/
#define		MAX_M_ENT	16   			/* M table size	*/
#define		MAX_P_ENT	271   			/* P hash size	*/
#define		MAX_G_ENT	4129   			/* G hash size	*/
#define		MAX_PER_PAGE	10			/* # page ptrs  */
#define		MAX_D_BLK	20000  			/* max # pages  */
#define		MAX_M_NO	9999			/* max msg num  */
#define		PSIZE		1024			/* 1K pagesize  */
#define		MSGLEN		(sizeof(struct message) - MAX_PER_PAGE*PSIZE)

#define		VALID		0			/* valid PID	*/
#define		INVALID		(-1)			/* invalid PID	*/

#define		INSERT		0			/* hash insert	*/
#define		FIND		1			/* hash find	*/
#define 	NULLARY		(char *)0		/* null array ptr*/
    
					/* function return codes	*/
#define		ERROR		(-1)			/* return code  */
#define		MTBLERR		(-3)			/* M-tbl error  */
#define		ROK		0			/* return code	*/
#define		PFULL		1			/* P table full */
#define		NOEXIST		2			/* PID nonexist */
#define		GFULL		3			/* G table full */
#define		NOPAGES		4			/* out of 1k blk*/

					/* message types		*/
#define		CREATE_REQ	1			/* creat request*/
#define		CRSTOR_REQ	2			/* creat & store*/
#define		STORE_REQ	3			/* store request*/
#define		FETCH_REQ	4			/* fetch request*/
#define		TERMIN_REQ	5			/* terminate req*/
#define		CREATE_RSP	6			/* creat respons*/
#define		CRSTOR_RSP	7			/* creat & store*/
#define		STORE_RSP	8			/* store respons*/
#define		FETCH_RSP	9			/* fetch respons*/
#define		TERMIN_RSP	10			/* terminate res*/
#define		BADREQ		12			/* bad request	*/
#define		START_REQ	13      		/* start request*/
#define		END_REQ		14      		/* end request	*/
#define		START_RSP	15      		/* start response*/
#define		END_RSP		16      		/* end response */
#define		NACK_REQ	17			/* NACK request when
							   received out of
							   sequence msg */
#define		TEST_REQ	20
#define		TEST_RSP	21

					/* store request flags		*/
#define		NEW		1			/* malloc new	*/
#define		OLD		2			/* use old ones */

#define	TRUE	1			/* Boolean TRUE			*/
#define	FALSE	0			/* Boolean FALSE		*/

#define	DBGHIGH		125
#define	DBGMEDHIGH	100
#define	DBGMED		75
#define	DBGMEDLOW	50
#define	DBGLOW		25

extern int curdbglevel;

struct d_block {		/* data block(s) in which to store page */
	char data[PSIZE];	/* actual data from page		*/
	struct d_block *next;	/* link to next if on free list		*/
};

struct m_entry {		/* M (machine) message sequence number  */
	int valid;		/* table entry				*/
	int tstamp;		/* time stamp for the client(machine)	*/
	int msg_num;		/* message number last received		*/
	char mach_id[4];	/* machine IP address			*/
};
    
struct p_entry {		/* P (process) hash table entry		*/
	int mach_id;		/* internet machine address (net byte)	*/
	int mach_tstp;		/* machine time stamp			*/
	int proc_id;		/* per client process id (PID)		*/
	int valid_flag;		/* indicates if current data is valid	*/
	int timestamp;		/* used to distinguish old and new PID  */
}; 

struct g_entry {		/* G (global) page hash table entry	*/
	int mach_id;		/* internet machine address (net byte)	*/
	int proc_id;		/* per client process id (PID)		*/
	int page_num;		/* page number to store			*/
	int valid_flag;		/* indicates if current data is valid	*/
	int timestamp;		/* used to see if old or new entry	*/
	struct p_entry *p_ent_ptr;	/* back pointer to P entry	*/
	int dblk_cnt; 		/* number of 1k blocks to be stored	*/
	struct d_block *d_blk_ptr[MAX_PER_PAGE];	/* point to data*/
};

struct message {		/* message from William-Protocol layer	*/
	int msg_num;		/* unique per client number		*/
	int msg_type;		/* fetch, store, create or terminate	*/
	char mach_id[4];	/* internet machine address (net byte)	*/
	int proc_id;		/* per client process id (PID)		*/
	int page_num;		/* page number to store			*/
	int page_size;		/* size of the page to be stored/fetched*/
	int ok_error;		/* reply OK or ERROR code		*/
	char data[MAX_PER_PAGE*PSIZE];	/* actual page data (optional)	*/
};

extern struct m_entry *M;    					/* M table */
extern struct p_entry *P;    					/* P table */
extern struct g_entry *G;    					/* G table */
extern struct d_block *D;    				/* the free list  */
extern struct tmessage *msglist[];	/* saved message list		*/
extern struct d_block *free_list;      /* pointer to head of free list */
extern struct sockaddr_in from;        /* IP address of a client port  */

extern FILE *logdesc;		       /* log file descriptor	       */

extern short curmidx;			/* current M-table index	*/

extern int	store_num;
extern int	fetch_num;
extern int	cur_ststrt;
extern int 	cur_ftstrt;
extern int	store_actime;
extern int	fetch_actime;
extern int	sttime,fttime,maxst,minst,maxft,minft;
extern int	llft,llst,llftacc,llstacc;
