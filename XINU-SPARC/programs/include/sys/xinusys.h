/* xinusys.h - came from kernel.h -  definition of kernel constants */

#include	<sys/xinuconf.h>	/* hopefully this is temporary	*/

/* Symbolic constants used throughout Xinu */

#ifndef	ASM
#ifndef BOOL
#define BOOL	Bool
typedef	char		Bool;		/* Boolean type			*/
#endif
#endif	ASM

#define	FALSE		0		/* Boolean constants		*/
#define	TRUE		1
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
#define	MAGIC		0125252		/* unusual value for top of stk	*/

/* Universal return constants */

#define	OK		 1		/* returned when system	call ok	*/
#define	SYSERR		-1		/* returned when sys. call fails*/
#define	EOF		-2		/* returned for end-of-file	*/
#define	TIMEOUT		-3		/* returned  "  "  "  times out	*/
#define	INTRMSG		-4		/* keyboard "intr" key pressed	*/
					/*  (usu. defined as ^B)	*/

/* Miscellaneous utility inline functions */

#ifndef	ASM
#define	isodd(x)	(01&(int)(x))
#define	min(a,b)	((a) < (b) ? (a) : (b))
#endif ASM

