/*----------------------------------------------------------------------
 * xpppgio.c - do paging using XINU Paging Protocol
 *----------------------------------------------------------------------
 */
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <mem.h>
#include <vmem.h>
#include <frame.h>
#include <network.h>
#include <tp.h>
#include <pgio.h>
#include <xpppgio.h>
#include <clock.h>

extern int bitmap[];
extern int  bufmesgno;

void	xpphstonet();
void	xppnettohs();
int	xpppgingsem;	/* semaphore to protect pend list	*/
int	pgsdev;		/* page server device number		*/
int	sendproc;	/* sender process id			*/
int	recvproc;	/* reciever process id			*/
char	*rmsg;
struct rqnode *xppfindrq();

/* local routines */
LOCAL void	abort();
LOCAL void	dropnd();
LOCAL void	fillmsg();
LOCAL void	insertr();
LOCAL int	mkrqt();
LOCAL void	sendrq();
LOCAL void	tkoffrq();
LOCAL struct rqnode *enqrqt();

/* local globals */
static int	badmesg = (-1);
static long	tmptime;	/* temp variable to store time	*/
static struct	rqnode rq[6];	/* request list head/tail node array	*/
static int	msgpool;	/* message buf pool id			*/
static int	nodepool;	/* request node buf pool id		*/
static int	msgcount;	/* message counter for msg number	*/
static int	sendproc;	/* sender process id			*/
static int	recvproc;	/* reciever process id			*/

#define RQSYSERR ((struct rqnode *) SYSERR)

/*--------------------------------------------------------------------------
 *  xpphstonet - convert message from host to net
 *--------------------------------------------------------------------------
 */
void xpphstonet(mptr)
     struct message *mptr;
{
    mptr->msg_num = hl2net(mptr->msg_num);
    mptr->msg_type = hl2net(mptr->msg_type);
    mptr->pa_id = hl2net(mptr->pa_id);
    mptr->page_num = hl2net(mptr->page_num);
    mptr->page_size = hl2net(mptr->page_size);
    mptr->msg_code = hl2net(mptr->msg_code);
}


/*--------------------------------------------------------------------------
 *  xppnettohs - convert message from net to host
 *--------------------------------------------------------------------------
 */
void xppnettohs(mptr)
     struct message *mptr;
{
    mptr->msg_num = net2hl(mptr->msg_num);
    mptr->msg_type = net2hl(mptr->msg_type);
    mptr->pa_id = net2hl(mptr->pa_id);
    mptr->page_num = net2hl(mptr->page_num);
    mptr->page_size = net2hl(mptr->page_size);
    mptr->msg_code = net2hl(mptr->msg_code);
}


/*--------------------------------------------------------------------------
 *  pgsdr - client paging sender
 *--------------------------------------------------------------------------
 */
