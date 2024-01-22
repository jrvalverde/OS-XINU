/* sleep.h */

#define	CVECTOR	((int *)0xc0)	/* location of clock interrupt vector	*/
#define	STRTCLK	0x51		/* value to load, start interval clock	*/
#define	CLKINTVL 100000		/* microseconds in one clock tick	*/
#define STRTUCLK 0x40		/* value to enable Microvax rt clock	*/

extern	int	clkruns;	/* 1 iff clock exists; 0 otherwise	*/
				/* Set at system startup.		*/
extern	int	clockq;		/* q index of sleeping process list	*/
extern	int	count10;	/* used to ignore 9 of 10 ticks		*/
extern	short	*sltop;		/* address of first key on clockq	*/
extern	int	slnempty;	/* 1 iff clockq is nonempty		*/

extern	int	defclk;		/* >0 iff clock interrupts are deferred	*/
extern	int	clkdiff;	/* number of clock ticks deferred	*/
extern	long	clktime;	/* time in seconds since 1/1/1970	*/
extern	int	clmutex;	/* mutual exclusion for time of day	*/
extern	int	tickctr;	/* count of 10ths of seconds 10-0	*/
extern	int	clkint();	/* clock interrupt handler		*/
