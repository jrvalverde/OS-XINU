/* kernel.h - isodd, min */

#include <types.h>

/* Symbolic constants used throughout Xinu */

#ifndef	ASM
#ifndef BOOLDEF
#define BOOLDEF
typedef	int		Bool;		/* Boolean type			*/
#endif  BOOLDEF
#endif	ASM

#define	FALSE		0		/* Boolean constants		*/
#define	TRUE		1
#define	EMPTY		(-1)		/* an illegal gpq		*/
#define	NULL		(char *)0	/* Null pointer for linked lists*/
#define	NULLCH		'\0'		/* The null character		*/
#define	NULLSTR		""		/* Pointer to empty string	*/
#define	SYSCALL		int		/* System call declaration	*/
#define	LOCAL		static		/* Local procedure declaration	*/
#define	COMMAND		int		/* Shell command declaration	*/
#define	BUILTIN		int		/* Shell builtin " "		*/
#define	INTPROC		int		/* Interrupt procedure  "	*/
#define	PROCESS		int		/* Process declaration		*/
#define	RESCHYES	1		/* tell	ready to reschedule	*/
#define	RESCHNO		0		/* tell	ready not to resch.	*/
#define	LOWBYTE		0377		/* mask for low-order 8 bits	*/
#define	LOW6		077		/* mask for low-order 6 bits	*/
#define	LOW16		0177777		/* mask for low-order 16 bits	*/

/* stuff for select and non-blocking IO */
#define NOTIFYR         10001           /* set read notify for select   */
#define NOTIFYW         10002           /* set write notify for select  */
#define UNNOTIFYR       10003           /* retract read for select      */
#define UNNOTIFYW       10004           /* retrace write for select     */
#define SETNBIO         10005           /* set non-blocking I/O         */
#define UNSETNBIO       10006           /* turn off non-blocking I/O    */
#define IONREAD		10007		/* # of chars ready to read	*/
#define IONWRITE	10008		/* free space in write buffer	*/

/* Universal return constants */

#define	OK		 1		/* returned when system	call ok	*/
#define	SYSERR		-1		/* returned when sys. call fails*/
#define	EOF		-2		/* returned for end-of-file	*/
#define	TIMEOUT		-3		/* returned  "  "  "  times out	*/
#define	INTRMSG		-4		/* keyboard "intr" key pressed	*/
					/*  (usu. defined as ^B)	*/
#define IOREADY		-5		/* IO ready in select call      */
#define BLOCKERR	-6		/* no data in NBIO call         */

/* Miscellaneous utility inline functions */

#ifndef	ASM
#define	isodd(x)	(01&(int)(x))
#define	min(a,b)	((a) < (b) ? (a) : (b))
#define	max(a,b)	((a) > (b) ? (a) : (b))
#endif	ASM

#ifndef ASM
extern	int	rdyhead, rdytail;
extern	int	preempt;
#endif	ASM

/* machine dependent kernel constants */
#include <kernel.sparc.h>