pgsdr()
{
    int    i, fn, timdif, ret;
    int	   pgouttimes;			/*pgouttimes is consecutive pgout limit*/
    long   timestp;
    struct message msg;
    struct message *msgp,*mp;
    struct rqnode *rqptr;
    struct fte *flmptr;

    mp = &msg;
    /* pick up START_REQ first */

    wait(xpppgingsem);
    if (((rqptr=enqrqt(PROCRQHD, PROCRQTL, PENDINGTL))== RQSYSERR ) ||
	(rqptr->msgptr->msg_type != START_REQ))
	panic("There is no machine START request to the PS !\n");

    prlsize++;
    fillmsg(msgp = rqptr->msgptr);	/* fill in host parameter*/
    blkcopy(mp, (char*)msgp, MSGLEN);
    sendrq(rqptr,mp,MSGLEN);
    signal(xpppgingsem);

    while (TRUE) {
	/* enqueue request(s) into prlist from three lists */
	wait(xpppgingsem);

	while (psready && prlsize < MAXPRLLEN) { /* flow control */
	    if ((rqptr=enqrqt(PROCRQHD,PROCRQTL,PENDINGTL))== RQSYSERR )  
		break;			/* take from proc c/t rqt*/
	    else {
		prlsize++;
		fillmsg(msgp = rqptr->msgptr); /* fill in host parameter*/
		blkcopy(mp, (char*)msgp, MSGLEN);
		sendrq(rqptr,mp,MSGLEN); /* send mach, asid, proc */
	    } /* requests */
	};

	while (psready && prlsize < MAXPRLLEN) {
	    if((rqptr=enqrqt(PGINRQHD,PGINRQTL,PENDINGTL))== RQSYSERR )  
		break;			/* take rqt from pagein  */
	    else {
		prlsize++;
		fillmsg(msgp = rqptr->msgptr);
		blkcopy(mp, (char *)msgp, MSGLEN);
		sendrq(rqptr,mp,MSGLEN);
	    }
	};

	/* start with the first frame after the head of the	*/
	/* modified list (might just be the tail)		*/
	fn = ft[FTMHD].next;

	pgouttimes = 0;			/* reset pgout times     */
	while (psready && prlsize < MAXPRLLEN && pgouttimes < 5) {
	    flmptr = &ft[fn];

	    if (!ftinfo.mlen || (fn == FTMTL))
		break;			/*no pgout request to be made*/

	    /* before making page-out request, free modified frames
	       whose owner died */

	    if (flmptr->bits.st.list != FTMLIST) {
		fn = ft[FTMHD].next;	/* start over	*/
		continue;
	    }

	    if (ftobsolete(fn)) {
		continue;
	    }

	    msgp = &msg;
	    if (mkrqt(fn, STORE_REQ, &msgp)==SYSERR)
		break;			/* can't make request    */
	    else {
		ft[fn].bits.st.pgout =TRUE; /*paging out in progress*/
		rqptr = (struct rqnode *)getbuf(nodepool);               
		rqptr->msgptr = msgp;	/* connect node & msg    */
		rqptr->sent = 0;
		insertr(rqptr,PENDINGTL); /* add rqnode to prl tail*/
		prlsize++;
		flmptr = &ft[fn];	/* advance M-list pointer*/
		msgp->fram_num = fn;	/* for TP's use */
		fn = flmptr->next;
		fillmsg(msgp);
		blkcopy(mp, (char *)msgp, MSGLEN );
		sendrq(rqptr, mp, MSGLEN+PGSIZ );
		pg.pagingouts++;
		pgouttimes++;
	    }
	}

	/* scan pending request list, handle timeout */

	prloldest = clktime;
	rqptr = rq[PENDINGHD].nextnd;
	/* find the earliest time */
	if (prlsize) {
	    for (i=0; i<prlsize; i++, rqptr=rqptr->nextnd) {
		if (prloldest > rqptr->timestp) /* minimum timestp is oldest*/
		    prloldest = rqptr->timestp;
	    }				
	    /* find all timed out rqt's */
	    if (clktime - prloldest >= TIMOUT) {
		rqptr = rq[PENDINGHD].nextnd; /* from prl beginning  */

		for ( i=0; i<prlsize; i++, rqptr=rqptr->nextnd ) {
		    timestp = rqptr->timestp;
		    msgp = rqptr->msgptr;
		    if ( clktime-timestp >= TIMOUT ) { /* the rqt timed out */
			if (rqptr->sent >= LIMIT) 
			    /* beyond re-sending times */
			    if((msgp->msg_type==TERMIN_REQ ||
				msgp->msg_type==START_REQ ) &&
			       rqptr->sent < MAXSEND ) {
				blkcopy(mp, (char*)msgp, MSGLEN);
				sendrq(rqptr, mp, MSGLEN);
			    } else
				abort(rqptr, TIMEDOUT); /* abort !! */
			else {		/* resend the request  */
#ifdef DEBUG
			    kprintf("msg timed-out, resending msg:  msg num = %d, type = %d\n",
				    msgp->msg_num, msgp->msg_type);
#endif
			    badmesg = msgp->msg_num;
			    resend(rqptr, mp, msgp);
			    if (msgp->msg_type==START_REQ &&
				rqptr->sent == 2)
				kprintf("PS no response, still trying ...\n");
			}
		    } /* if TIMOUT ends      */
		} /* "for" loop ends     */
	    } /* prlodest TIMOUT ends*/
	} /* if (prlsize) ends   */
	signal(xpppgingsem);
	/* call Xinu receive with timeout */
	timdif= TIMOUT-clktime+prloldest;

	if (timdif > 0)
	    ret = recvtim(timdif*10); 

	/* handle the End-to-End NACK informed by pgrcvr */

	if (ret > 0) {			/* pgsdr is given a msg number */
	    wait(xpppgingsem);
	    if((rqptr = xppfindrq(ret, &rqptr)) != RQSYSERR) {
		/* this is the case when pgrcvr received NACK from the PS */
		msgp = rqptr->msgptr;	
		rqptr->sent = 0;	
		resend(rqptr, mp, msgp);
	    } /* ignore NACK if rqt is not found  */
	    signal(xpppgingsem);
	}
    }
}


