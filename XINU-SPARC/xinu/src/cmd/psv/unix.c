
/* 
 * unix.c - The NAFP protocol routines
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Jul 27 17:07:11 1990
 *
 * Copyright (c) 1990 Jim Griffioen
 */

/*#define DEBUG*/
/*#define USE_USER_BCOPY*/
/*#define USE_XLARGE_PACKET*/

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
#include <strings.h>
#include "tp.h"
#include "psv.h"

#ifdef USE_USER_BCOPY
#define bcopy(f,t,l) user_bcopy(f,t,l)
#endif

/* Global */

#if defined(VAX) || defined(SUN) || defined(DEC)
#define SOCKADDR_USEFULPART_LEN 8
#else
#define SOCKADDR_USEFULPART_LEN sizeof(struct sockaddr_in)
#endif

#define XLARGE_PACKET 2000

int fromlen = sizeof(struct sockaddr_in);
TPReadListType TPReadList[MAXTPREADLIST];

#ifdef USE_XLARGE_PACKET
char wtemp[XLARGE_PACKET], rtemp[XLARGE_PACKET];
#else
TPPacketType wtemp, rtemp;
#endif

/*==============================================================
 *	  TPSwrite -- Transport Protocol Server Write Routine
 *===============================================================
 */
	    
TPSwrite(msgp, p, DataLen, pfrom, ptrs)
struct message *msgp, *p;
int    DataLen;
struct sockaddr_in *pfrom;
struct d_block  *ptrs[MAX_PER_PAGE];
{
    register TPPacketType *wpwp;
    int    dev;
    char   *fragptr, *saddr;
    int    NumFrag, nf, fragsize;
    int    currfrag, copysize, cpsize, i, len, offset;
    char   sendpg;
    int  bindx, bufnum;
struct message *mp;

    /* What is the number of fragments we need to break this mesg */
    i = DataLen / FRAGDATASIZE;
    NumFrag = (DataLen % FRAGDATASIZE == 0)? i : i + 1;

    if (NumFrag > MAXFRAGNO) {
       psvlog(DBGHIGH, "The data is too long !\n");
       return(SYSERR);
    }

    currfrag  = 0;
    sendpg = FALSE;
    bindx = 0;

#ifdef USE_XLARGE_PACKET
    wpwp = (TPPacketType *)
	   (( (int) &wtemp[0]) + (XLARGE_PACKET - sizeof(TPPacketType) - 1));
#else
    wpwp = &wtemp;
#endif
    bcopy( p, wpwp->Data, MSGLEN );
    
    DataLen -= MSGLEN;

    if ( DataLen > 0 ) {	/* there is page data to be written out */
	fragptr = wpwp->Data + MSGLEN;
	saddr = ptrs[bindx]->data;
	offset = 0;
	cpsize = copysize = ( DataLen > FRAGDATASIZE - MSGLEN )?
	    			FRAGDATASIZE - MSGLEN : DataLen;
	sendpg = TRUE;
    }

