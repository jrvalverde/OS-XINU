/*---------------------------------------------------------------------------
 * tprotocol.c -  Xinu Client Transport Protocol
 *                (goes with XPP, xpppgio.c)
 *---------------------------------------------------------------------------
 */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>
#include <mem.h>
#include <vmem.h>
#include <frame.h>
#include <pgio.h>
#include <xpppgio.h>
#include <tp.h>

/*#define DEBUG*/

extern int xpppgingsem;
int bitmap[MAXFRAGNO];
int  bufmesgno;

TPPacketType *wpwp, *rpwp;

/*==============================================================
 *	  TPCwrite -- Transport Protocol Client Write Routine
 *===============================================================
 */
TPCwrite(mptr, netmp, DataLen)
struct message *mptr;			/* points to message */
char *netmp;				/* pts to message in net byte order*/
int DataLen;
{
	
	char   *fragptr, *vaddr, *cppgaddr;
	int	n, NumFrag, nf, id;
	int    FragNum, copysize, fragsize;
	int   outsign = 0;			/* 1-> paging out; 0->others */
	
	/* What is the number of fragments we need to break this mesg */
	n = DataLen/FRAGDATASIZE;
	NumFrag = (DataLen % FRAGDATASIZE == 0)? n : n + 1;
	
	if (NumFrag > MAXFRAGNO) {	/* data is too long		*/
		kprintf("TPSwrite: data is too long\n");
		return(SYSERR);
	}
	
	FragNum  = 0;
	nf = NumFrag;
	
	blkcopy(wpwp->Data, netmp, MSGLEN);  	/* copy message first */
	fragsize = HEADLENGTH + MSGLEN;
	
	DataLen -= MSGLEN;
	if (DataLen > 0)	{		/* page-store-request only */
		fragptr = wpwp->Data + MSGLEN; 
		vaddr = (char *)tovaddr(ft[mptr->fram_num].pageno);
		id = ft[mptr->fram_num].id;
		copysize = (DataLen >  FRAGDATASIZE - MSGLEN )?
		    FRAGDATASIZE - MSGLEN : DataLen;
		outsign = 1;			/* this is a paging out */
	}
	
	
	while (nf > 0) {
		if (nf == NumFrag ) {
			wpwp->PacketType     = hl2net(NSETE);
			wpwp->MesgNum        = hl2net(mptr->msg_num);
			wpwp->TotalFrags     = hl2net(NumFrag);
			wpwp->HeadLength     = hl2net(HEADLENGTH);
			wpwp->MaxDataLen     = hl2net(FRAGDATASIZE);
		}
		wpwp->FragNum = hl2net(FragNum++);
		
		if ( outsign ) {
			if ( nf == NumFrag ){	/* first packet includes data*/
				
				cppgaddr =(char *)physpgcopy(vaddr,id,
							     fragptr,currpid,
							     copysize,
							     FALSE);
				/* this return addr is vaddr plus copysize */
				
				fragsize += copysize;
			}
			else {
				if (isintaddr(fragptr) && isintaddr(cppgaddr))
				    fblkcopy( (int *)fragptr,
					     (int *)cppgaddr, copysize);
				/* fast block copy routine */
				else
				    blkcopy( fragptr, cppgaddr, copysize);
				fragsize = HEADLENGTH + copysize;
				cppgaddr += copysize;
			}
			fragptr = wpwp->Data; /* prepare for the next packet */
			DataLen -= copysize;
			copysize = (DataLen > FRAGDATASIZE)?
			    FRAGDATASIZE : DataLen;
		}
		
		wpwp->FragSize = hl2net(fragsize);
		
		if (write(pgsdev, (char *)wpwp, fragsize) != OK){
		        kprintf("in TPwrite: write(pgsdev..) fails\n");
	     		return(SYSERR);
		}
		--nf;
		
	}
	return(OK);
}

/*====================================================================
  
  TPCread -- called by Client to get a response.
  
  ====================================================================*/

