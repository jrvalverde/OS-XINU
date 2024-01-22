/* 
 * psv.c - The Xinu Page Server
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Jul 27 17:03:17 1990
 *
 * Copyright (c) 1990 Jim Griffioen
 */


/*----------------------------------------------------------------------------
 * psv.c
 *
 *  The Xinu version 8 Page Server utilizes 2 hash tables for quick access
 *  to the stored pages.  The P[] table holds information about processes 
 *  that are currently using the page server.  The G[] table holds the 
 *  actual page data, plus information about the pages process.  A double
 *  hashing algorithm is used to index into these tables.
 *----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/timeb.h>

#include "psv.h"
#include "tp.h"

/*-------- Global data ---------*/

struct m_entry *M;    /* M table 				*/
struct p_entry *P;    /* P table 				*/
struct g_entry *G;    /* G table 				*/
struct d_block *D;    /* the free list		        */
struct d_block *free_list;      /* pointer to the head of the free list	*/
struct tmessage *msglist[MAX_M_ENT];	/* saved message list		*/
struct sockaddr_in from; 	/* IP address of a client port 		*/
int num_d_blks;

short curmidx;			/* current M-table index		*/

/*------------------------------*/

int                pid;	
struct timeval	   ps_begin, ps_end;	/* to calculate efficiency	*/
struct timezone    tzdummy;		/* req'd for 'gettimeofday'	*/
char		   *FindHost(), *Which(), *ErrNo();
struct d_block     *Init_Free();	/* creates free list of 1k pages*/


int fd;
char *fp;
#define		SERVNAME	"xinupageserver"

/*#define		NOFORK		1*/
/* define NOFORK if you don't 	*/
/* want it to fork a child	*/

/*#define		DEBUG */
/* compile in debugging statements */

int	curdbglevel = DBGHIGH;
FILE	*dbgdevice;
char	logfilename[256];
int	garbcollectindex=0;
char	stopgarbcollect;
int	dblkopt = 100000;
int	portopt = (-1);

int SOCK_FD;


#ifdef DEBUG				/* define when tracing enabled  */
#define		DMPGTAB		50	/* command to dump G table	*/
#define		DMPPTAB		51	/* command to dump P table	*/
#define		DMPGNP		52	/* dump both P and G tables	*/
#endif			

int	store_num = 0;
int	fetch_num = 0;
int 	cur_ststrt;
int	cur_ftstrt;
int	store_actime = 0;
int	fetch_actime = 0;
int	fttime, sttime,maxst=0,maxft=0,minst=100,minft=100;
int	llft, llst, llftacc=0, llstacc=0;

/*----------------------------------------------------------------------------
 * MAIN() - Xinu8 Page Server driver routine
 *----------------------------------------------------------------------------
 */

main(argc, argv)
     int argc;
     char *argv[];
{
    int i;
    
    strcpy(logfilename, "");
    
    for (i=1; i<argc; i++) {
	if (strcmp(argv[i], "-h") == 0) {
	    help(argv[0]);
	}
	else if (strcmp(argv[i], "-help") == 0) {
	    help(argv[0]);
	}
	else if (strcmp(argv[i], "-dh") == 0) {
	    curdbglevel = DBGHIGH;
	}
	else if (strcmp(argv[i], "-dmh") == 0) {
	    curdbglevel = DBGMEDHIGH;
	}
	else if (strcmp(argv[i], "-dm") == 0) {
	    curdbglevel = DBGMED;
	}
	else if (strcmp(argv[i], "-dml") == 0) {
	    curdbglevel = DBGMEDLOW;
	}
	else if (strcmp(argv[i], "-dl") == 0) {
	    curdbglevel = DBGLOW;
	}
	else if (strcmp(argv[i], "-lf") == 0) {
	    if (argc > (i+1))
		strcpy(logfilename, argv[i+1]);
	    else {
		fprintf(stderr, "%s: no log file given\n");
		help(argv[0]);
		exit(1);
	    }
	    i++;
	}
	else if (strcmp(argv[i], "-nd") == 0) {
	    if (argc > (i+1)) {
		if ((dblkopt = atoi(argv[i+1])) <= 0) {
		    fprintf("%s: invalid # of dblks %d\n", argv[0], dblkopt);
		    help(argv[0]);
		    exit(1);
		}
	    }
	    else {
		fprintf(stderr, "%s: no dblock count given\n");
		help(argv[0]);
		exit(1);
	    }
	    i++;
	}
	else if (strcmp(argv[i], "-p") == 0) {
	    if (argc > (i+1)) {
		if ((portopt = atoi(argv[i+1])) <= 0) {
		    fprintf("%s: invalid UDP port %d\n", argv[0], portopt);
		    help(argv[0]);
		    exit(1);
		}
	    }
	    else {
		fprintf(stderr, "%s: no UDP port given\n");
		help(argv[0]);
		exit(1);
	    }
	    i++;
	}
	else {
	    fprintf(stderr, "%s: unknown option %s\n",
		    argv[0], argv[i]);
	    help(argv[0]);
	    exit(1);
	}
    }
    
    initdbgdevice();
    
#ifndef NOFORK
    if ((pid=fork()) == 0) {	/* fork a child process		*/
#endif
	
	Init();			/* initialize all data structs	*/
	Do_Paging();		/* child executes		*/
#ifndef NOFORK
    } else {
	if (pid < 0) {
	    psvlog(DBGHIGH, "error forking child process\n");
	    psvlog(DBGHIGH, "pid returned by fork = %d\n", pid);
	    psvlog(DBGHIGH, "there is probably not enough swap space\n");
	    psvlog(DBGHIGH, "kill a few gnumacs sessions to make room\n");
	    psvlog(DBGHIGH, "psv is quiting\n");
	}
	exit(0);
    }
#endif			
    
}

/*----------------------------------------------------------------------------
 * NET_TO_HOST() - Change header info from network to host byte order
 *----------------------------------------------------------------------------
 */
Net_to_Host(p)
     struct message *p;
{
    int  i;
    
    p->msg_num   = ntohl(p->msg_num);	/* change entire header	*/
    p->msg_type  = ntohl(p->msg_type);
    p->proc_id   = ntohl(p->proc_id);
    p->page_num  = ntohl(p->page_num);
    p->page_size = ntohl(p->page_size);
    p->ok_error  = ntohl(p->ok_error);
    return(ROK);
}

/*----------------------------------------------------------------------------
 * HOST_TO_NET() - Change header info from host to network byte order
 *----------------------------------------------------------------------------
 */
Host_to_Net(p)
     struct message *p;
{
    int  i;
    