    nf = NumFrag;
    while (nf > 0 ) {
	if ( nf == NumFrag ) {
          	wpwp->PacketType     = htonl(NSETE);
          	wpwp->MesgNum        = htonl(msgp->msg_num);
          	wpwp->TotalFrags     = htonl(NumFrag);
          	wpwp->HeadLength     = htonl(HEADLENGTH);
          	wpwp->MaxDataLen     = htonl(FRAGDATASIZE);
	}
#ifdef DEBUG
	psvlog(DBGMED, "wm=%d,f=%d\n", msgp->msg_num, currfrag);
#endif
	wpwp->FragNum  = htonl(currfrag++);
        fragsize = HEADLENGTH;
	
	if (NumFrag > 1) {		
	      while (1) {		/* this loop for copying a frag */
		len = (copysize > PSIZE-offset)? PSIZE-offset : copysize;

#ifdef OLDWAY
		if (isintaddr(saddr) && isintaddr(fragptr) && ismultint(len)) 
		    fastbcopy((unsigned int *)saddr,(unsigned int *)fragptr,
			   len/sizeof(int));
		else
#endif
	      	    bcopy(saddr, fragptr, len);
		
				  /* adjust source addr and offset for next */
		if ( len + offset == PSIZE ) {	/* one d_blk copied */
    		     saddr = ptrs[++bindx]->data;
		     offset = 0;
		}else {				/* still in the current d_blk*/
		     saddr += len;
		     offset += len;
		}
		if ((copysize -= len) == 0)
		    break;		/* copying completes for this frag. */
		fragptr += len;		/*continue copying, advance dest-addr*/
	      }					/* copying a frag finishes */

	      fragsize += cpsize;
	      if (nf == NumFrag )
		  fragsize += MSGLEN;
	      fragptr = wpwp->Data;	/* prepare for the next packet */
	      DataLen -= cpsize;
	      cpsize =copysize =(DataLen > FRAGDATASIZE)?FRAGDATASIZE :DataLen;
	}
        else {						/* NumFrag == 1 */
	      fragsize += MSGLEN;
	      if (sendpg) {				/* with page data  */
#ifdef DEBUG
		      psvlog(DBGMED, "sending single packet with data\n");
		      psvlog(DBGMED, "DataLen = %d\n", DataLen);
#endif
#ifdef OLDWAY
		 if (isintaddr(saddr) && isintaddr(fragptr)
		     && ismultint(DataLen)) 
		    fastbcopy((unsigned int *)saddr,(unsigned int *)fragptr,
			   DataLen/sizeof(int));
		 else
#endif
		    bcopy(saddr, fragptr, DataLen);
		     	/*suppose DataLen<=PSIZE since FRAGDATASIZE>PSIZE now*/
		 fragsize += DataLen;
	      }
	}      
	wpwp->FragSize = htonl(fragsize);

#ifdef TIMING
	mp = (struct message *)wpwp->Data;
	if (mp->msg_type == htonl(STORE_RSP)) 
	    llstacc += (currtime() - llst);
	if (mp->msg_type == htonl(FETCH_RSP)) 
	    cc += (currtime() - llft);
#endif

        sendto(SOCK_FD, wpwp, fragsize, 0, pfrom, fromlen);
	nf--;
      }

#ifdef DEBUG
    psvlog(DBGMED, "wmdone,#%d,typ=%d\n", msgp->msg_num, ntohl(p->msg_type));
#endif
    
        if (freeTPReadList(msgp->msg_num, pfrom) == SYSERR) {
	     psvlog(DBGHIGH, "Error : Cannot free TPReadList ! mesg # %d, pfrom %09x\n",msgp->msg_num, pfrom);;
	     return(SYSERR);
	}
	else
             return(OK);
}

/*********************************************************************
   
     freeTPReadList - marks the buffer that conatin this mesgno and 
       client to be UNUSED.

**********************************************************************/

freeTPReadList(mesgno, pfrom) 
int mesgno;
struct sockaddr_in *pfrom;
{
    int i,j;
    register TPReadListType *ptr;

    for (i=0; i<MAXTPREADLIST; i++) {
       ptr = &TPReadList[i]; 
       if ((ptr->msgno == mesgno)  && 
	   (bcmp(&(ptr->from) ,pfrom, SOCKADDR_USEFULPART_LEN) == 0)) 
	{      							
	       ptr->msgno = UNUSED;
	       ptr->totalfrag  = 0;
	       ptr->curfrag  = -1;
               ptr->dblkavail  = 0;
	       ptr->dblknum  = 0;
	       ptr->cur_dblk  = 0;
	       ptr->offset  = 0;
	       for (j=0; j<MAXFRAGNO; j++)
		  ptr->bitmap[j] = 0;
	       return(0);
	}
    }	
#ifdef DEBUG
    for (i=0; i<MAXTPREADLIST; i++) {	
	    ptr = &TPReadList[i];
	    if( ptr->msgno != UNUSED)
	    	psvlog(DBGMEDLOW, "TPReadList[i].msgno=%d &(TPReadList[i].from)=%09x\n",TPReadList[i].msgno,(unsigned)(&(TPReadList[i].from)));
    }
#endif
    
    return(SYSERR);     
}

/********************************************************************

   LookupReadList -  Look thru the ReadList and look for one containing
       the mesgno and client. If not found, give me a unused buffer.
       else error.

*********************************************************************/

LookupReadList(mesgno,pfrom)
int mesgno;
struct sockaddr_in *pfrom;
{
    int i;
    int unused = -1;
    register TPReadListType *ptr;

    for (i=0; i<MAXTPREADLIST; i++) {
       ptr = &TPReadList[i];
       if (ptr->msgno == UNUSED) 
	  unused = i;
       else if ( ptr->msgno == mesgno  && 
	   	 bcmp(&(ptr->from) ,pfrom, SOCKADDR_USEFULPART_LEN) == 0 ) 
	       return(i);
    }
    
