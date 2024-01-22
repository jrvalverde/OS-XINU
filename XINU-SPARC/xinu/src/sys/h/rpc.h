/* rpc.h - constants and structures for Remote Procedure Call, RPC */


#define RPCS_FREE 1		/* device is FREE			*/
#define RPCS_INUSE 2		/* device is IN USE			*/

#define RPCT_CLIENT 0x01	/* open as an RPC client		*/
#define RPCT_LSERVER 0x02	/* open as a listening RPC server	*/
#define RPCT_ASERVER 0x03	/* open as an active RPC server		*/

#define RPCT_RESPORT 0x10	/* use a reserved port number		*/

#define RPC_NOPORT	1	/* port number not yet assigned		*/

#define RPC_EVERYHOST	"*"	/* broadcast host name			*/


/* error codes returns from client RPC calls */
#define RPC_OK			1	/* no error			*/
#define RPC_ERROR_UNKNOWN	-10	/* unknown RPC error		*/
#define RPC_ERROR_UNREG		-11	/* program is not registered	*/
#define RPC_ERROR_TIMEOUT	-13	/* got no response		*/
#define RPC_ERROR_RPCVERS	-14	/* wrong RPC version		*/
#define RPC_ERROR_AUTHERROR	-15	/* couldn't authenticate	*/
#define RPC_ERROR_PROGUNAVAIL	-16	/* program not exported		*/
#define RPC_ERROR_GARBARGS	-17	/* can't decode params		*/
#define RPC_ERROR_PROGMISMATCH	-18	/* version not available	*/
#define RPC_ERROR_PROCUNAVAIL	-19	/* procedure not available	*/
#define RPC_ERROR_BUFERROR	-101	/* xdr buffering error		*/
#define RPC_ERROR_FORMAT	-102	/* FORMAT control call error	*/


/* additional error codes for server replies */
#define RPC_ERROR_AUTH_BADCRED		-20	/* bad credentials (seal broken)	*/
#define RPC_ERROR_AUTH_REJECTEDCRED	-21	/* client must begin new session	*/
#define RPC_ERROR_AUTH_BADVERF		-22	/* bad verifier (seal broken)		*/
#define RPC_ERROR_AUTH_REJECTEDVERF	-23	/* verifier expired or replayed		*/ 
#define RPC_ERROR_AUTH_TOOWEAK		-24	/* rejected for security reasons	*/


    
#define RPC_ERROR_GET_NAME(errno)	\
    (errno == RPC_OK)			?"no error":			\
    (errno == RPC_ERROR_UNREG)		?"program not registered":	\
    (errno == RPC_ERROR_TIMEOUT)	?"no response within timeout":	\
    (errno == RPC_ERROR_RPCVERS)	?"RPC version not supported":	\
    (errno == RPC_ERROR_AUTHERROR)	?"permission denied":		\
    (errno == RPC_ERROR_PROGUNAVAIL)	?"program not exported":	\
    (errno == RPC_ERROR_GARBARGS)	?"parameters unparseable ":	\
    (errno == RPC_ERROR_PROGMISMATCH)	?"program version unsupported":	\
    (errno == RPC_ERROR_UNKNOWN)	?"misc RPC protocol error":	\
    (errno == RPC_ERROR_PROCUNAVAIL)	?"procedure not available":	\
    "invalid RPC error code"


/* rpc device control block */
struct rpcblk {
	unsigned	rpc_dnum;	/* device number		*/
	unsigned	rpc_state;	/* RPCS_FREE or RPCS_INUSE	*/
	unsigned	rpc_mutex;	/* mutual exclusion semaphore	*/
	unsigned	rpc_type;	/* RPCT_((CLIENT)|([LA]SERVER))	*/
	int		rpc_isbrc;	/* is this a broadcast??	*/
	unsigned	rpc_program;	/* program number		*/
	unsigned	rpc_version;	/* max. program version number	*/
	unsigned	rpc_minversion;	/* min. supported prog. version	*/
	unsigned	rpc_procedure;	/* procedure number		*/
	unsigned	rpc_sequence;	/* sequence number		*/
	unsigned	rpc_status;	/* return value for last call	*/