    p->msg_num   = htonl(p->msg_num);	/* change entire header	*/
    p->msg_type  = htonl(p->msg_type);
    p->proc_id   = htonl(p->proc_id);
    p->page_num  = htonl(p->page_num);
    p->page_size = htonl(p->page_size);
    p->ok_error  = htonl(p->ok_error);
    return(ROK);
    
}
/*----------------------------------------------------------------------------
 * INIT() - Initialize global data structures
 *----------------------------------------------------------------------------
 */

Init()
     
{
    char *bufptr;
    
    SOCK_FD = ListenUdp();		/* set up socket to listen	*/
    
    M = (struct m_entry *) malloc(sizeof(struct m_entry)*MAX_M_ENT);
    P = (struct p_entry *) malloc(sizeof(struct p_entry)*MAX_P_ENT);
    G = (struct g_entry *) malloc(sizeof(struct g_entry)*MAX_G_ENT);
    bufptr = NULL;
    num_d_blks = ((MAX_D_BLK < dblkopt) ? MAX_D_BLK : dblkopt);
    while ((bufptr==NULL) && (num_d_blks > 0)) {
	bufptr = (char *) malloc(sizeof(struct d_block)*num_d_blks);
	num_d_blks -= 500;
    }
    num_d_blks += 500;
    
    psvlog(DBGHIGH, "Server has %d Kbytes of storage\n",
	   ((num_d_blks)*sizeof(struct d_block))/1024);
    psvlog(DBGHIGH, "Server has %d d_blocks of storage\n",
	   num_d_blks);
    
    D = (struct d_block *) bufptr;
    
    if ((M == (struct m_entry *)NULL) ||
	(P == (struct p_entry *)NULL) ||
	(G == (struct g_entry *)NULL) ||
	(D == (struct d_block *)NULL)) {
	psvlog(DBGHIGH, "error reserving space for M,P,G,D\n");
	exit(1);
    }
    
    psvlog(DBGHIGH, "Initializing Data Structures: please wait\n",
	   num_d_blks);
    
    Set_Data_Lim();			/* give the child some room	*/
    InitTPbuff();			/* init Transport Proto. buffers*/
    Init_Mtable();			/* initialize M table		*/
    Init_Ptable();			/* initialize P hash table	*/
    Init_Gtable();			/* initialize G hash table	*/
    free_list = Init_Free();	/* set up 1K free list of pages */
    
    psvlog(DBGHIGH, "Page server started, curpid = %d\n", getpid());
}


/*----------------------------------------------------------------------------
 * currtime() - millisecons
 *----------------------------------------------------------------------------
 */

currtime()
{
    struct timeb timeb, *timebp;
    
    timebp = &timeb;
    ftime(timebp);
    return( (timebp->time%100000) * 1000 + timebp->millitm);
    /* take the four least significant digits of the
       time since the epoch in secoonds */
}

/*----------------------------------------------------------------------------
 * DO_PAGING() - Read packets and service requests
 *----------------------------------------------------------------------------
 */

Do_Paging()
     