TPCread(pData)
char *pData;
{
	char   *ptr, *pcktdatap, *cppgaddr, *curcppgaddr;
	struct message msg, *mp, *mptr;
	struct rqnode *ndptr;
	int  type, mesgno, fragno, totalfrag, fragsize, headlen, maxfdatalen;
/*	int  bufmesgno;*/
	int  offset;
/*	int  bitmap[MAXFRAGNO];*/
	int done=0, counter=0;
	int id, length, j;
char sb[80];

	
	bufmesgno = UNUSED;
	for (j=0; j<MAXFRAGNO; j++)
	    bitmap[j] = 0;
	
	while (!done) {    /* while have not received a full msg */
		
		if (read(pgsdev, (char *)rpwp, FRAGSIZE) == SYSERR)
		    kprintf("read from  pgsdev returns SYSERR\n");

		
		type   = net2hl(rpwp->PacketType);
		mesgno = net2hl(rpwp->MesgNum);
		fragno = net2hl(rpwp->FragNum);
		totalfrag = net2hl(rpwp->TotalFrags);
		headlen   = net2hl(rpwp->HeadLength);
		maxfdatalen = net2hl(rpwp->MaxDataLen);
		fragsize  = net2hl(rpwp->FragSize);

sprintf(sb, "(%d^%d)", mesgno, fragno);
Debug(sb);

		wait(xpppgingsem);
		
		pcktdatap = rpwp->Data;
		if (type == NACK) {		/* possible for multi-packet
						   store-page request only */
#ifdef DEBUG
			kprintf("received a NACK packet, msg #%d, frag #%d\n",
				mesgno, fragno);
#endif
			if (xppfindrq(mesgno, &ndptr)) {   /* Can reply to Nack */
				mptr = ndptr->msgptr;
				length = maxfdatalen;
				if (fragno==0) { /* must copy message first*/
					mp = &msg;
					blkcopy(mp, mptr, MSGLEN);
					xpphstonet(mp);
					blkcopy(pcktdatap, mp, MSGLEN);
					/*prepare for copying pg data*/
					pcktdatap += MSGLEN;
					length -= MSGLEN;
				} else if (fragno == totalfrag-1)
				    /* the last fragment */
				    length = ( MSGLEN + PGSIZ ) % maxfdatalen;
				
				offset = (fragno > 0)?
				    fragno*maxfdatalen - MSGLEN : 0;
				ptr = (char *)tovaddr(mptr->page_num) + offset;
				/*starting addr. of the page to be out*/
				id  = ft[mptr->fram_num].id;
					/* the page's owner */
				
				physpgcopy(ptr, id, pcktdatap, currpid,
					   length, FALSE);
				
				fragsize = (fragno < totalfrag-1)?
				    FRAGSIZE : HEADLENGTH+length;
				rpwp->PacketType = hl2net(REPLNACK);
				rpwp->FragSize = hl2net(fragsize);
#ifdef DEBUG				
				kprintf("resending frag #%d, msg #%d\n",
					fragno, mesgno);
#endif
				if (write(pgsdev,(char *)rpwp,fragsize) != OK)
				    return(SYSERR);
			}
		}
		else if (type == NSETE){ /* normal response */
			/* next message gets here || start of message */
			if (((bufmesgno+1) == mesgno) ||
			    (bufmesgno == UNUSED)) {
				if (fragno==0) { /* drop cur msg, start */
					/* receiving the new message	*/

					/* make sure we are still 	*/
					/* waiting for this message	*/
					if(xppfindrq(mesgno, &ndptr) == SYSERR) {
kprintf("got frag 0 of wrong msg num = %d, bufmsgno = %d\n",
	mesgno, bufmesgno);
						signal(xpppgingsem);
						continue; /* skip it */
					}
					
					bufmesgno = mesgno;
					for (j=0; j<MAXFRAGNO; j++)
					    bitmap[j] = 0;
					counter = 0;
				}else { /* ignore this packet, continue */
					signal(xpppgingsem);
kprintf("got frag = %d of wrong msg num = %d, bufmsgno = %d\n",
	fragno, mesgno, bufmesgno);
					continue;
				}
			}
			
			
			if ((bufmesgno == mesgno) && (bitmap[fragno] == 0)){
				/* normal receiving cases*/
				if ((fragno == 0) &&
				    (fragsize- HEADLENGTH - MSGLEN == 0)) {
					blkcopy(pData, pcktdatap, MSGLEN);
				}
					/* message only */
				else {	/* has page data */
					length = fragsize - headlen;
					if (fragno == 0) {
						blkcopy(pData, pcktdatap,
							MSGLEN);
						pcktdatap += MSGLEN;
						if(xppfindrq(mesgno, &ndptr)==
						   SYSERR) { /* orig rqst */
kprintf("dropping packet cause we were not waiting for it\n");
							signal(xpppgingsem);
							continue; /* dropped */
						}
						
						mptr = ndptr->msgptr;
						id  = ft[mptr->fram_num].id;
							/* the page's owner */
						length -= MSGLEN;
						ptr = (char *)
						    tovaddr(mptr->page_num);
						cppgaddr = (char *)
						    physpgcopy(pcktdatap,
							       currpid, ptr,
							       id,
							       length,
							       FALSE);
						/* this is the vaddr in	*/
						/* copypage after vbcopy*/
					}else {
						curcppgaddr = cppgaddr +
						    ((fragno-1)*maxfdatalen);
						if (isintaddr(pcktdatap) &&
						    isintaddr(curcppgaddr))
						    fblkcopy((int *)curcppgaddr,
							     (int *)pcktdatap,
							     length);
		     				/* fast block copy routine */
						else
						    blkcopy(curcppgaddr,
							    pcktdatap, length);
					}
				}
				bitmap[fragno] = 1; /* received the frag */
				counter = counter + 1;
				
				if (counter == totalfrag) 
				    done = 1;
			}   /* normal receiving ends */
		}    /* else if ends */
		
		signal(xpppgingsem);
		
	}   /* while ends */
	return(OK);
}

