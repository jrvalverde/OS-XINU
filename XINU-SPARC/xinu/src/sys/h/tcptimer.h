/* tcptimer.h */

/* A timer delta list entry */

struct	tqent {
	int	tq_timeleft;		/* time to expire (1/100 secs)	*/
	long	tq_time;		/* time this entry was queued	*/
	int	tq_port;		/* port to send the event	*/
	int	tq_portlen;		/* length of "tq_port"		*/
	int	tq_msg;			/* data to send when expired	*/
	struct	tqent	*tq_next;	/* next in the list		*/
};
/* timer process declarations and definitions */

extern	int		tcptimer();
#define	TMSTK		4096		/* stack size for fast timer	*/
#define	TMPRI		100		/* timer process priority	*/
#define	TMNAM		"tcptimer"	/* name of fast timer process	*/
#define	TMARGC		0		/* count of args to TCP timer 	*/

#define ctr100 realtime

extern	unsigned long ctr100;		/* 1/100th of a second clock	*/

extern	int	tqmutex;
extern	int	tqpid;
extern	struct	tqent	*tqhead;