	/*==============================================*/
	/*    for UDP communication			*/
	/*==============================================*/
	unsigned	rpc_lport;	/* local protocol port		*/
	unsigned	rpc_rport;	/* remote protocol port to use	*/
	int		rpc_udpdev;	/* UDP device we're using	*/
	unsigned	rpc_protocol;	/* protocol to use (UDP)	*/

	/*==============================================*/
	/*    reliability constraints			*/
	/*==============================================*/
	unsigned	rpc_nextTO;	/* timeout for subsequent resps	*/
	unsigned	rpc_RTO;	/* round trip timeout	 	*/
	unsigned	rpc_SRTT;	/* smoothed round trip time est	*/
	unsigned	rpc_timeout;	/* when sent	 		*/
	unsigned	rpc_timein;	/* when received 		*/
	unsigned	rpc_trans;	/* transmission count		*/
	unsigned	rpc_maxtrans;	/* max number of transmissions	*/

	/*==============================================*/
	/*    OUTGOING VARIABLES (for extroverts only)	*/
	/*==============================================*/
	char	 	*rpc_reqxbuf;	/* XDR request buffer		*/
	unsigned	rpc_reqxbufsize;/* size of xdrbuf		*/
	unsigned	rpc_reqxix;	/* index to next xdrbuf loc.	*/
	unsigned	rpc_reqxlen;	/* number valid bytes in xdrbuf	*/
	IPaddr		rpc_hostIP;	/* foreign machine's IP address	*/
	unsigned	rpc_reqauth;	/* AUTH method to use		*/
	int		rpc_requid;	/* uid to use (AUTHUNIX)	*/
	int		rpc_reqgid;	/* gid to use (AUTHUNIX)	*/

	/*==============================================*/
	/*    INCOMING VARIABLES                  	*/
	/*==============================================*/
	struct ep	*rpc_resppep;	/* pep for incoming packet	*/
	char	 	*rpc_respxbuf;	/* XDR response buffer		*/
	unsigned	rpc_respxbufsize;/* size of xdrbuf		*/
	unsigned	rpc_respxix;	/* index to next xdrbuf loc.	*/
	unsigned	rpc_respxlen;	/* number valid bytes in xdrbuf	*/
	IPaddr		rpc_resphostIP;	/* responding host's IP address	*/
	unsigned 	rpc_respport;	/* responding host's port	*/
	unsigned	rpc_respauth;	/* AUTH method used by responder*/
	int		rpc_respuid;	/* uid of responder (AUTHUNIX)	*/
	int		rpc_respgid;	/* gid of responder (AUTHUNIX)	*/
	unsigned	rpc_respvers;   /* version of the call msg.	*/

	/*==============================================*/
	/*    FOR STATISTICS	                  	*/
	/*==============================================*/
	unsigned	rpc_sends;	/* # msgs sent			*/
	unsigned	rpc_recvs;	/* # msgs received		*/
	unsigned	rpc_resends;	/* # msgs resent (timeout)	*/
	unsigned	rpc_badrecvs;	/* # msgs received out of order	*/
};


/* control call definitions */
    /* marshal INTO a message */
#define XSET_INT	10
#define XSET_UINT	11
#define XSET_ENUM	12
#define XSET_BOOL	13
#define XSET_FLOAT	14
#define XSET_DOUBLE	15
#define XSET_STRING	16
#define XSET_FOPAQ	17
#define XSET_VOPAQ	18
#define XSET_FORMAT	19
    /* marshal OUT OF a message */
#define XGET_INT	30
#define XGET_UINT	31
#define XGET_ENUM	32
#define XGET_BOOL	33
#define XGET_FLOAT	34
#define XGET_DOUBLE	35
#define XGET_STRING	36
#define XGET_FOPAQ	37
#define XGET_VOPAQ	38
#define XGET_FORMAT	39
    /* misc controls */