/*--------------------------------------------------------------------------
 *  pgrcvr - client paging receiver
 *--------------------------------------------------------------------------
 */
pgrcvr()
{
    struct message *msgp, *mp;
    struct rqnode  *rqp, rqnode;
    struct ppte    ppte;
    struct ppte    *ppteptr;
    struct fte     *ftptr;
    int    i, ftn, id;
    int	   rcount;

    /* allocate space for receive message buffers */
    rmsg = (char *) getmem(MSGLEN);

    rcount = 0;
    while(TRUE) {
	TPCread(rmsg);			/* Transport Protocol  */
	msgp =(struct message*) rmsg;
	xppnettohs(msgp);		/* net-to-host conver. */

#ifdef DEBUG
	kprintf("pgrcvr: got reply (typ:%d id:%d num:0x%x code:%d)\n",
		msgp->msg_type, msgp->pa_id, msgp->page_num, msgp->msg_code);
#endif
	rqp = &rqnode;
	wait(xpppgingsem);

	if (xppfindrq(msgp->msg_num, &rqp)!=RQSYSERR) { /* do nothing if can't */
	    mp = rqp->msgptr;		/* find related request*/
	    switch(msgp->msg_type) {
	      case STORE_RSP:
		/* gather paging stats */
		getrealtim(&tmptime);
		accouttime += (tmptime - (rqp->statvar));
		outtimes[couttimes++] = (tmptime - (rqp->statvar));
		if (couttimes == OTIMSIZE) {
		    couttimes = 0;
		}
		/* end gather paging stats */

		ftn = mp->fram_num;	/* get fram No from rqt*/
		ftptr = &ft[ftn];
		if (ftobsolete(ftn)) {
		    dropnd(rqp,TRUE);
		    break;
		}

		if (msgp->msg_code == OKRES) {
		    ftptr->bits.st.pgout = 0;
		    ftrm(ftn);		/* remove it from M-list*/
		    ppteptr = &ppte;
		    ftgetppte(ftn, ppteptr);
		    setmod(ppteptr, FALSE); /* reset M-bit in pte  */
		    if (ftptr->bits.st.wanted) { /* wanted by the owner */
			ftptr->bits.st.wanted = 0;
			mvtoalist(ftn); /* move to active list */
		    } else {		/*move to r-list, don't*/
#ifdef DEBUG
			kprintf("pgrcvr: freeing frame %d\n", ftn);
#endif
			ftinbf(ftn,FTRTL); /* make it free	     */
			signal(ftfreesem);  
		    }
		    dropnd(rqp,TRUE);
		}else			/* error response     */
		    abort(rqp,ERROR);
		break;  

	      case FETCH_RSP:
		/* gather paging stats */
		getrealtim(&tmptime);
		accintime += (tmptime - (rqp->statvar));
		intimes[cintimes++] = (tmptime - (rqp->statvar));
		if (cintimes == ITIMSIZE) {
		    cintimes = 0;
		}
		/* end gather paging stats */
		ftn = mp->fram_num;
		if (ftobsolete(ftn)) {	/* owner's killed    */
		    dropnd(rqp,TRUE);
		    break;
		}
		if (msgp->msg_code == OKRES) {
		    mvtoalist(ftn);
		    ppteptr = &ppte;
		    ftgetppte(ftn, ppteptr);
		    sethvalid(ppteptr, TRUE);
		    setreference(ppteptr, TRUE);
		    id = mp->pa_id;		       
		    if (isasid(id))	/* if frame's id is asid */
			id = mp->msg_code; /* proc_id's saved here  */
		    dropnd(rqp, TRUE);
		    signal(proctab[id].pginsem);
		}else 
		    abort(rqp,ERROR);
		break;

	      case CREATE_RSP:
		if (msgp->msg_code == OKRES){
		    i = mp->pa_id;
		    if (ispid(i))
			proctab[i].psinformed = 1;
		    else if ( isasid(i))
			addrtab[asidindex(i)].psinformed = 1;
		    dropnd(rqp, TRUE);
		}
		/* the proc's page can be written out now*/
		else 
		    abort(rqp, ERROR);
		break;

	      case TERMIN_RSP:
		dropnd(rqp,TRUE);
		break;

	      case START_RSP:
		if (msgp->msg_code == ERROR)
		    kprintf("\nThe PS found M-table-entry error !\n");
		else {
		    psready = TRUE;	/* ps is ready */
		    kprintf("\nPage Server is ready for this machine !\n");
		}
		dropnd(rqp, TRUE);
		send(sendproc, 0);	/* wake up page sender */
		break;

	      case END_RSP:
		dropnd(rqp, TRUE);
		break;

	      case NACK_REQ:		/* PS received out-of-order message  */
		rcount--;		/* keep same by dec now, inc later */
		send(sendproc,msgp->msg_num); /* informs sender    */
		break;

	      case BADREQ:
		kprintf("PS received improper machine id or unknown ");
		kprintf("request from this machine!\n");
		abort(rqp,ERROR);
		break;

	      default:
		kprintf(" paging receiver gets unknown response!\n");
		kprintf(" paging receiver gets type=%d\n",msgp->msg_type);
		dropnd(rqp, TRUE);
	    }
	}
	signal(xpppgingsem);
	/* when M-list is long and R-list is too short, the following is
	   useful since paging sender usually does not work continuously */

	rcount++;
	if ((ftinfo.rlen < FM_LOW) || (rcount >= (MAXPRLLEN >> 1))) {
	    rcount = 0;
	    send(sendproc, OK);		/* try to speed up paging out */
	}
    }
}