    if (unused != -1) {    /* Give a new buffer */
       ptr = &TPReadList[unused];
       ptr->msgno = mesgno;
       bcopy(pfrom, &(ptr->from), fromlen);
    }
    else
       psvlog(DBGHIGH, " Panic. No more UNUSED buffers\n");

    return(unused);    /* cannot found any buffer to use at all */
}


/*====================================================================

     TPSread -- called by Page Server to get a request.

====================================================================*/

TPSread(pBuf, Datap, pfrom, dnum)
char **pBuf;
struct d_block  *Datap[MAX_PER_PAGE];
struct sockaddr_in *pfrom;
int  *dnum;
{
    char  done=0;
    register TPPacketType *rpwp;
    register TPReadListType  *ReadListp;
    int type, mesgno, totalfrag, fragno;
    int headsize, fragsize, datalen, mfragdatalen;
    int num, i, j, k, n,copylen, len, pgofst, pgdatalen;
    struct message *mptr;
    char *sptr,*dptr;
int *p,*q;
    
    while (!done) {    /* while have not recieved a full msg */


#ifdef USE_XLARGE_PACKET
       rpwp = (TPPacketType *)
	      (((int) &rtemp[0]) + (XLARGE_PACKET - sizeof(TPPacketType) - 1));
#else
       rpwp =  &rtemp;
#endif
       if (recvfrom(SOCK_FD, rpwp, FRAGSIZE, 0, pfrom, &fromlen) == SYSERR)
          perror("recvfrom");

       mptr = (struct message *)rpwp->Data;

type = ntohl(mptr->msg_type);
if (type == STORE_REQ) 
		    llst = currtime();	

if (type == FETCH_REQ) 
		    llft = currtime();

       
       if ((type = ntohl(rpwp -> PacketType)) != NSETE && type != REPLNACK ) {
	   psvlog(DBGHIGH, "PS received neither NSETE nor REPLNACK packet\n");
	   continue;
       }
       mesgno    = ntohl(rpwp -> MesgNum);
       totalfrag = ntohl(rpwp->TotalFrags);
       fragno    = ntohl(rpwp->FragNum);
       headsize  = ntohl(rpwp->HeadLength);
       fragsize  = ntohl(rpwp->FragSize);
       mfragdatalen = ntohl(rpwp->MaxDataLen);

     if ((i = LookupReadList(mesgno, pfrom)) != -1)  {
						/* found existing or new buf */
	 if (TPReadList[i].bitmap[fragno] == 1)   	
		continue;				/* duplicate frag */

	 ReadListp = &TPReadList[i];
	 datalen = fragsize - headsize;
	 if ( fragno == 0 ) 		/* frag. 0 always contains msg. */
       		bcopy( rpwp->Data, ReadListp->msgp , MSGLEN );
	 						/* copy message */
#ifdef DEBUG
	 psvlog(DBGMED, "rm=%d,e=%d,g=%d\n", mesgno, ReadListp->curfrag+1, fragno);
#endif
	 if ( !fragno && datalen > MSGLEN || fragno>0 ) {
	     					/* the packet has page data */
	   if (!ReadListp->dblkavail) {		/* d_block(s) unavailable */
	       if ( (k = ntohl(mptr->page_size)) <= PSIZE )
		   num = 1;			/* number of d_blk needed */
	       else 
	       	   num = ( k % PSIZE == 0)? k / PSIZE : k / PSIZE + 1;
	       
	       if ( Get_Free(ReadListp->datap, num) == ERROR) {
		  /* free any blocks occupied by invalid G-entries. */
#ifdef DEBUG
		  psvlog(DBGMEDHIGH,
			 "Out of data blocks: Doing garbage collection\n");
#endif
		  garbagecollect(10);	/* garb collect enough to keep going */
					/* don't want to delay too long here */
		  if (Get_Free(ReadListp->datap, num) == ERROR){
			  				/* no free blocks */
			psvlog(DBGHIGH, "PS used up all the data blocks!!\n");
		   	return(SYSERR);
		  }
	       }
	       ReadListp->dblknum = num;
	       ReadListp->dblkavail = 1;
           }

	   /* copy (page) data from fragment into data_block(s) */
	   
	   sptr = ( fragno == 0 )? rpwp->Data + MSGLEN : rpwp->Data;
	   						/* source pointer */
	   pgdatalen = copylen = ( fragno == 0 )? (datalen - MSGLEN) : datalen;

	   if ( fragno == ReadListp->curfrag + 1 ) {	/* the expected frag.*/
		k = ReadListp->offset;	        	/* offset in d_block */
		n = ReadListp->cur_dblk;		/* d_block index */
	   }
	   else {			/* fragno > the expected frag. no */
#ifdef DEBUG
		psvlog(DBGMED, "\ngot packet out of order - should send NACK\n");
#endif
		/* first frag has size = (mfragdatalen-MSGLEN) */
		pgofst = fragno*mfragdatalen - MSGLEN;
		k = pgofst % PSIZE;
		n = pgofst / PSIZE;
#ifdef DEBUG
		psvlog(DBGMED, "fragno = %d, mfragdatalen = %d\n",
		       fragno, mfragdatalen);
#endif
	   }
	   len = ( PSIZE - k >= copylen )? copylen : PSIZE - k;
	   					/* current copy length */
	   while ( 1 ) {
	   	dptr = ReadListp->datap[n]->data + k;
						/* destination pointer */
#ifdef OLDWAY
		if (isintaddr(sptr) && isintaddr(dptr) && ismultint(len)) 
		    fastbcopy((unsigned int *)sptr,(unsigned int *)dptr,
			   len/sizeof(int));
		else
#endif
		    bcopy( sptr, dptr, len );

		if ( (k += len) == PSIZE ) {
		     n++;
		     k = 0;
	        }
		if ( (copylen -= len) == 0 )	/* data copying completed */
		    break;
		sptr += len;
		len = (copylen > PSIZE)? PSIZE : copylen;
						/* next copy length */
	   }

	   /* if fragno is expected, adjust curfrag, cur_dblk, offset */
           if (fragno == ReadListp->curfrag + 1){ 
	        ReadListp->curfrag++;
		ReadListp->offset = k;
	        ReadListp->cur_dblk = n;
		
		/* for NACK reply, may need further adjustment */
	        if (type == REPLNACK && (j = fragno + 1) != totalfrag-1 ) 
	            while (ReadListp->bitmap[j]) {    /* next frag. received */
		           ReadListp->curfrag++;	
			   k += mfragdatalen;
			   while ( k >= PSIZE ) {
				k -= PSIZE;
				n++;
			   }
			   ReadListp->offset = k;
	        	   ReadListp->cur_dblk = n;
		           j++;		        
	            }
	   }else {		/* the frag. is beyond the expected one */
		   /* only send a NACK if you are the first packet to	*/
		   /* come in after a missing packet			*/
		if ((type == NSETE) && (ReadListp->bitmap[fragno-1]==0)) {
#ifdef DEBUG
			psvlog(DBGMED, "sending NAFP NACK\n");
#endif
			sendnack(mesgno, ReadListp->curfrag+1, pfrom);
		}
	   }  				/* send Nacks for missing fragments */

	   ReadListp->bitmap[fragno] = 1; 		/*received this frag */
	   
	 } 					  /* page data handling ends*/

	 ReadListp->totalfrag++;
	 
	   				/* check if we have a complete msg */
      	 if (ReadListp->totalfrag >= totalfrag) {
	   *pBuf = (char *)ReadListp->msgp;
	   for (j=0; j < ReadListp->dblknum; j++ )
		Datap[j] = ReadListp->datap[j];	/*copy d_blk ptr's */
	   *dnum = ReadListp->dblknum;
	   done = 1;
#ifdef DEBUG
	   psvlog(DBGMED, "rmdone,#%d,typ=%d\n", ntohl(ReadListp->msgp->msg_num),
		  ntohl(ReadListp->msgp->msg_type));
#endif
      	 }
     }else {      		/*more uncompleted message than I can handle */
             psvlog(DBGHIGH, "Too many mesgs arrive at PS. Droping mesg # %d\n",mesgno);
	     return(SYSERR);
     }
   } 							/* done loop ends */
   return(OK);
}

