/* initialize.c - nulluser, sysinit */

#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <q.h>
#include <errno.h>
#include <stdio.h>
#include <sys/time.h>
#undef NULL
#include <sys/param.h>
#include <sys/socket.h>

extern	int	_ccc__userproc();	/* address of user's main prog	*/

/* Declarations of major kernel variables */

struct	pentry	proctab[NPROC]; /* process table			*/
int	nextproc;		/* next process slot to use in create	*/
struct	sentry	semaph[NSEM];	/* semaphore table			*/
int	nextsem;		/* next semaphore slot to use in screate*/
struct	qent	q[NQENT];	/* q table (see queue.c)		*/
int	nextqueue;		/* next slot in q structure to use	*/
struct	fd_ent	*fd_tab;	/* fd table for pre_create info		*/
int	ConC_SYS_PIPE;		/* fd for ConC side of I/O pipe		*/
int	Serv_SYS_PIPE;		/* fd for io_server side of I/O pipe	*/
int	SYS_PIPE;		/* fd for current side of I/O pipe	*/
int	server_pid;		/* pid of server (to kill when xdone)	*/
int	is_serv_init;		/* is server initialized ??		*/
char	cwd[MAXPATHLEN];	/* current working directory		*/

/* active system status */

int	numproc;		/* number of live user processes	*/
int	currpid;		/* id of currently running process	*/
int	bpmark = FALSE;		/* marker for buffer pools		*/
int	ptmark = FALSE;		/* marker for ports			*/

/* real-time clock variables and sleeping process queue pointers	*/

struct itimerval vtival, vtioval;/* virtual time clock structure	*/
struct itimerval rtival, rtioval;/* real    time clock structure	*/
int     slnempty;		/* FALSE if the sleep queue is empty	*/
int     clockq;			/* head of queue of sleeping processes  */
int	squantum =  S_TIME_QUANT;/* sec time quantum			*/
int	usquantum = U_TIME_QUANT;/* usec time quantum			*/
int	rdyhead,rdytail;	/* head/tail of ready list (q indexes)	*/
int	sem_sys_write;		/* sys_write semaphore			*/
int	sem_sys_gen;		/* mutex sem for gen routines		*/
int	sem_sys_malloc;		/* mutex sem for malloc routines	*/
int	sem_sys_dir;		/* mutex sem for dir routines		*/
int	sem_sys_net;		/* mutex sem for net routines		*/
int	sem_sys_compat;		/* mutex sem for compat routines	*/


/************************************************************************/
/***				NOTE:				      ***/
/***								      ***/
/***   This is where the system begins after the C environment has    ***/
/***   been established.  Interrupts are initially DISABLED, and      ***/
/***   must eventually be enabled explicitly.  This routine turns     ***/
/***   itself into the null process after initialization.  Because    ***/
/***   the null process must always remain ready to run, it cannot    ***/
/***   execute code that might cause it to be suspended, wait for a   ***/
/***   semaphore, or put to sleep, or exit.  In particular, it must   ***/
/***   not do I/O unless it uses kprintf for disabled, direct output. ***/
/***								      ***/
/************************************************************************/

main(argc,argv,envp)
int argc;
char **argv, **envp;
{
    nulluser(argc,argv,envp);
}

/*------------------------------------------------------------------------
 *  nulluser  -- initialize system and become the null process (id==0)
 *------------------------------------------------------------------------
 */
LOCAL
nulluser(argc,argv,envp)		/* babysit CPU when no one home */
int argc;
char **argv, **envp;
{
	long	ps;

	disable(ps);

	sysinit();			/* initialize first part of Xinu */

	enable();			/* enable interrupts	  	 */

	/* start a process executing the user's main program	  	 */

	resume(
	  create(_ccc__userproc,INITSTK,INITPRIO,INITNAME,3,argc,argv,envp)
	);

	while (TRUE) {			/* run forever without actually */
		xpause();		/*  executing instructions	*/
	}
}

/*------------------------------------------------------------------------
 *  sysinit  --  initialize all Xinu data structures and devices
 *------------------------------------------------------------------------
 */
LOCAL	sysinit()
{
	int	i,j,sv[2];
	struct	pentry	*pptr;
	struct	sentry	*sptr;

	numproc = 0;			/* initialize system variables */
	nextproc = NPROC-1;
	nextsem = NSEM-1;
	nextqueue = NPROC;		/* q[0..NPROC-1] are processes */
	is_serv_init = 0;		/* IO server not initialized   */

	for (i=0 ; i<NPROC ; i++)	/* initialize process table */
		proctab[i].pstate = PRFREE;

	pptr = &proctab[NULLPROC];	/* initialize null process entry */
	pptr->pstate = PRCURR;
	for (j=0; j<7; j++)
		pptr->pname[j] = "prnull"[j];
	pptr->plimit = DONTCARE;
	pptr->pbase = DONTCARE;
	pptr->paddr = (long) nulluser;
	pptr->pargs = 0;
	pptr->tdir  = NULL;
	currpid = NULLPROC;

	for (i=0 ; i<NSEM ; i++) {		/* initialize semaphores */
		(sptr = &semaph[i])->sstate = SFREE;
		sptr->sqtail = 1 + (sptr->sqhead = newqueue());
	}

	sem_sys_write = screate(1);	   /* system exclusive write sem      */
	sem_sys_gen   = screate(1);	   /* system exclusive gen routine sem*/
	sem_sys_malloc= screate(1);	   /* system exclusive malloc rout sem*/
	sem_sys_dir   = screate(1);	   /* system exclusive dir routine sem*/
	sem_sys_net   = screate(1);	   /* system exclusive net routine sem*/
	sem_sys_compat= screate(1);	   /* sys excl compat routine sem     */

	fd_tab	   = (struct fd_ent *)malloc(sizeof(struct fd_ent)*FD_SETSIZE);

	getwd(cwd);

	fd_tab[STDIN].pfd_head	= FD_NULL;
	fd_tab[STDIN].pfd_tail	= FD_NULL;
	fd_tab[STDIN].sem	= screate(1);

	fd_tab[STDOUT].pfd_head	= FD_NULL;
	fd_tab[STDOUT].pfd_tail	= FD_NULL;
	fd_tab[STDOUT].sem	= screate(1);

	fd_tab[STDERR].pfd_head	= FD_NULL;
	fd_tab[STDERR].pfd_tail	= FD_NULL;
	fd_tab[STDERR].sem	= screate(1);

	for ( i = STDERR+1 ; i < FD_SETSIZE ; i++) {
	    fd_tab[i].pfd_head	= FD_NULL;
	    fd_tab[i].pfd_tail	= FD_NULL;
	    fd_tab[i].sem	= screate(1);
	    }

	sys_socketpair(AF_UNIX,SOCK_STREAM,0,sv);     /* create io_serv pipe */
	ConC_SYS_PIPE	= sv[0];		      /* 1 fd each for ConC  */
	Serv_SYS_PIPE	= sv[1];		      /* io_server	     */
	SYS_PIPE	= ConC_SYS_PIPE;

	rdytail = 1 + (rdyhead=newqueue());/* initialize ready list	      */
	clockq	= newqueue();		   /* initialize clock queue	      */

	sys_interinit();	/* initialize all but quantum clock interrupts*/

	return(OK);
}