{
    
    int    seq, status, msize, hdr_len, msg_len, dblk_num;
    struct d_block   *parray[MAX_PER_PAGE];
    struct message msg, *mptr;
    int 		j;
    struct tmessage	*tmpmsg;
    struct tmessage	*pnextmsg;
    char		getnewmsg;
    TPReadListType	*tprlt;
    
    
    mptr = &msg; 
    msg_len = sizeof(struct message);	/* only calculate once  */
    curmidx = 0;
    
    while (1)    {				/* forever (hopefully)	*/
	
#ifdef DEBUG
	if (msglist[curmidx] != (struct tmessage *)NULL) {
	    psvlog(DBGMED, "need msg #%d, msg #%d (%d)is waiting\n", M[curmidx].msg_num+1,
		   msglist[curmidx]->tprdlstptr->msgno,
		   msglist[curmidx]->tprdlstptr->msgp->msg_num);
	}
#endif
	if (((msglist[curmidx] != (struct tmessage *)NULL) &&
	     (msglist[curmidx]->tprdlstptr->msgno ==
	      (M[curmidx].msg_num+1)))){
	    /* got msg out of order but saved it */
	    getnewmsg = FALSE;
	    tprlt = msglist[curmidx]->tprdlstptr;
	    mptr = tprlt->msgp;
#ifdef DEBUG
	    psvlog(DBGLOW, "mptr->msg_num = %d\n", mptr->msg_num);
	    psvlog(DBGLOW, "curmidx = %d\n", curmidx);
#endif
	    for (j=0; j < tprlt->dblknum; j++)
		parray[j] = tprlt->datap[j];
	    dblk_num = tprlt->dblknum;
	    bcopy(&(tprlt->from), &from,
		  sizeof(struct sockaddr_in));
	    tmpmsg = msglist[curmidx];
	    msglist[curmidx] = tmpmsg->nextmsg;
	    free(tmpmsg);
	}
	else {
	    getnewmsg = TRUE;
	}
	
	if (getnewmsg) {
	    while (TPSread(&mptr, parray, &from, &dblk_num)
		   != ROK)	/* get msg*/
		{
		    psvlog(DBGHIGH, " *** TP buffers full! ***\n");
		}
	    Net_to_Host(mptr);	/* header in net byte   */
	}
	
#ifdef DEBUG
	psvlog(DBGMEDLOW, "Message from TPSread :\n");
	psvlog(DBGMED, "PS receives msg_num = %d type = %d\n",
	       mptr->msg_num, mptr->msg_type);
	print_msg(DBGMEDLOW, mptr);
#endif
	
#ifdef TIMING
	if (mptr->msg_type == STORE_REQ){
	    store_num++;
	    cur_ststrt = currtime();	
	}
	if (mptr->msg_type == FETCH_REQ){
	    fetch_num++;
	    cur_ftstrt = currtime();
	}
#endif
	
	/* message sequence checking */
	if (mptr->msg_type != START_REQ){
	    
	    /*			curmidx = HashM(mptr);*/
	    curmidx = HashM(mptr);		
	    if ( curmidx == ERROR ) {
		status = ERROR;
		hdr_len = MSGLEN;
		Response(status, BADREQ ,mptr, hdr_len);
		continue;
	    }
	    
#ifdef DEBUG
	    if (!getnewmsg) {
		psvlog(DBGMED,
		       "curmidx = %d, M[curmidx].msg_num = %d  ",
		       curmidx, M[curmidx].msg_num);
		psvlog(DBGMED, "mptr->msg_num = %d\n",
		       mptr->msg_num);
	    }
#endif
	    seq = M[curmidx].msg_num;
	    /* check msg sequence*/
	    seq = (++seq > MAX_M_NO)?(seq - MAX_M_NO): seq;
	    /* seq = expected num*/
	    if ( mptr->msg_num == seq )
		/* normal case	     */
		M[curmidx].msg_num = seq;
	    /* update msg seq No */
	    else if ( mptr->msg_num > seq ) {
		if (mptr->msg_type==TERMIN_REQ ||
		    mptr->msg_type==END_REQ ) {
		    M[curmidx].msg_num = seq;
		}
		else {		/* send End-to-end NACK	*/
		    /* save the message for later	*/
		    tmpmsg = (struct tmessage *)
			malloc(sizeof(struct tmessage));
		    if (msglist[curmidx] == NULL) {
			msglist[curmidx] = tmpmsg;
			tmpmsg->nextmsg = NULL;
		    }
		    else {
			pnextmsg = msglist[curmidx];
			while ((pnextmsg->tprdlstptr->msgno
				< mptr->msg_num) &&
			       (pnextmsg->nextmsg!=NULL))
			    pnextmsg = pnextmsg->nextmsg;
			tmpmsg->nextmsg=pnextmsg->nextmsg;
			pnextmsg->nextmsg = tmpmsg;
		    }
		    if ((j=LookupReadList(mptr->msg_num,&from))
			!=(-1)) {
			tmpmsg->tprdlstptr =
			    &(TPReadList[j]);
		    }
		    /* done saving message for later */
		    
		    /* find out if the missing message	*/
		    /* is currently coming in: if it is	*/
		    /* don't bother sending an E-to-E	*/
		    /* NACK, else send a E-to-E NACK	*/
#ifdef DEBUG
		    psvlog(DBGMED, "check if missing packet %d is currently coming in\n", seq);
#endif
		    if ((j=LookupReadList(seq, &from))!=(-1)){
#ifdef DEBUG
			psvlog(DBGMED, "msgno = %d, curfrag = %d, j = %d\n",
			       TPReadList[j].msgno,
			       TPReadList[j].curfrag, j);
#endif
			if (TPReadList[j].curfrag==(-1)) {
#ifdef DEBUG
			    psvlog(DBGMED, "packet %d is NOT coming in, E-E NACK it\n", TPReadList[j].msgno);
#endif
			    /* has not come in yet */
			    /* so send NACK	   */
			    bcopy(mptr, TPReadList[j].msgp,
				  MSGLEN);
			    status = ERROR;
			    TPReadList[j].msgp->msg_num=seq;
			    Response(status,NACK_REQ,
				     TPReadList[j].msgp,
				     MSGLEN);
			}
#ifdef DEBUG
			else {
			    psvlog(DBGMED, "packet %d is coming in, do not E-E NACK\n", TPReadList[j].msgno);
			}
#endif
			/* else it is coming in, so	*/
			/* let it finish, no NACK	*/
		    }
		    continue;
		}
	    }
#ifdef WRONGWAYTODOIT
	    else {		/* mptr->msg_num < seq	*/
		psvlog(DBGMEDHIGH, "dropping duplicate packet, msg #%d\n",
		       mptr->msg_num);
		continue;
	    }
#endif
	    /* third case is:mptr->msg_num < seq,just go ahead */
	    /* and process it, the client must not have 	   */
	    /* received our first response			   */
	}    
	
	switch (mptr->msg_type)  {	/* what kind?	*/
	    
	  case CRSTOR_REQ	:		/* creat & store*/
	      
	      status = Creat_Store(mptr);
	      
#ifdef DEBUG		
	      if (status == ROK) DumpData(DBGLOW, mptr);
#endif			
	      
	      Response(status,CRSTOR_RSP,mptr,MSGLEN,NULLARY);
	      break;
	      
	    case STORE_REQ	:		/* store page	*/
		status = Store_Page(mptr, parray, dblk_num);
#ifdef DEBUG		
	      if (status == ROK) DumpData(DBGLOW, mptr);
#endif
	      Response(status,STORE_RSP,mptr,MSGLEN,NULLARY);
	      break;
	      
	    case FETCH_REQ	:		/* fetch page	*/
		status = Fetch_Page(mptr, parray);
#ifdef DEBUG		
	      if (status == ROK) DumpData(DBGLOW, mptr);
#endif
	      if (status == ROK) {
		  Response(status,FETCH_RSP,mptr,
			   mptr->page_size+MSGLEN, parray);
	      }
	      else {		/* error - so there is no data */
		  Response(status,FETCH_RSP,mptr,
			   MSGLEN, NULLARY);
	      }
	      break;
	      
	    case TEST_REQ:
	      status = ROK;
	      Response(status,TEST_RSP,mptr,MSGLEN,NULLARY);
	      break;
	      
	    case CREATE_REQ	:		/* proc create	*/
		status = Creat_Proc(mptr);
	      
#ifdef DEBUG
	      psvlog(DBGMEDLOW, "case:CREATE_REQ\n");
	      if (status == ROK) DumpData(DBGLOW, mptr);
#endif			
	      Response(status,CREATE_RSP,mptr,MSGLEN,NULLARY);
	      break;
	      
	    case TERMIN_REQ	:		/* proc terminat*/
		status = Terminate(mptr);
#ifdef TIMING
	      if (mptr->proc_id<=23){
		  psvlog(DBGHIGH, "store_num = %d accstore = %d\n",store_num,store_actime);
		  psvlog(DBGHIGH, "maxst = %d minst = %d\n",maxst,minst);
		  psvlog(DBGHIGH, "fetch_num = %d accfetch = %d\n",fetch_num,fetch_actime);
		  psvlog(DBGHIGH, "maxft = %d minft = %d\n",maxft,minft);
		  psvlog(DBGHIGH, "llstacc=%d llftacc=%d\n",llstacc, llftacc);
		  llstacc = llftacc = 0;
		  maxst=maxft=store_actime=fetch_actime=0;
		  store_num=fetch_num=0;
		  minst=minft=100;
	      }
#endif
	      
	      Response(status,TERMIN_RSP,mptr,MSGLEN,NULLARY);
	      break;
	      
	    case START_REQ  :
		status = Mach_Start(mptr);
	      psvlog(DBGMEDLOW, "START_ status = %d curmidx= %d\n",
		     status,curmidx);
	      Response(status,START_RSP,mptr,MSGLEN,NULLARY);
	      break;
	      
	    case END_REQ  :
		status = Mach_End(mptr);
	      Response(status,END_RSP,mptr,MSGLEN,NULLARY);
	      break;
#ifdef DEBUG			  
	    case DMPGTAB	:		/* dump G table	*/
		status = DumpG(DBGHIGH);
	      break;
	      
	    case DMPPTAB	:		/* dump P table	*/
		status = DumpP(DBGHIGH);
	      break;
	      
	    case DMPGNP	:		/* dump both	*/
		status = DumpGnP(DBGHIGH);
	      break;
	      
#endif			
	      
	      default		:		/* invalid	*/
		  status = BADREQ;
	      psvlog(DBGHIGH, "BAD message - contents follow\n");
	      DumpData(DBGHIGH, mptr);
	      Response(status,mptr->msg_type,mptr,MSGLEN);
	      
	  }
	
    }
    
}