/**************************************************************
   
    sendnack - send a nack to the client using the structure pfrom

******************************************************************/

sendnack(msgnum, n, pfrom)
int msgnum;
int n;
struct sockaddr_in *pfrom;
{
       TPPacketType temp;
       short   len;

       temp.PacketType	= htonl(NACK);
       temp.MesgNum	= htonl(msgnum);
       temp.TotalFrags	= htonl(1);
       temp.FragNum	= htonl(n);
       temp.HeadLength	= htonl(HEADLENGTH);
       len = HEADLENGTH+MSGLEN;
       temp.FragSize	= htonl(len);
       temp.MaxDataLen	= htonl(FRAGDATASIZE);
#ifdef DEBUG
       psvlog(DBGMED, "in sendnack - calling sendto()\n");
#endif

       sendto(SOCK_FD,&temp,len,0,pfrom,fromlen);
}

/****************************************************************************
    InitTPbuff()
       init the TPReadList buffers on the server side.
*****************************************************************************/
InitTPbuff()
{
   int i,j;
   TPReadListType *ptr;
   
   for (i=0; i<MAXTPREADLIST; i++) {
      ptr = &TPReadList[i];
      ptr->msgno = UNUSED;
      ptr->curfrag = -1;
      ptr->totalfrag = 0;
      ptr->dblkavail = 0;
      ptr->dblknum = 0;
      ptr->offset   = 0;
      ptr->cur_dblk = 0;
      for (j=0; j<MAXFRAGNO; j++)
	 ptr->bitmap[j] = 0;
      ptr->msgp = (struct message *)malloc(MSGLEN);
   }

   /* zero fill the temp tp packets */
#ifndef USE_XLARGE_PACKET
   wtemp.PacketType = 0;
   wtemp.MesgNum = 0;
   wtemp.TotalFrags = 0;
   wtemp.FragNum = 0;
   wtemp.HeadLength = 0;
   wtemp.FragSize = 0;
   wtemp.MaxDataLen = 0;

   rtemp.PacketType = 0;
   rtemp.MesgNum = 0;
   rtemp.TotalFrags = 0;
   rtemp.FragNum = 0;
   rtemp.HeadLength = 0;
   rtemp.FragSize = 0;
   rtemp.MaxDataLen = 0;
#endif
}
 

