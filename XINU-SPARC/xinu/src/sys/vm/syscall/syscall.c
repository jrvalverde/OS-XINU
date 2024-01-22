/* syscall.c */

#include	<conf.h>
#include	<kernel.h>
#include	<mem.h>
#include	<vmem.h>
#include 	<syscall.h>
#include 	<user_traps.h>

/*#define DEBUG_DSPTCHER*/

/* system call dispatch array defined in syscall.h */
int (*syscalf[SYSCALNUM])();   /* an array of pointers to functions */

/*-------------------------------------------------------------------------
 * dsptcher - 
 *-------------------------------------------------------------------------
 */
dsptcher()		/* entered from system call vector: scallvec */
{
    int *rsa, ret;

#ifdef DEBUG_DSPTCHER
    kprintf("Just got a System Call\n");
#endif
    rsa = (int *)(vmaddrsp.kernstk + 8);

#ifdef DEBUG_DSPTCHER
    kprintf("calculated rsa = 0x%08x\n", rsa);
    kprintf("Sys Call number = %d\n", *rsa);
    kprintf("Calling procedure = 0x%08x\n", syscalf[*rsa]);
#endif
    ret = (int)(*syscalf[*rsa])( *(rsa+1), *(rsa+2), *(rsa+3), *(rsa+4),
				*(rsa+5), *(rsa+6), *(rsa+7),*(rsa+8) );
#ifdef DEBUG_DSPTCHER
    kprintf("Done with system call, retval = 0x%08x\n", ret);
    kprintf("Placing value at rsa = 0x%08x\n", rsa);
#endif
    *rsa = ret;			/* return value put in RSA */
}

/*----------------------------------------------------------------------
 * syscalinit - system call initialization
 *
 *---------------------------------------------------------------------- */
syscalinit()
{
    /* set the system call trap vector */
    set_evec(((U_TRAP_SYSCALL+TICC_OFFSET)*4), dsptcher);

    syscalf[SYSC_ACCESS]	=	access;
    syscalf[SYSC_ASCREATE]	=	ascreate;

    syscalf[SYSC_CHPRIO]	=	chprio;
    syscalf[SYSC_CLOSE]		=	close;
    syscalf[SYSC_CONTROL]	=	control;
    syscalf[SYSC_CREATE]	=	create;

    syscalf[SYSC_FREEHEAP]	=	freeheap;

    syscalf[SYSC_GETADDR]	=	getaddr;
    syscalf[SYSC_GETC]		=	getc;
    syscalf[SYSC_GETHEAP]	=	getheap;
    syscalf[SYSC_GETNET]	=	getnet;
    syscalf[SYSC_GETPID]	=	getpid;
    syscalf[SYSC_GETPRIO]	=	getprio;
    syscalf[SYSC_GETREALTIM]	=	getrealtim;
    syscalf[SYSC_GETUTIM]	=	getutim;
    syscalf[SYSC_GETASID]	=	getasid;

    syscalf[SYSC_IP2NAME]	=	ip2name;
    syscalf[SYSC_KILL]		=	kill;
    syscalf[SYSC_LOGIN]		=	login;
    syscalf[SYSC_MOUNT]		=	mount;
    syscalf[SYSC_OPEN]		=	sysopen;

    syscalf[SYSC_PANIC]		=	panic;
    syscalf[SYSC_PCOUNT]	=	pcount;
    syscalf[SYSC_PCREATE]	=	pcreate;
    syscalf[SYSC_PDELETE]	=	pdelete;
    syscalf[SYSC_PRECEIVE]	=	preceive;
    syscalf[SYSC_PRESET]	=	preset;
    syscalf[SYSC_PROCREATE]	=	procreate;
    syscalf[SYSC_PSEND]		=	psend;
    syscalf[SYSC_PUTC]		=	putc;

    syscalf[SYSC_READ]		=	read;
    syscalf[SYSC_RECEIVE]	=	receive;
    syscalf[SYSC_RECVCLR]	=	recvclr;
    syscalf[SYSC_RECVTIM]	=	recvtim;
    syscalf[SYSC_REMOVE]	=	remove;
    syscalf[SYSC_RENAME]	=	rename;
    syscalf[SYSC_RESUME]	=	resume;

    syscalf[SYSC_SCOUNT]	=	scount;
    syscalf[SYSC_SCREATE]	=	screate;
    syscalf[SYSC_SDELETE]	=	sdelete;
    syscalf[SYSC_SEEK]		=	seek;
    syscalf[SYSC_SEND]		=	send;
    syscalf[SYSC_SENDF]		=	sendf;
    syscalf[SYSC_SETDEV]	=	setdev;
    syscalf[SYSC_SETNOK]	=	setnok;
    syscalf[SYSC_SHELL]		=	shell;
    syscalf[SYSC_SIGNAL]	=	signal;
    syscalf[SYSC_SIGNALN]	=	signaln;
    syscalf[SYSC_SLEEP]		=	sleep;
    syscalf[SYSC_SLEEP10]	=	sleep10;
    syscalf[SYSC_SRESET]	=	sreset;
    syscalf[SYSC_SUSPEND]	=	suspend;

    syscalf[SYSC_TCREATE]	=	tcreate;
    syscalf[SYSC_UNMOUNT]	=	unmount;
    syscalf[SYSC_UNSLEEP]	=	unsleep;
    syscalf[SYSC_USERRET]	=	userret;
    syscalf[SYSC_WAIT]		=	wait;
    syscalf[SYSC_WRITE]		=	write;

    syscalf[SYSC_NAME2IP]	=	name2ip;
    syscalf[SYSC_XINUEXECV]	=	xinu_execv;
    syscalf[SYSC_DVLOOKUP]	=	dvlookup;

    syscalf[SYSC_NOP]		=	null_system_call;
    syscalf[SYSC_STRLEN]	=	strlen;
    syscalf[SYSC_KPRINTF]	=	kprintf;
}


null_system_call()
{
	/* do nothing and return */
}