/*----------------------------------------------------------------------------
 * SET_DATA_LIM() - Set the data segment of the server process to maximum
 *----------------------------------------------------------------------------
 */

Set_Data_Lim()
     
{
    struct rlimit rl;
    
    
#ifdef DEBUG			
    psvlog(DBGLOW, "Setting data limit to maximum.\n");
#endif				
    
    
    getrlimit(RLIMIT_DATA, &rl);	/* get soft limit on data seg	*/
    rl.rlim_cur = rl.rlim_max;	/* set it to maximum		*/
    setrlimit(RLIMIT_DATA, &rl);	/* set limit			*/
    
}


/*----------------------------------------------------------------------------
 * INIT_MTABLE() - Initialize the M (machine) table
 *----------------------------------------------------------------------------
 */

Init_Mtable()
     
{
    int i;
    
    
#ifdef DEBUG			
    psvlog(DBGMEDLOW, "Initializing M table.\n");
#endif				
    
    
    for (i=0; i<MAX_M_ENT; i++) {	/* initialize each slot 	*/
	M[i].valid = INVALID;	        /* mark it invalid	*/
	M[i].tstamp = 0;		/* initial time stamp   */
	M[i].msg_num  = INVALID;	/* initialize msg number */
	msglist[i] = (struct tmessage *)NULL; /* init saved msg	*/
	/* list for out of order msgs	*/
    }
    
}

/*----------------------------------------------------------------------------
 * INIT_PTABLE() - Initialize the P (process) hash table
 *----------------------------------------------------------------------------
 */

Init_Ptable()
     
{
    int i;
    
    
#ifdef DEBUG			
    psvlog(DBGMEDLOW, "Initializing P hash table.\n");
#endif				
    
    
    for (i=0; i<MAX_P_ENT; i++) {	/* initialize each slot 	*/
	P[i].valid_flag = INVALID;	/* mark it invalid	*/
	P[i].mach_tstp = 0;		/* initialize mach tstamp */
	P[i].timestamp  = 0;	        /* initialize timestamp */
    }
    
}


/*----------------------------------------------------------------------------
 * INIT_GTABLE() - Initialize G (global) hash table
 *----------------------------------------------------------------------------
 */

Init_Gtable()
     
{
    int i;
    
    
#ifdef DEBUG			
    psvlog(DBGMEDLOW, "Initializing G hash table.\n");
#endif				
    
    
    for (i=0; i<MAX_G_ENT; i++){	/* initialize each slot		*/
	G[i].valid_flag = INVALID;	/* mark it invalid	*/
	G[i].dblk_cnt   = 0;
    }
    
}


/*----------------------------------------------------------------------------
 * INIT_FREE() - Create the free list of pages
 *----------------------------------------------------------------------------
 */

struct d_block *Init_Free()
     
{
    int i;
    
    
#ifdef DEBUG			
    psvlog(DBGMEDLOW, "Linking together the free list.\n");
#endif				
    
    
    for (i=0; i<num_d_blks-1; i++) {	/* create maximum number allowed*/
	D[i].next = &D[i+1];	/* link them together		*/
    }
    
    D[num_d_blks-1].next = NULL;	/* set tail of list		*/
    return(D);			/* return head			*/
    
}


/*-----------------------------------------------------------------------------
 * CREAT_PROC() - Set up for a new process
 *-----------------------------------------------------------------------------
 */

Creat_Proc(p)
     struct message *p;
     
{
    int pidx;
    
    pidx = HashP(p, INSERT);	/* hash into P table inserting	*/
    
#ifdef DEBUG
    psvlog(DBGLOW, "Creat_Proc:pid=%d, pidx=%d\n",p->proc_id,pidx);
#endif
    
    if (pidx == ERROR) return(PFULL);	/* no room		*/
    P[pidx].valid_flag = VALID;	/* validate the new process	*/
    P[pidx].proc_id = p->proc_id;	/* store the PID		*/
    P[pidx].mach_id = curmidx;	/* store machine id		*/
    P[pidx].mach_tstp = M[curmidx].tstamp;	/*store machine t-stamp*/
    
    
#ifdef DEBUG			
    psvlog(DBGMEDLOW, "Create proc %d in P slot %d valid=%d\n",
	   p->proc_id,pidx,P[pidx].valid_flag);
#endif		
    
    
    return(ROK);
    
}

/*-----------------------------------------------------------------------------
 * MACH_START() - Set up for a new machine
 *-----------------------------------------------------------------------------
 */

Mach_Start(p)
     struct message *p;
     
{
    int midx;
    
    midx = HashM(p);
    
    if (midx==ERROR)
	return(ERROR);
    if (M[midx].valid != VALID) 	     /* client boots up first time */
	M[midx].valid = VALID;	     /* validate the new machine */
    M[midx].tstamp++;		     /* increment time stamp	 */
    M[midx].msg_num = p->msg_num;	     /* store the message number */
    
    return(ROK);
    
}
/*-----------------------------------------------------------------------------
 * MACH_END() - Clear up for a machine
 *-----------------------------------------------------------------------------
 */

Mach_End(p)
     struct message *p;
     
{
    int midx;
    
    midx = curmidx;
    M[midx].valid = INVALID;             /* validate the new machine */
    M[midx].msg_num = INVALID;	     /* store the message number */
    
    return(ROK);
    
}
/*-----------------------------------------------------------------------------
 * CREAT_STORE() - create process and store first page
 *-----------------------------------------------------------------------------
 */

Creat_Store(p)
     struct message *p;
     
{
    
    int status;
    
    if ((status = Creat_Proc(p)) != ROK) 
	return(status);			/* create the process	*/
    
    if ((status = Store_Page(p)) != ROK) {	/* store it's page	*/
	Terminate(p);			/* undo the Creat_Proc */
	return(status); 
    }
    
    return(ROK);
    
}


/*-----------------------------------------------------------------------------
 * STORE_PAGE() - Store a page
 *-----------------------------------------------------------------------------
 */