/*****************************************************************************
  PrintPacket takes a wp packet and prints the content out for debugging 
  purposes.
  *****************************************************************************/
printpacket(temp)
     TPPacketType temp;
{
	int i;
	TPPacketType *pwp;
	
	pwp = &temp;
	switch (net2hl(pwp->PacketType)) {
	      case NSETE : kprintf("NSETE packet\n");
		break;
	      case NACK :  kprintf("Nack Packet\n");
		break;
	      case REPLNACK :  kprintf("ReplNack Packet\n");
		break;
	      default:kprintf("Illegal packet type\n");
	}
	kprintf("Mess No = %d\n",	net2hl(pwp->MesgNum));
	kprintf("Total Frag = %d\n",	net2hl(pwp->TotalFrags));
	kprintf("Frag Num = %d\n",	net2hl(pwp->FragNum));
	kprintf("HeadLength = %d\n",	net2hl(pwp->HeadLength));
	kprintf("FragSize = %d\n",	net2hl(pwp->FragSize));
	kprintf("MaxDataLength = %d\n",	net2hl(pwp->MaxDataLen));
	for (i=0;i<=32; i++)
	    kprintf("%x",pwp->Data[i]);
	kprintf("\n\n");
}

/*---------------------------------------------------------------------------
 * printbuflist - print the packets of a msg that have been received so far
 *---------------------------------------------------------------------------
 */
printbuflist(bufmesgno, counter, bitmap)
     int bufmesgno;
     int counter;
     int bitmap[MAXFRAGNO];
{
	int j;
	
	kprintf("----------------------------------------------\n");
	kprintf("Mesgno = %d\n",bufmesgno);
	kprintf("counter = %d\n",counter);
	for (j=0; j<MAXFRAGNO; j++)
	    kprintf("%d   ",bitmap[j]);
	kprintf("\n----------------------------------------------\n");
}