/*-------------------------------------------------------------------------
 * xpppgiostart - start the xpp page server processes
 *-------------------------------------------------------------------------
 */
xpppgiostart()
{
    /* Open a datagram device to talk to the Page Sever */
    if ((pgsdev = open(UDP, PGSERVERADDR, ANYLPORT)) == SYSERR) {
	panic("error opening page server device");
    }

    /* set DATA_ONLY mode       */
    control(pgsdev, DG_SETMODE, DG_DMODE);

    xpppgingsem = screate(1);		/* create xpppgingsem	*/

    kprintf("...strating paging sender process\n");
    /* start paging sender process    */
    sendproc = kcreate(PGSEND, PGSSTK, PGSPRI, PGSNAM, PGSARGC );
    resume(sendproc);
    
    kprintf("...strating paging receiver process\n");
    /* paging receiver process  */
    recvproc = kcreate(PGRCVR, PGRSTK, PGRPRI, PGRNAM, PGRARGC );
    resume(recvproc);
}

/*--------------------------------------------------------------------------
 *  resend - resend a request
 *--------------------------------------------------------------------------
 */
resend(rqptr,mp,msgp)
     struct rqnode *rqptr;
     struct message *mp, *msgp;
{
    int  len;

    blkcopy(mp, (char*)msgp, MSGLEN);
    len = MSGLEN;
    if (msgp->msg_type == STORE_REQ)
	len += PGSIZ;
    sendrq( rqptr, mp, len );
}

/*--------------------------------------------------------------------------
 *  fillmsg - fill client parameters into message to be sent
 *--------------------------------------------------------------------------
 */
LOCAL void fillmsg(mptr)
     struct message *mptr;
{
    int  i;