Store_Page(p, ptrs, num)
     struct message *p;
     struct d_block   *ptrs[MAX_PER_PAGE];
     int    num;
{
    
    int pidx, gidx, j;
    char s[10];
    
    pidx = HashP(p, FIND);		/* find the process & timestamp	*/
    if (pidx == ERROR) {		/* not there		*/
	psvlog(DBGHIGH, "HashP not find process %d\n",
	       p->proc_id);
	return(NOEXIST);
    }
    gidx = HashG(p, P[pidx].timestamp, INSERT);	/* find a slot	*/
    if (gidx == ERROR) {				/* no room	*/
	psvlog(DBGHIGH, "HashG no room\n");
	return(GFULL);
    }
    G[gidx].mach_id = curmidx;	/* save machine id	*/
    G[gidx].proc_id = p->proc_id;	/* save PID		*/
    G[gidx].page_num = p->page_num; /* save page number	*/
    G[gidx].valid_flag = VALID;	/* validate the new one */
    G[gidx].timestamp = P[pidx].timestamp;	/* stamp it	*/
    G[gidx].p_ent_ptr = &P[pidx];	/* save back pointer	*/
    
    if (G[gidx].dblk_cnt)		/* free old d_blocks if any */
	Put_Free(G[gidx].d_blk_ptr,G[gidx].dblk_cnt);
    
    G[gidx].dblk_cnt = num;		/* save number of 1k's	*/
    for ( j=0; j< num; j++)
	G[gidx].d_blk_ptr[j] = ptrs[j];
    
    return(ROK);
}

/*-----------------------------------------------------------------------------
 * FETCH_PAGE() - Get a page for the client
 *-----------------------------------------------------------------------------
 */

Fetch_Page(p, ptrs)
     struct message *p;
     struct d_block *ptrs[MAX_PER_PAGE];
{
    
    int pidx, gidx, nalloc, i;
    char *a;
    
    pidx = HashP(p, FIND);		/* find the process & timestamp	*/
    if (pidx == ERROR) {
	psvlog(DBGHIGH, "Fetch hashP not found, pid = %d\n",
	       p->proc_id);
	return(NOEXIST);	/* not there		*/
    }
    gidx = HashG(p, P[pidx].timestamp, FIND);	/* find the slot*/
    if (gidx == ERROR){
	psvlog(DBGHIGH, "Fetch hashG not found\n");
	return(NOEXIST);		/* not there	*/
    }
    
    nalloc = G[gidx].dblk_cnt;		
    
    for (i=0; i<nalloc; i++)
	ptrs[i] = G[gidx].d_blk_ptr[i];
    
    return(ROK);
    
}


/*-----------------------------------------------------------------------------
 * TERMINATE() - Indicate that a process no longer exists for a client
 *-----------------------------------------------------------------------------
 */

Terminate(p)
     struct message *p;
     
{
    
    int pidx;
    
    pidx = HashP(p, FIND);		        /* find the entry 	*/
    
    if (pidx == ERROR) return(NOEXIST);	/* not there		*/
    
    P[pidx].valid_flag = INVALID;		/* mark as invalid	*/
    P[pidx].timestamp++;			/* increment timestamp	*/
    return(ROK);
    
}

/*-----------------------------------------------------------------------------
 * GET_FREE() - Get a number of 1k pages from the free list
 *-----------------------------------------------------------------------------
 */

Get_Free(d, cnt)
     struct d_block *d[MAX_D_BLK];
     int cnt;
     
{
    
    int i;
    struct d_block *tp;
    
    for (i=0, tp=free_list; (tp != NULL)&&(i<cnt); i++, tp=tp->next)
	d[i] = tp;		/* put in a list of pointers	*/
    
    if (i != cnt) return(ERROR);	/* ran out before completing	*/
    
    free_list = tp;			/* update free list pointer	*/
    return(ROK);
    
}

/*-----------------------------------------------------------------------------
 * HASHM() - Hash function into the M (machine) hash table
 *-----------------------------------------------------------------------------
 */

HashM(p)
     struct message *p;
{
    short  index = 0;
    
    index = ( p->mach_id[3] & p->mach_id[2] | p->mach_id[1] |
	     p->mach_id[0]) & 017;
    while( M[index].valid == VALID ) {
	if ( M[index].mach_id[3]==p->mach_id[3] &&
	    M[index].mach_id[2]==p->mach_id[2] &&
	    M[index].mach_id[1]==p->mach_id[1] &&
	    M[index].mach_id[0]==p->mach_id[0] )
	    return(index);
	else {
	    index = ((index += 13) > MAX_M_ENT-1)? index-MAX_M_ENT : index;
	    continue;
	}
    }
    if ( p->msg_type == START_REQ ) {
	M[index].mach_id[3] = p->mach_id[3];
	M[index].mach_id[2] = p->mach_id[2];
	M[index].mach_id[1] = p->mach_id[1];
	M[index].mach_id[0] = p->mach_id[0];		
	return(index);
    } else
	return(ERROR);
}

/*-----------------------------------------------------------------------------
 * HASHG() - Hash function into the G (global) page hash table.  If INSERT
 *           then if it exists, return the index else if it doesn't exist
 *	     return a blank slot else return error.  If FIND then if it exists
 *	     return the index else return error.
 *-----------------------------------------------------------------------------
 */

HashG(p, t, cmd)
     struct message *p;
     int t, cmd;
     
{
    
    int idx, inc, nchk;
    struct p_entry *bptr;
    
    HashFnG(p, t, &idx, &inc);	/* hash the keys		*/
    
    
#ifdef DEBUG			
    psvlog(DBGLOW, "Hashing into G table. IDX=%d INC=%d\n",idx,inc);
#endif				
    
    
    for (nchk=1; nchk<=MAX_G_ENT; nchk++) {		/* until exhaust*/
	
	
#ifdef DEBUG			
	psvlog(DBGLOW, "Check %d time(s).",nchk);
#endif				
	
	if ((G[idx].valid_flag == VALID)	&&
	    (G[idx].page_num == p->page_num)	&&
	    (G[idx].proc_id == p->proc_id)	&&	/* found*/
	    (G[idx].mach_id == curmidx)	&&
	    (G[idx].timestamp == t)) return(idx);
	
	if (G[idx].valid_flag == VALID) {	/* found valid	*/
	    
	    bptr = G[idx].p_ent_ptr;	/* P backpointer*/
	    if (bptr->valid_flag == INVALID){/* old G entry */
		
#ifdef DEBUG			
		psvlog(DBGLOW,
		       "Old G entry, P entry invalid.\n");
#endif				
		
		G[idx].valid_flag = INVALID;
		Put_Free(G[idx].d_blk_ptr, G[idx].dblk_cnt);
		G[idx].dblk_cnt = 0;
		if (cmd == INSERT) return(idx);
		
	    } else {		/* old G entry, used P	*/
		
		if (G[idx].proc_id != bptr->proc_id	||
		    G[idx].mach_id != bptr->mach_id	||
		    G[idx].timestamp != bptr->timestamp) {
		    
		    
#ifdef DEBUG			    
		    psvlog(DBGLOW,
			   "Old G entry, P entry re-used\n");
#endif				    
		    
		    
		    G[idx].valid_flag = INVALID;
		    Put_Free(G[idx].d_blk_ptr,G[idx].dblk_cnt);
		    G[idx].dblk_cnt = 0;
		    if (cmd == INSERT) return(idx);
		} 
	    }
	    
	} else {		/* invalid, no collision	*/
	    
#ifdef DEBUG
	    psvlog(DBGMEDLOW, "\n");
#endif
	    
	    if (cmd == INSERT) return(idx);	/* INSERT, ok	*/
	}
#ifdef DEBUG
	psvlog(DBGMED, "HashG: skipped entry %d\n", idx);
#endif
	
	if ((idx -= inc) < 0) idx += MAX_G_ENT; /* search again */
    }
    
#ifdef DEBUG
    if (cmd == INSERT)
	psvlog(DBGMEDHIGH, "HashG: Did not find free entry\n");
    else
	psvlog(DBGMEDHIGH, "HashG: Did not find entry\n");
#endif
    
    return(ERROR);
    
}


