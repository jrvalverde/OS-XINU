/* sleep.h */

#define	C14VECTOR	0x0078	/* Clock vector for autovector 14	*/


extern	int	clkruns;	/* 1 iff clock exists; 0 otherwise	*/
				/* Set at system startup.		*/
extern	int	clockq;		/* q index of sleeping process list	*/
extern	int	count6;		/* used to ignore 5 of 6 interrupts	*/
extern	int	count10;	/* used to ignore 9 of 10 ticks		*/
extern	unsigned long clktime;	/* current time in secs since 1/1/70	*/
extern	int	clmutex;	/* mutual exclusion sem. for clock	*/
extern	int	*sltop;		/* address of first key on clockq	*/
extern	int	slnempty;	/* 1 iff clockq is nonempty		*/

extern	int	defclk;		/* >0 iff clock interrupts are deferred	*/
extern	int	clkdiff;	/* number of clock clicks deferred	*/
extern	int	clkint();	/* clock interrupt handler		*/

/* the following is used by clkint.c */
#define USECS_PER_SEC	1000000
#define TICKS_PER_TENTH	((USECS_PER_SEC/10)/usecs_per_intr)

extern int count_per_tenth;
extern int usecs_per_intr;	/* interrupt occurs every usecs_per_int	*/
				/* microseconds	- set in clkinit()	*/
				/* Must be a factor of USECS_PER_SEC	*/
