/* kernel.sparc.h - SPARC specific kernel constants */

/* Typedef's */

#ifndef ASM
typedef	long	WORD;		/* maximum of (int, char *)		*/
typedef int	STATWORD[1];	/* saved machine status for		*/
				/* disable/restore by declaring it to 	*/
				/* be an array, the name provides an	*/
				/* address so forgotten &'s don't	*/
				/* become a problem			*/
#define	PStype	STATWORD	/* For old routines that still use PStype */
#endif

/* User and Kernel mode Process Status Register initialization constants */
#define	INITKMPS	0x0		/* initial process PS (K mode)	*/
#define	INITUMPS	0x0		/* initial process PS (User mode)*/


/* Initialization constants */

#define	INITSTK		0x4000		/* initial process stack - 16 k	*/
#define	INITPRIO	20		/* initial process priority	*/
#define	INITNAME	"main"		/* initial process name		*/
#define	INITARGS	1,0		/* initial count/arguments	*/
#define	INITRET		kernret 	/* INITRET=K process' return point*/
#define	UINITRET	userret 	/* UINITRET=U process' return point*/
#define	INITPS		0x0		/* init PS = interrupts off	*/
#define	INITREG		0		/* initial register contents	*/
#define	NULLSTK		0x1000		/* process 0 stack size		*/

#define	QUANTUM		10		/* clock ticks until preemption	*/

#define	MINSTK		4096		/* minimum process stk size */

#define MAXINT		0x7fffffff	
#define MININT		0x80000000