/*-----------------------------------------------------------------------------
 * HASHP() - Hash function into the P (process) hash table
 *-----------------------------------------------------------------------------
 */

HashP(p, cmd)
     struct message *p;
     int cmd;
     
{
    
    int idx, inc, nchk;
    
    HashFnP(p, &idx, &inc);		/* hash the keys		*/
    
    
#ifdef DEBUG			
    psvlog(DBGLOW, "Hashing into P table, IDX=%d INC=%d.\n",idx,inc);
#endif				
    
    for (nchk=1; nchk<=MAX_P_ENT; nchk++) {
	
#ifdef DEBUG			
	psvlog(DBGLOW, "Check %d time(s)\n",nchk);
#endif			
	
	if (P[idx].valid_flag == VALID		&&
	    P[idx].proc_id == p->proc_id	&&
	    P[idx].mach_id == curmidx	&&
	    P[idx].mach_tstp == M[curmidx].tstamp )
	    return(idx); 			/* match	*/
	
	if ((P[idx].valid_flag == INVALID ||	/* prev. terminated */
	     P[idx].proc_id == p->proc_id	&&    
	     P[idx].mach_id == curmidx	&&     /* or client  */
	     P[idx].mach_tstp < M[curmidx].tstamp) && 
	    cmd == INSERT)
	    return(idx);			/* INSERT, ok	*/
	else if ((idx -= inc) < 0)	
	    idx += MAX_P_ENT; 			/* again */
    }
    
    return(ERROR);
    
}


/*-----------------------------------------------------------------------------
 * PUT_FREE() - put blocks back on the free list
 *-----------------------------------------------------------------------------
 */

Put_Free(d, nblk)
     struct d_block *d[MAX_D_BLK];
     int nblk;
     
{
    
    int i;
    struct d_block *tp;
    
    for (i=(nblk-1), tp=free_list; i>=0; i--){/* return reverse order*/
	(d[i])->next = tp;		/* link last to head	*/
	tp = d[i];			/* last is new head	*/
    }
    
    free_list = tp;				/* new head of freelist */
    return(ROK);
    
}


/*-----------------------------------------------------------------------------
 * HASHFNG() - Hash function for the G table
 *-----------------------------------------------------------------------------
 */

HashFnG(p, time, idx, inc)
     struct message *p;
     int time, *idx, *inc;
     
{
    
    unsigned int val;
    
    val = FoldG(curmidx, p->proc_id, p->page_num, time);
    *idx = val % MAX_G_ENT;			/* fold four integers..	*/
    *inc = (val % (MAX_G_ENT-2)) + 1;	/* ..into one and mod	*/
    
}


/*-----------------------------------------------------------------------------
 * HASHFNP() - Hash function for the P table
 *-----------------------------------------------------------------------------
 */

HashFnP(p, idx, inc)
     struct message *p;
     int *idx, *inc;
     
{
    
    unsigned int val;
    
    val = FoldP(curmidx, p->proc_id);
    *idx = val % MAX_P_ENT;			/* fold two integers..	*/
    *inc = (val % (MAX_P_ENT-2)) + 1;	/* ..into one and mod	*/
    
}


/*-----------------------------------------------------------------------------
 * FOLDG() - Fold the G table keys into an integer.  To keep this function
 *	relatively machine independent, the htonl() function was used to
 *	standardize the bit ordering of the hashed integers and get the best
 *	distribution for any machine.
 *-----------------------------------------------------------------------------
 */

FoldG(m, pid, pn, ts)
     int m, pid, pn, ts;
     
{
    
    int t1, t2, final;
    
    t1 = pid << 16;		/* PID upper 2 bytes		*/
    t2 = pn  << 8;		/* page number middle 2 bytes	*/
    /* small timestamp, lower 2 byte*/
    final = ((m ^ t1) ^ t2) ^ ts;	/* timestamp lower 2 bytes	*/
    return(final);
    
}


/*-----------------------------------------------------------------------------
 * FOLDP() - Fold the P table keys into an integer.  To keep this routine
 *	relatively machine independent, the htonl() function was used to
 *	standardize the bit ordering of the hashed integers and get the best
 *	distribution for any machine.
 *-----------------------------------------------------------------------------
 */

FoldP(m, pid)
     int m, pid;
     
{
    
    int t1, final;
    
    t1 = pid << 16;		/* PID upper 2 bytes		*/
    final = m ^ t1;		/* EXOR with machine id		*/
    return(final);
    
}


/*-----------------------------------------------------------------------------
 * RESPONSE() - Response to a command from a client
 *-----------------------------------------------------------------------------
 */

Response(stat, msgresp, p, msgsize, ptrs)
     int stat, msgresp, msgsize;
     struct message *p;
     char *ptrs[MAX_PER_PAGE];
{
    struct message msg, *mptr;
    int  reqtype, time;
    long sec, usec;
    char hostserved[64], str1[16], str2[16];
    
    mptr = &msg;
    bcopy( p, mptr, MSGLEN);
    
    p->ok_error = stat;		/* status of the request	*/
    reqtype = p->msg_type;		/* save if need to report error */
    p->msg_type = msgresp;		/* response type		*/
    
    
#ifdef DEBUG			
    psvlog(DBGMEDLOW, "Message to TPSWrite :\n");
    print_msg(DBGLOW, p);
#endif				
    
    
#ifdef TIMING
    if (p->msg_type == STORE_RSP) {
	sttime = currtime() - cur_ststrt;
	if (sttime > maxst) maxst = sttime;
	if (sttime < minst) minst = sttime;
	store_actime += sttime;
    }
    if (p->msg_type == FETCH_RSP) {
	fttime = currtime() - cur_ftstrt;
	if (fttime > maxft) maxft = fttime;
	if (fttime < minft) minft = fttime;
	fetch_actime += fttime; 
    }
#endif
    
    Host_to_Net(p);			/* change to network byte order	*/
    TPSwrite(mptr , p, msgsize, &from, ptrs);	/* back to client */
}