    mptr->msg_num  = (++msgcount>MAXMNO)? (msgcount-MAXMNO): msgcount ;
    /* incremented msg count */
    for (i=0; i<4; i++)
	mptr->mach_id[i] = machineid[i];
    mptr->page_size = PGSIZ;
}

/*-------------------------------------------------------------------------
 *  sendrq - send a request to server through Transport Protocol
 *-------------------------------------------------------------------------
 */
LOCAL void sendrq(ndp,mptr,length)
     struct rqnode  *ndp;
     struct message *mptr;
     int    length;
{

#ifdef DEBUG
    kprintf("sendrq: sending request (typ:%d id:%d num:0x%x frame:0x%x)\n",
	    mptr->msg_type, mptr->pa_id,
	    mptr->page_num,mptr->fram_num);
#endif

    ndp->listlen = prlsize;		/* for measurement */
    ndp->timestp = clktime;
    getrealtim(&tmptime);
    ndp->statvar = tmptime;		/* statistical variable	*/
    ndp->sent++;			/* increment times       */
    xpphstonet(mptr);			/* host-to-net conversion */
    TPCwrite(ndp->msgptr, mptr, length); /* transport protocol    */
}

/*--------------------------------------------------------------------------
 *  insertr  - insert rqnode into tail of a request list
 *--------------------------------------------------------------------------
 */
LOCAL void insertr(ndpt,listail)
     struct rqnode *ndpt;
     int listail;
{
    struct rqnode *np;

    np = rq[listail].prevnd;
    ndpt->prevnd = np;
    ndpt->nextnd = &rq[listail];
    rq[listail].prevnd = np->nextnd = ndpt;
}

/*--------------------------------------------------------------------------
 *  enqrqt  - enqueue request from head of a list to tail of another list
 *--------------------------------------------------------------------------
 */
LOCAL struct rqnode *enqrqt(rqlisthd,rqlisttl,ltail)
     int rqlisthd;
     int rqlisttl;
     int ltail;
{
    struct rqnode *rqptr;

    if ((rqptr = rq[rqlisthd].nextnd) == &rq[rqlisttl])
	return(RQSYSERR);		/* no request on list   */

    rq[rqlisthd].nextnd = rqptr->nextnd; /* adjust original list */
    rqptr->nextnd->prevnd = &rq[rqlisthd];
    insertr(rqptr,ltail);		/* add to pending list  */

    return(rqptr);
}

/*--------------------------------------------------------------------------
 *  mkrqt - make a request
 *--------------------------------------------------------------------------
 */
LOCAL int mkrqt(ftenum, rqtype, mptr)
     int    ftenum;				/* fte number on M-list  */
     int rqtype;				/* request type          */
     struct message **mptr;
{
    int    *mbuf;
    struct fte     *ftep;

    if ((mbuf = (int *) getbuf(msgpool))==(int *)SYSERR) /* can't get msg buffer */
	return(SYSERR);

    ftep = &ft[ftenum];
    *mptr = (struct message *)mbuf;
    (*mptr)->msg_type = rqtype;		
    (*mptr)->pa_id  = ftep->id;
    (*mptr)->page_num = ftep->pageno;	/* get page number       */
    (*mptr)->fram_num = ftenum;

    return(OK);
}

/*--------------------------------------------------------------------------
 *  tkoffrq - take off requests of same id  on pending & pg_in request lists
 *--------------------------------------------------------------------------
 */
LOCAL void tkoffrq(rqp)
     struct  rqnode *rqp;
{
    int id;
    struct rqnode *rqptr;

    id = rqp->msgptr->pa_id;
    rqptr = rq[PENDINGHD].nextnd;	/* drop from pending list */
    while( rqptr != &rq[PENDINGTL]) {
	if ( rqptr != rqp && rqptr->msgptr->pa_id == id  )
	    dropnd(rqptr,TRUE);
	rqptr = rqptr->nextnd;
    }

    rqptr = rq[PGINRQHD].nextnd;	/* drop from page-in list */
    while( rqptr != &rq[PGINRQTL]) {
	if ( rqptr->msgptr->pa_id == id  )
	    dropnd(rqptr,FALSE);
	rqptr = rqptr->nextnd;
    }
}