#define XDR_REWIND	50
#define XDR_ERASE	51
#define RPC_PROGVERS	52
#define RPC_SETPROC	53
#define RPC_SETRTO	54
#define RPC_SETMAXTRANS	57
#define RPC_GETMAXTRANS	58
#define RPC_GETRTO	59
#define RPC_CALL	60
#define RPC_NEXTRESP	61
#define RPC_GETHOST	62
#define RPC_GETPORT	63
#define RPC_LISTEN	64
#define RPC_SETNEXTTO	65
#define RPC_SETAUTH	66
#define RPC_GETRAUTH	67
#define RPC_GETRUID	68
#define RPC_GETRGID	69
#define RPC_GETSTATUS	70
#define RPC_REPLY	71
#define RPC_GETXID	72
#define RPC_GETRVERS	73
#define RPC_GETPROC	74


/* configuration parameters */
#define RPC_REQBUF_SIZE BPMAXB


/* Only supported RPC version */
#define RPCVERS          2

#define RPC_PMAP_PORT	111


#define RPC_MAX_AUTH_SIZE 400
#define RPC_MAX_MACHINENAME_LEN 256

/* definitions of header fields */
#define RPC_MSGTYPE_CALL 		0
#define RPC_MSGTYPE_REPLY		1
#define RPC_MSGTYPE_NAMES		{"CALL","REPLY",0}

#define RPC_REPLYSTAT_ACCEPTED	 	0
#define RPC_REPLYSTAT_DENIED	 	1
#define RPC_REPLYSTAT_NAMES	 	{"ACCEPTED","DENIED",0}

#define RPC_ACCEPTSTAT_SUCCESS	 	0 /* RPC executed successfully	     */
#define RPC_ACCEPTSTAT_PROGUNAVAIL 	1 /* remote hasn't exported program  */
#define RPC_ACCEPTSTAT_PROGMISMATCH 	2 /* remote can't support version #  */
#define RPC_ACCEPTSTAT_PROCUNAVAIL 	3 /* program can't support procedure */
#define RPC_ACCEPTSTAT_GARBAGEARGS	4 /* procedure can't decode params   */
#define RPC_ACCEPTSTAT_NAMES		{"SUCCESS", "PROG_UNAVAIL", \
					     "PROG_MISMATCH", "PROC_UNAVAIL", \
						 "GARBAGE_ARGS",0}

#define RPC_REJECTSTAT_RPCMISMATCH 	0 /* RPC version number != 2         */
#define RPC_REJECTSTAT_AUTHERROR 	1 /* remote can't authenticate caller*/
#define RPC_REJECTSTAT_NAMES		{"RPC_MISMATCH", "AUTH_ERROR",0}

#define RPC_AUTHSTAT_BADCRED		1 /* bad credentials (seal broken) */
#define RPC_AUTHSTAT_REJECTEDCRED	2 /* client must begin new session */
#define RPC_AUTHSTAT_BADVERF		3 /* bad verifier (seal broken)    */
#define RPC_AUTHSTAT_REJECTEDVERF	4 /* verifier expired or replayed  */
#define RPC_AUTHSTAT_TOOWEAK		5 /* rejected for security reasons */
#define RPC_AUTHSTAT_NAMES		{ "==UNDEFINED==", "AUTH_BADCRED", \
					      "AUTH_REJECTEDCRED", \
						  "AUTH_BADVERF", \
						      "AUTH_REJECTEDVERF", \
							  "AUTH_TOOWEAK",0}

#define RPC_AUTHFLAVOR_NULL	 	0
#define RPC_AUTHFLAVOR_UNIX	 	1
#define RPC_AUTHFLAVOR_SHORT	 	2
#define RPC_AUTHFLAVOR_DES	 	3
#define RPC_AUTHFLAVOR_NAMES		{"NULL","UNIX","SHORT","DES",0}


extern struct	rpcblk	rpctab[];
extern int	rpcmutex;

/* connection defaults in second */
#define RPC_DEFAULT_RTO		5	/* 5 seconds 	*/
#define RPC_DEFAULT_MAXRTO	60	/* 60 seconds 	*/
#define RPC_DEFAULT_NEXTTO	5	/* 5 seconds	*/
#define RPC_DEFAULT_MAXTRANS	3
