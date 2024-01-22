/* ipreass.h */

/* Internet Protocol (IP)  reassembly support */

/* maximum number of network buffers available to fragmentation queues	*/
/* currently, 1/3 of total						*/
#define	IP_MAXFB	(NETBUFS/3)

/* maximum number of fragments maintained for each datagram		*/
/* currently, maximum datagram size divided by ethernet packet size	*/
#define IP_MAXNF	(1+(BPMAXB/EP_DLEN))

/* maximum number of datagrams to concurrently reassemble		*/
/* currently, function of max netbufs and max frags above		*/
#define IP_FQSIZE	(IP_MAXFB/IP_MAXNF)

/* maximum time that fragments of a datagram will be kept		*/
/* currently, 60 seconds as recommended in RFC 1122			*/
#define	IP_FTTL		60

/* ipf_state flags */

#define	IPFF_VALID	1	/* contents are valid			*/
#define	IPFF_BOGUS	2	/* drop frags that match		*/
#define	IPFF_FREE	3	/* this queue is free to be allocated	*/

struct	ipfq	{
	int	ipf_state;		/* VALID, FREE or BOGUS		*/
	int	ipf_id;			/* datagram id			*/
	int	ipf_ttl;		/* countdown to disposal	*/
	int	ipf_q;			/* the queue of fragments	*/
	IPaddr	ipf_src;		/* IP address of the source	*/
};

extern	int	ipfmutex;		/* mutex for ipfqt[]		*/
extern	struct	ipfq	ipfqt[];	/* IP frag queue table		*/
