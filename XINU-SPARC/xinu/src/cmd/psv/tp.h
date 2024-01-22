
/* 
 * tp.h - The NAFP protocol header file (NAFP used to be called TP)
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Jul 27 17:05:50 1990
 *
 * Copyright (c) 1990 Jim Griffioen
 */


/*===================================================================*/
/*                Function Success Return Code                       */
/*===================================================================*/
#define OK            0
#define SYSERR        -1

/*===================================================================*/
/*                     Packet Constants                              */
/*===================================================================*/
#define HEADLENGTH    (sizeof(TPPacketType)-FRAGDATASIZE)
#define FRAGDATASIZE  1380          /* The size of current fragments */

#define FRAGSIZE      (sizeof(TPPacketType))
		                    /* frag data plus wp header size */
#define MAXFRAGNO     10              /* Max number of frag possible */

#define NSETE         1            /* Non Standard End to End Packet */
#define NACK          2            /* Negative Acknowldgement Packet */
#define REPLNACK      3                   /* Reply to a Nack Request */
#define MAX_PER_GTE   10		/* == MAX_PER_PAGE in psv.h */
/*===================================================================*/
/*                      Packet Structure                             */
/*===================================================================*/
typedef struct {
   int  PacketType;                          /* Either NSETTE or NACK */
   int  MesgNum;                                    /* Message Number */
   int  TotalFrags;                      /* Total Number of Frgaments */
   int FragNum;                                   /* Fragment Number */
   int HeadLength;                         /* Transport Header Length*/
   int FragSize;                               /* Size of a fragment */
   int MaxDataLen;
   char Data[FRAGDATASIZE];                           /* Actual Data */
}TPPacketType;

/*===================================================================*/
/*                 Client side buffer structure                      */
/*===================================================================*/
typedef char Fragtype[FRAGDATASIZE];
typedef Fragtype TPbuffType[MAXFRAGNO]; /* buffer for assembly client*/

/*===================================================================*/
/*                 Server side buffer constants                      */
/*===================================================================*/
#define UNUSED        -2                      /* Flag for unsed buff */
#define MAXTPREADLIST  10               /* Num of buffer for assembly */

#define		isintaddr(x)	!((unsigned)(x) % sizeof(int))
#define		ismultint(x)	!(x % sizeof(int))

/*===================================================================*/
/*                 Server side buffer structure                      */
/*===================================================================*/
typedef struct {
    int    msgno;                  /* Messgage number in this buffer */
    struct sockaddr_in from;       /* host that sent this message */
    int    curfrag;                /* current fragment index */
    int    totalfrag;              /* total number of frag expected */
    char   dblkavail;	 	   /* 1->data block(s) available  */
    char   dblknum;		   /* number of d-block allocated  */
    struct d_block  *datap[MAX_PER_GTE];	/* d-blk pointers  */
    char   cur_dblk;		   /* current d_blk index */ 
    int    offset;		   /* copied data offset in cur d_blk */
    char   bitmap[MAXFRAGNO];	   /* fragment bit map */
    struct message   *msgp;	   /* points to the message received */
} TPReadListType;

struct tmessage {			/* temporary message list node	*/
	TPReadListType  *tprdlstptr;	/* TPReadList Node pointer	*/
	struct tmessage *nextmsg;	/* next pointer			*/
};

extern TPReadListType TPReadList[MAXTPREADLIST];

#ifdef USE_XLARGE_PACKET
extern char  wtemp[], rtemp[];
#else
extern TPPacketType wtemp, rtemp;
#endif
 
/*===================================================================*/
/*        Socket to listen on. Init by the Server                    */
/*===================================================================*/
int SOCK_FD;                /* Socket File Descriptor */

/*===================================================================*/
/*        Extern Procedure contained                                 */
/*===================================================================*/
extern TPCread();
extern TPCwrite();
extern TPSread();
extern TPSwrite();
