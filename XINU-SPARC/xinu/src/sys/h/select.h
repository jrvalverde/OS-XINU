/* 
 * select.h - for select()
 * 
 * 		Dept. of Computer Science
 * 		Purdue University
 * Date:	Fri Mar 26 10:26:43 1993
 */

#define	SETSIZE     128

#define	BPM (sizeof(long) * 8)	/* bits per mask */

/*
 * figure out how many array entries we need to hold the set of device
 * descriptors for a select.  Make sure to round up if it's not a
 * power of 2.
 */
#define	arrayents(itemsize, items)(((items)+((itemsize)-1))/(itemsize))

typedef	struct dd_set {
	long mask[arrayents(BPM, SETSIZE)];
} dd_set;

#define	setbit(n, p)   ((p)->mask[(n)/BPM] |= (1 << ((n) % BPM)))
#define	clrbit(n, p)   ((p)->mask[(n)/BPM] &= ~(1 << ((n) % BPM)))
#define	isbitset(n, p) ((p)->mask[(n)/BPM] & (1 << ((n) % BPM)))
#define	zero_dd(p)     bzero((char *)(p), sizeof(*(p)))

/*
 * Unix compatibility stuff
 */

typedef dd_set fd_set;
#define FD_SET(n, p)   setbit(n, p)
#define FD_CLR(n, p)   clrbit(n, p)
#define FD_ISSET(n, p) isbitset(n, p)
#define FD_ZERO(p)     zero_dd(p)


/* control call definitions for select and non-blocking IO */
#define NOTIFYR         10001           /* set read notify for select   */
#define NOTIFYW         10002           /* set write notify for select  */
#define UNNOTIFYR       10003           /* retract read for select      */
#define UNNOTIFYW       10004           /* retrace write for select     */
#define SETNBIO         10005           /* set non-blocking I/O         */
#define UNSETNBIO       10006           /* turn off non-blocking I/O    */
#define IONREAD		10007		/* # of chars ready to read	*/
#define IONWRITE	10008		/* free space in write buffer	*/