/****************************************************************************
    PrintPacket takes a wp packet and prints the content out for debugging 
    purposes.
*****************************************************************************/
printpacket(dbglevel, temp)
int dbglevel;
TPPacketType temp;
{
	  int i;
	  
	  switch (temp.PacketType) {
	    case NSETE : psvlog(dbglevel, "NSETE packet\n");
			  break;
	    case NACK :  psvlog(dbglevel, "Nack Packet\n");
			 break;
	    case REPLNACK :  psvlog(dbglevel, "ReplNack Packet\n");
			 break;
	  }
          psvlog(dbglevel, "Mess No = %d\n",	ntohl(temp.MesgNum));
          psvlog(dbglevel, "Total Frag = %d\n",	ntohl(temp.TotalFrags));
          psvlog(dbglevel, "Frag Num = %d\n",	ntohl(temp.FragNum));
          psvlog(dbglevel, "HeadLength = %d\n",	ntohl(temp.HeadLength));
          psvlog(dbglevel, "FragSize = %d\n",	ntohl(temp.FragSize));
          psvlog(dbglevel, "MaxDatLength = %d\n", ntohl(temp.MaxDataLen));
          for (i=0;i<=32; i++)
	     psvlog(dbglevel, "%x",temp.Data[i]);
	  psvlog(dbglevel, "\n\n");
}

printTPList(dbglevel, totalfrag)
int dbglevel;
int totalfrag;
{
	int i,j;

	for (i=0; i<MAXTPREADLIST; i++) {
	  if (TPReadList[i].msgno != UNUSED) {
	   psvlog(dbglevel, "Buff # %d\n",i);
	   psvlog(dbglevel, "From = %x\n",TPReadList[i].from);
	   psvlog(dbglevel, "Mesgno = %d\n",TPReadList[i].msgno);
	   psvlog(dbglevel, "Totalfrag = %d\n",TPReadList[i].totalfrag);
	   psvlog(dbglevel, "Curfrag = %d\n",TPReadList[i].curfrag);
	   for (j=0; j<MAXFRAGNO; j++)
	      psvlog(dbglevel, "%d   ",TPReadList[i].bitmap[j]);
	   psvlog(dbglevel, "\n");
	   psvlog(dbglevel, "Totalfrag - %d\n",totalfrag);
	  }
	}
	psvlog(dbglevel,
	       "---------------------------------------------------\n");
}

/*====================================================================

     FREEBUF -- Free Read Buffer

====================================================================*/

FreeBuf(msgno,pfrom)
int  msgno;
struct sockaddr_in *pfrom;
{
	freeTPReadList(msgno, pfrom); 
        return(OK);
}