/*--------------------------------------------------------------------------
 *  dropnd - drop a rqnode from pending request list
 *--------------------------------------------------------------------------
 */
LOCAL void dropnd(ndp,list)
     struct rqnode *ndp;
     Bool   list;		/* 1=> from pending list; 0=> pgin list  */
{
    ndp->prevnd->nextnd = ndp->nextnd;
    ndp->nextnd->prevnd = ndp->prevnd;
    if (list)
	prlsize--;

    freebuf((int *)ndp->msgptr);
    freebuf((int *)ndp);
}

/*--------------------------------------------------------------------------
 *  abort -  abort the pending request, kill associated process if exists
 *           and notify the server
 *--------------------------------------------------------------------------
 */
LOCAL void abort(rpt,reason)
     struct rqnode *rpt;                            
     int reason;
{
    int    id, type;
    struct message *msgp;

    msgp = rpt->msgptr;
    type = msgp->msg_type;
    switch (type) {				
      case STORE_REQ:
	if(reason==TIMEDOUT)
	    kprintf("store-page rqt timed out twice!\n");
	else
	    kprintf("store-page rqt gets error response!\n");
	break;
      case FETCH_REQ:
	if(reason==TIMEDOUT)
	    kprintf("fetch-page rqt timed out twice!\n");
	else
	    kprintf("fetch-page rqt gets error response!\n");
	break;
      case TERMIN_REQ:			/* in this case, called only when timeout */
	kprintf("terminate-process request timed out too ");
	kprintf("many times! id = %d\n", msgp->pa_id);
	dropnd(rpt,TRUE);
	return;
      case CREATE_REQ:
	if (reason==TIMEDOUT)
	    kprintf("create-process request timed out!\n");
	else
	    kprintf("create-proc rqt gets err response!\n");
	break;
      case START_REQ:	
	panic("No page server response to start request! ");

      case CRSTOR_REQ:  ;
      default: ;
    }

    tkoffrq(rpt);			/* take off same id requests, so only the TERMIN-REQ */
    /* of the id exists. Ignore other RSP from the PS    */
    kill(id = msgp->pa_id);
    dropnd( rpt, TRUE );
    send(fmgr, OK);	/* awakmanager in case frames needed */

    if (isasid(id))
	kprintf(" asid =%d & associsted proc's were aborted\n",id);
    else if (ispid(id))
	kprintf(" pid =%d was aborted\n",id);
}

/*--------------------------------------------------------------------------
 *  xppfindrq - find corresponding request on the pending request list
 *--------------------------------------------------------------------------
 */
struct rqnode *xppfindrq(msgn,rqp)
     int msgn;
     struct  rqnode **rqp;
{

    if ((*rqp = rq[PENDINGHD].nextnd)==&rq[PENDINGTL])
	return(RQSYSERR);

    while (*rqp!=&rq[PENDINGTL]) {
	if ((*rqp)->msgptr->msg_num==msgn)
	    return(*rqp);
	else
	    *rqp = (*rqp)->nextnd;
    }

    return(RQSYSERR);
}

/*-------------------------------------------------------------------------
 * xpppgioinit - initialization for xpp paging subsystem
 *-------------------------------------------------------------------------
 */
