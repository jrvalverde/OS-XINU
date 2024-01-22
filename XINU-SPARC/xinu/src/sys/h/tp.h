
/*---------------------------------------------------------------------------
 * tp.h - transport protocol header info
 *---------------------------------------------------------------------------
 */


/*---------------------------------------------------------------------------
 *                     Packet Constants                              
 *---------------------------------------------------------------------------
 */
#define HEADLENGTH    28
#define FRAGDATASIZE  1380          /* The size of current fragments */

#define FRAGSIZE      (sizeof(TPPacketType))
		                    /* frag data plus wp header size */
#define MAXFRAGNO     10              /* Max number of frag possible */

#define NSETE         1            /* Non Standard End to End Packet */
#define NACK          2            /* Negative Acknowldgement Packet */
#define REPLNACK      3                   /* Reply to a Nack Request */
#define UNUSED	      -2		/* unused message number     */

/*---------------------------------------------------------------------------
 *                      Packet Structure                             
 *---------------------------------------------------------------------------
 */
typedef struct {
   int   PacketType;                          /* Either NSETTE or NACK */
   int   MesgNum;                                    /* Message Number */
   int   TotalFrags;                      /* Total Number of Frgaments */
   int   FragNum;                                   /* Fragment Number */
   int   HeadLength;                         /* Transport Header Length*/
   int   FragSize;                               /* Size of a fragment */
   int   MaxDataLen;
   char Data[FRAGDATASIZE];                           /* Actual Data */
}TPPacketType;

/*---------------------------------------------------------------------------
 *                 Client side buffer structure                     
 *---------------------------------------------------------------------------
 */
typedef char Fragtype[FRAGDATASIZE];
typedef Fragtype TPbuffType[MAXFRAGNO]; /* buffer for assembly client*/

 
/*---------------------------------------------------------------------------
 *		external variable
 *---------------------------------------------------------------------------
 */
extern int pgsdev;			/* page server device num    */

extern TPPacketType *wpwp, *rpwp;
