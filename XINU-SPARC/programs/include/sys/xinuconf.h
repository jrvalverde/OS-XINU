/* xinuconf.h - xinu devices */

/****************/
/*  IMPORTANT  */
/***************/
/* Important address -  really should get this addr from a sys call	*/
/* 			but we don't have a sys call for this yet	*/
/* check the following address with the vm/syscall/syscall.c file       */
/* If you change the "rsa" value in vm/syscall/syscall.c file,		*/
/* then you need to change "RSASTRTADDR", and rebuild the unixsyscall   */
/* library (unixsyscall.a).	  					*/

#define	RSASTRTADDR	0x0fcff000		/* vmaddrsp.kernstk+8  	*/

/* Device name definitions */
#define	CONSOLE     "CONSOLE"			/* type tty      */
#define	ETHER       "ETHER"			/* type eth      */
#define	UDP	    "UDP"			/* type dgm      */
#define	TCP	    "TCP"			/* type tcpm     */
#define	RFILSYS     "RFILSYS"			/* type rfm      */
#define	NAMESPACE   "NAMESPACE"			/* type nam      */