/*----------------------------------------------------------------------------
 * FINDHOST() - Decode host address into host name string
 *----------------------------------------------------------------------------
 */
char *FindHost(pfrom, pstr)
     struct sockaddr_in *pfrom;
     char *pstr;
{
    struct hostent *phe;
    int sz;
    char haddr[80];
    
    strcpy(haddr, inet_ntoa(pfrom->sin_addr));	/* address string*/
    sz = sizeof(pfrom->sin_addr);
    phe = gethostbyaddr(&(pfrom->sin_addr),sz,AF_INET);
    strcpy(pstr, phe->h_name);			/* host string	 */
    return(pstr);
}


/*----------------------------------------------------------------------------
 * LISTENUDP() - Called at initialization to set up UDP port for the
 *	protocol to listen.  The file descriptor is returned (to protocol).
 *	This routine will not work under 4.2 Berkley UNIX due to the fact
 *	that one cannot increase the size of the UDP buffers, and this must
 *	be increased if more than one client expects to use the server.
 *----------------------------------------------------------------------------
 */

ListenUdp()
     
{
    
    int fd, lsa, maxbuff, lmb;
    struct sockaddr_in sa;
    struct servent *pse;
    
    lsa = sizeof(sa);
    
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	perror("socket");		/* create socket 	*/
    
    maxbuff = 40*1024;			/* bigger UDP buffer 	*/
    lmb = sizeof(maxbuff);			/* req'd for getsockopt */
    
#if defined(VAX) || defined(SUN) || defined(DEC)
    if (setsockopt(fd,SOL_SOCKET,SO_RCVBUF,(char *)&maxbuff,lmb) != 0)
	perror("setsockopt");		/* make bigger buffer	*/
#endif
    
#ifdef	DEBUG
    psvlog(DBGLOW, "Setting UDP port buffer size to %d\n",maxbuff);
#endif
    
    if (portopt < 0) {
	if ((pse = getservbyname(SERVNAME,"udp")) == NULL)
	    perror("getservbyname");	/* find Xinu pageserver*/
	portopt = pse->s_port;
    }
    else {
	portopt = htons(portopt);
    }
    
    sa.sin_family = AF_INET;		/* set to socket port	*/
    sa.sin_port = portopt;
    psvlog(DBGHIGH, "Using port = %d\n", ntohs(portopt));
    sa.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(fd, &sa, lsa) == -1) {		/* bind file descriptor */
	psvlog(DBGHIGH, "%s already running\n",SERVNAME);
	exit(0);
    }
    
    return(fd);				/* file desc of server	*/
    
}


#ifdef DEBUG			

/*-----------------------------------------------------------------------
 * DumpG() - Dump G table. For debugging only.
 *-----------------------------------------------------------------------
 */

DumpG(dbglevel)
     int dbglevel;
{
    
    int i;
    
    psvlog(dbglevel, "==================== G Table ===========================\n");
    
    for (i=0; i<MAX_G_ENT; i++) {
	
	if (G[i].valid_flag == VALID) 
	    psvlog(dbglevel, "%4d: Mach=%X PID=%d Page=%d Val=VALID Time=%d Cnt=%d\n",
		   i,G[i].mach_id,G[i].proc_id,G[i].page_num,
		   G[i].timestamp,G[i].dblk_cnt);
    }
    
    return(ROK);
    
}


/*----------------------------------------------------------------------
 * DumpP() - Dump P table. For debugging only.
 *----------------------------------------------------------------------
 */

DumpP(dbglevel)
     int dbglevel;
{
    
    int i;
    
    psvlog(dbglevel, "================== P Table ======================\n");
    
    for (i=0; i<MAX_P_ENT; i++) {
	if (P[i].valid_flag == VALID) 
	    psvlog(dbglevel, "%4d: Mach=%X  PID=%d  Val=VALID  Time=%d\n",
		   i,P[i].mach_id,P[i].proc_id,P[i].timestamp);
    }
    
    return(ROK);
    
}


/*----------------------------------------------------------------------
 * DUMPTGNP() - Dump both P and G tables
 *----------------------------------------------------------------------
 */

DumpGnP(dbglevel)
     int dbglevel;
{	
    
    DumpP(dbglevel);
    DumpG(dbglevel);
    return(ROK);
    
}

#endif



/*------------------------------------------------------------------------
 * Dump the data
 *------------------------------------------------------------------------
 */

DumpData(dbglevel, p)
     int dbglevel;
     struct message *p;
{
    
    int i;
    
    if (dbglevel >= curdbglevel) {
	fprintf(dbgdevice , "========== PAGE DATA =============\n");
	for (i=0; i<32; i++) {
	    fprintf(dbgdevice, "%02x",
		    (unsigned char)(p->data[i]));
	}
	
	fprintf(dbgdevice, "\n");
	fflush(dbgdevice);
    }
    
    
    
}


/*--------------------------------------------------------------------------
 * PRINT_MSG() - Debugging utility to dump the page server header info
 *--------------------------------------------------------------------------
 */

print_msg(dbglevel, mp)
     int dbglevel;
     struct message *mp;
{
    
    char type[16];
    
    psvlog(dbglevel, "msg_num:    %d\n",mp->msg_num);
    psvlog(dbglevel, "msg_type:   %s\n",Which(mp->msg_type,type));
    /*	psvlog(dbglevel, "mach_id:    %d\n",mp->mach_id);*/
    psvlog(dbglevel, "proc_id:    %d\n",mp->proc_id);
    psvlog(dbglevel, "page_num:   %d\n",mp->page_num);
    psvlog(dbglevel, "page_size:  %d\n",mp->page_size);
    DumpData(dbglevel, mp);
    
}


/*-------------------------------------------------------------------------
 * WHICH() - Translates a Request/Response constant into a string
 *-------------------------------------------------------------------------
 */

char *Which(n,cp)
     int n;
     char *cp;
     
