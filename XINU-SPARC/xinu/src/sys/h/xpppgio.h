/*---------------------------------------------------------------------------
 * xpppgio.h - Xinu Paging Protocol page in/out head file
 *---------------------------------------------------------------------------
 */

#define MSGLEN (sizeof(struct message)-sizeof(int))
					/* not include "fram_num" field	*/
#define TIMOUT		2		/* max time waiting for reply	*/
					/*(the diff of clktime(sec))	*/
#define MAXPRLLEN	1		/* max pending req list length	*/
#define LIMIT		5		/* normal sending times		*/
#define MAXSEND		15		/* max sending times for term	*/
					/* strt rqt			*/
#define MAXMNO		9999		/* max message number		*/
#define TIMEDOUT	0		/* parameter for call abort	*/
#define ERROR		1		/* ,,	,,	,,	,,	*/
#define OKRES		0		/* OK response from server	*/
#define ERRES		-1		/* ERROR response from server	*/
#define MTBLERR		-3		/* Machine table error from PS	*/

#define RQNUM		(2*NPROC)	/* number of request nodes	*/
#define PROCRQHD	0		/* proc. create/term list head	*/
#define PROCRQTL	1		/* proc. create/term list tail	*/
#define PGINRQHD	2		/* page_in request list head	*/
#define PGINRQTL	3		/* page_in request list tail	*/
#define PENDINGHD	4		/* pending request list head	*/
#define PENDINGTL	5		/* pending request list tail	*/

#define CREATE_REQ	1		/* proc. creation request	*/
#define CRSTOR_REQ	2		/* proc. create&page store rqt	*/
#define STORE_REQ	3		/* page store request		*/
#define FETCH_REQ	4		/* page fetch request		*/
#define TERMIN_REQ	5		/* proc. termination request	*/

#define CREATE_RSP	6		/* creat response		*/
#define	CRSTOR_RSP	7		/* creat & store		*/
#define	STORE_RSP	8		/* store response		*/
#define	FETCH_RSP	9		/* fetch response		*/
#define	TERMIN_RSP	10		/* terminate response		*/
#define BADREQ		12		/* bad request			*/

#define START_REQ	13		/* system start request		*/
#define END_REQ		14		/* system end request		*/
#define	START_RSP	15		/* start response		*/
#define	END_RSP		16		/* end response			*/
#define	NACK_REQ	17		/* NACK request when PS rcvd	*/
					/* out of sequence message	*/

#define PGSEND		pgsdr		/* paging sender routine	*/
#define PGSSTK		4096		/* paging sender stack size	*/
#define PGSPRI		97		/* paging sender priority	*/
#define PGSNAM		"pgsdr"		/* paging sender name		*/
#define PGSARGC		0		/* paging sender argument count	*/

#define PGRCVR		pgrcvr		/* paging receiver routine	*/
#define PGRSTK		4096		/* paging receiver stack size	*/
#define PGRPRI		98		/* paging receiver priority	*/
#define PGRNAM		"pgrcvr"	/* paging receiver name		*/
#define PGRARGC		0		/* paging sender argument count	*/


extern	int		PGSEND(), PGRCVR();

struct message {			/* uniform request/reply msg	*/
	int	msg_num;		/* message number		*/
	int	msg_type;		/* message type			*/
	char	mach_id[4];		/* machine id			*/
	int	pa_id;			/* process or address-space id	*/
	int	page_num;		/* page number			*/
	int	page_size;		/* 512/8192 etc.		*/
	int	msg_code;		/* message code			*/
	int	fram_num;		/* frame num (not sent to PS)	*/
};
 
extern	int	sendproc;		/* sender process id		*/
extern	int	recvproc;		/* reciever process id		*/
extern	long	prloldest;		/* oldest ts on pending list	*/
extern	int	prlsize;		/* size of pending list		*/
extern	int	msgcount;		/* msg counter for msg number	*/

extern	int	msgpool;		/* message buf pool id		*/
extern	int	nodepool;		/* request node buf pool id	*/
extern	char	*rbuf;			/* receiving buffer		*/
extern	char	*rmsg;			/* receiving message buffer	*/
extern	int	psready;		/* PS accepts Xinu start request*/
extern	char	machineid[];		/* this machines IP address	*/

extern	struct	fte *flmptr;		/* pointer to fte on M-list	*/