xpppgioinit()
{
    PStype ps;
    IPaddr myipaddr;
    int i,size;
    struct message *mptr;
    struct rqnode  *rqp;


    getaddr(myipaddr);
    for (i=0; i<IP_ALEN; i++)
	machineid[i] = myipaddr[i];

    wpwp = (TPPacketType *)getmem(sizeof(TPPacketType));
    rpwp = (TPPacketType *)getmem(sizeof(TPPacketType));
    /* for lower level protocol, int boundary guaranteed*/


    rq[PENDINGHD].nextnd = &rq[PENDINGTL]; /* empty prlist           */
    rq[PENDINGTL].prevnd = &rq[PENDINGHD];

    rq[PGINRQHD].nextnd = &rq[PGINRQTL]; /* empty page in rqt list */
    rq[PGINRQTL].prevnd = &rq[PGINRQHD];

    rq[PROCRQHD].nextnd = &rq[PROCRQTL]; /* empty proc c/t rqt list*/
    rq[PROCRQTL].prevnd = &rq[PROCRQHD];

    prlsize = 0;
    msgcount = 1;			/* 0 might cause confusion*/

    size = sizeof(struct rqnode);
    nodepool= mkpool(size, RQNUM);	/* make rqnode-buf pool   */

    msgpool = mkpool(MSGLEN+sizeof(int), RQNUM); /* make msg-buf pool      */

    disable(ps);

    mptr = (struct message *)getbuf(msgpool);
    rqp = (struct rqnode *)getbuf(nodepool);

    mptr->msg_type = START_REQ;		/* fill in start req message */
    mptr->pa_id = getpid();		/* fill in pgsender id	*/
    rqp->msgptr  = mptr;		/* fill in request node      */
    rqp->sent    = 0;			/* request has not been sent */
    insertr(rqp,PROCRQTL);		/* add rqnode to proc create/
					   terminate rqt list tail   */
    restore(ps);
}


/*-------------------------------------------------------------------------
 * xpppgioterminate - terminate the process or address space given
 *-------------------------------------------------------------------------
 */
xpppgioterminate(id)
     int id;
{
    struct message *mptr;
    struct rqnode  *rqp;
    int    *buf1, *buf2;

    if ((buf1 = (int *) getbuf(msgpool))== (int *)SYSERR  ||
	(buf2 = (int *) getbuf(nodepool))== (int *)SYSERR )
	return(SYSERR);

    mptr = (struct message *)buf1;
    rqp = (struct rqnode *)buf2;

    mptr->msg_type = TERMIN_REQ;	/* fill in request message   */
    mptr->pa_id  = id;
    rqp->msgptr  = mptr;		/* fill in request node      */
    rqp->sent    = 0;			/* request has not been sent */

    insertr(rqp,PROCRQTL);		/* add rqnode to proc create
					   terminate rqt list tail   */
    send(sendproc, 0);
    return(OK);
}

/*-------------------------------------------------------------------------
 * xpppgiocreate - create a new process or address space
 *-------------------------------------------------------------------------
 */
xpppgiocreate(id)
     int id;
{
    int    *buf1, *buf2;
    struct message *mptr;
    struct rqnode  *rqp;

    if ((buf1 = (int *) getbuf(msgpool))== (int *)SYSERR  ||
	(buf2 = (int *) getbuf(nodepool))== (int *)SYSERR ) {
	return(SYSERR);
    };

    mptr = (struct message *)buf1;
    rqp = (struct rqnode *)buf2;

    mptr->msg_type = CREATE_REQ;	/* fill in request message   */
    mptr->pa_id  = id;
    rqp->msgptr  = mptr;		/* fill in request node      */
    rqp->sent    = 0;			/* request has not been sent */

    insertr(rqp, PROCRQTL);		/* add rqnode to proc create*/

    send(sendproc, 0);
    return(OK);
}


/*-------------------------------------------------------------------------
 * xpppgiogetpage - get a page from the swap space
 *-------------------------------------------------------------------------
 */
xpppgiogetpage(id,findex,virt_addr)
{
    PStype	ps;
    struct message msg;
    struct message *mptr;
    struct rqnode *rqptr;

    mptr = &msg;
    if(mkrqt(findex,FETCH_REQ,&mptr)== SYSERR) {
#ifdef DEBUG
	kprintf("pfault: got SYSERR from mkrqt\n");
#endif
	restore(ps);
	return(SYSERR);
    }

    if (isasid(id))			/* have to save proc id for */
	mptr->msg_code = currpid;	/* pgrcvr to resume it, we  */

    /* borrow the msg_code field */
    rqptr = (struct rqnode *)getbuf(nodepool);
    rqptr->msgptr = mptr;
    rqptr->sent = 0;
    wait(xpppgingsem);
    insertr(rqptr, PGINRQTL);
    signal(xpppgingsem);
    wait(proctab[currpid].pginsem);

    send(sendproc, 0);
    return(OK);
}