{
    
    switch (n) {
	
      case	CREATE_REQ : strcpy(cp,"CREAT_REQ"); break;
      case	CRSTOR_REQ : strcpy(cp,"CRSTOR_REQ");break;
      case	STORE_REQ  : strcpy(cp,"STORE_REQ"); break;
      case	FETCH_REQ  : strcpy(cp,"FETCH_REQ"); break;
      case	TERMIN_REQ : strcpy(cp,"TERMIN_REQ");break;
      case	CREATE_RSP : strcpy(cp,"CREATE_RSP");break;
      case	CRSTOR_RSP : strcpy(cp,"CRSTOR_RSP");break;
      case	STORE_RSP  : strcpy(cp,"STORE_RSP"); break;
      case	FETCH_RSP  : strcpy(cp,"FETCH_RSP"); break;
      case	TERMIN_RSP : strcpy(cp,"TERMIN_RSP");break;
      case	BADREQ	   : strcpy(cp,"BADREQ");    break;
	default		   : sprintf(cp,"%d",n);
	
    }
    
    return(cp);
    
}

/*---------------------------------------------------------------------------
 * ERRNO() - Translates an error return code constant into a string
 *---------------------------------------------------------------------------
 */

char *ErrNo(no,str)
     int no;
     char *str;
{
    switch (no) {
      case  ERROR 	: strcpy(str,"ERROR"); break;
      case  ROK	: strcpy(str,"ROK"); break;
      case  PFULL	: strcpy(str,"P TABLE FULL"); break;
      case  NOEXIST	: strcpy(str,"NON-EXISTANT"); break;
      case  GFULL	: strcpy(str,"G TABLE FULL"); break;
      case  NOPAGES	: strcpy(str,"OUT OF PAGES"); break;
    }
    return(str);
}

/*---------------------------------------------------------------------------
 * fastbcopy - fast block copy
 *	       used from int address to int address with multiple int's
 *---------------------------------------------------------------------------
 */
fastbcopy(fromaddr, toaddr, numofint)
     register unsigned int *fromaddr, *toaddr;
     register int numofint;
{
    while( numofint-- > 0 )
	*toaddr++ = *fromaddr++;
}


/*---------------------------------------------------------------------------
 * psvlog - log all page server messages
 *---------------------------------------------------------------------------
 */
psvlog(dbglevel, fmt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9)
     int dbglevel;
     char *fmt;
     int a0, a1, a2, a3, a4, a5, a6, a7, a8, a9;
{
    char *curtime;
    
    if (dbglevel >= curdbglevel) { 
	gettimeofday(&ps_begin,&tzdummy);
	curtime = ctime(&(ps_begin.tv_sec));
	curtime[strlen(curtime)-1] = '\0';
	fprintf(dbgdevice, "psv: %s: ", curtime); 
	fprintf(dbgdevice, fmt, a0,a1,a2,a3,a4,a5,a6,a7,a8,a9);
	fflush(dbgdevice);
    } 
}


/*---------------------------------------------------------------------------
 * initdbgdevice - Initialize debug logging device
 *---------------------------------------------------------------------------
 */
initdbgdevice()
{
    if (strlen(logfilename) > 0) {
	if ((dbgdevice = fopen(logfilename, "w")) == (FILE *)NULL) {
	    fprintf(stderr, "Error opening log file %s\n", logfilename);
	    exit(1);
	}
	else {
	    fprintf(stderr, "Opened file %s for logging messages\n",
		    logfilename);
	}
    }
    else {
	dbgdevice = stderr;
	fprintf(stderr, "Using stderr as log file\n");
    }
    
}


/*---------------------------------------------------------------------------
 * help - print usage information
 *---------------------------------------------------------------------------
 */
help(cmd)
     char *cmd;
{
    fprintf(stderr, "usage: %s [options]\n", cmd);
    fprintf(stderr, "possible options:\n");
    fprintf(stderr, "\t-h\thelp\n");
    fprintf(stderr, "\t-help\thelp\n");
    fprintf(stderr, "\t-dh\tset debug level high (default)\n");
    fprintf(stderr, "\t-dmh\tset debug level medium high\n");
    fprintf(stderr, "\t-dm\tset debug level medium\n");
    fprintf(stderr, "\t-dml\tset debug level medium low\n");
    fprintf(stderr, "\t-dl\tset debug level low\n");
    fprintf(stderr, "\t-lf logfilename  specify logging filename\n");
    fprintf(stderr, "\t-nd num_dblks    specify number of data blocks\n");
    fprintf(stderr, "\t-p UDP_port      specify UDP port to listen on\n");
    exit(0);
}


/*-----------------------------------------------------------------------------
 * garbagecollect(needed) - reclaim unused memory data blocks
 *			return 1 if got them all,
 *			return 0 if more to do (i.e. we were interrupted)
 *-----------------------------------------------------------------------------
 */
garbagecollect(needed)
     int needed;				/* min data blocks needed */
{
    
    int	idx;
    int	iii;
    int	freedcount;
    struct p_entry *bptr;
    struct g_entry *gptr;
    
    
#ifdef DEBUG			
    psvlog(DBGLOW, "Garbag collecting\n");
#endif				
    
    
    stopgarbcollect = FALSE;
    iii = 0;
    freedcount = 0;
    
    while ((!stopgarbcollect) &&
	   (iii < MAX_G_ENT) && (freedcount < needed)) {
	
#ifdef LOTS_OF_OUTPUT
	if ((garbcollectindex<<27)==0)
	    psvlog(DBGMED, "garbage collect looking at G[%d]\n",
		   garbcollectindex);
#endif
	
	if (G[garbcollectindex].valid_flag == VALID) {
	    
	    gptr = &(G[garbcollectindex]);
	    
	    bptr = gptr->p_ent_ptr;	/* P backpointer*/
	    if (bptr->valid_flag == INVALID){/* old G entry */
		gptr->valid_flag = INVALID;
		Put_Free(gptr->d_blk_ptr, gptr->dblk_cnt);
		gptr->dblk_cnt = 0;
		freedcount++;
	    }
	    else {		/* old G entry, used P	*/
		if (gptr->proc_id != bptr->proc_id	||
		    gptr->mach_id != bptr->mach_id	||
		    gptr->timestamp != bptr->timestamp) {
		    gptr->valid_flag = INVALID;
		    Put_Free(gptr->d_blk_ptr,
			     gptr->dblk_cnt);
		    gptr->dblk_cnt = 0;
		    freedcount++;
		} 
	    }
	    
	}
	garbcollectindex++;
	if (garbcollectindex >= MAX_G_ENT)
	    garbcollectindex = 0;
	iii++;
    }
    
    if (stopgarbcollect)
	return(0);
    else
	return(1);
}

