/* create.c - create, tstart, do_create, newpid */

#include <kernel.h>
#include <proc.h>
#include <mem.h>
#include <errno.h>

/*------------------------------------------------------------------------
 *  create  -  create a task to start running a procedure.
 *	       Pass addr of args to do_create.  Done so tstart will work.
 *------------------------------------------------------------------------
 */
SYSCALL create(procaddr,ssize,priority,name,nargs,args)
	int	*procaddr, ssize, priority, nargs, args;
	char	*name;
{
	return(do_create(procaddr,ssize,priority,name,nargs,&args));
}

/*------------------------------------------------------------------------
 *  tstart  -  create and resume a task to start running a procedure
 *	       Stack size is that of INITSTK,
 *	       priority is that of "parent",
 *	       name is that of START_NAME.
 *------------------------------------------------------------------------
 */
SYSCALL tstart(procaddr,nargs,args)
	int	*procaddr, nargs, args;
{
    int pid;
	resume(
	    pid =
	    do_create(procaddr,INITSTK,getprio(gettid()),START_NAME,nargs,&args)
	);

	return(pid);
}

/*------------------------------------------------------------------------
 *  do_create  -  actual creation of a task to start running a procedure
 *------------------------------------------------------------------------
 */
LOCAL do_create(procaddr,ssize,priority,name,nargs,pargs)
	int	*procaddr;		/* procedure address		*/
	int	ssize;			/* stack size in words		*/
	int	priority;		/* process priority > 0		*/
	char	*name;			/* name (for debugging)		*/
	int	nargs;			/* number of args that follow	*/
	int	*pargs;			/* arguments (treated like an	*/
					/* array in the code)		*/
{
	int	pid;			/* stores new process id	*/
	register struct	pentry	*pptr;	/* pointer to proc. table entry */
	int	i;
	long	*saddr;			/* stack address		*/
	register long	*sp;		/* move through 'initial stack'	*/
	long	*sp_top;		/* initial stack pointer	*/
	long	ps;			/* saved processor status	*/
	int	INITRET();

	disable(ps);
	if (!is_serv_init && numproc >= 1)
	    serv_create();		/* deferred creation of io_server */

	if ( ssize < MINSTK || isodd(procaddr) || priority < 1 ||
		((int)(saddr=(long *)getstk(ssize*sizeof(long))) == SYSERR) ||
		(pid=newpid()) == SYSERR ) {
		errno = ((int)saddr==SYSERR) ? errno : ((pid==SYSERR) ? CENOTASK
								      : EINVAL);
		restore(ps);
		return(SYSERR);
	}

	numproc++;
	pptr = &proctab[pid];
	pptr->pstate = PRSUSP;
	for (i=0 ; i<PNMLEN && (pptr->pname[i]=name[i])!=0 ; i++)
		;
	pptr->pprio = priority;
	pptr->pbase = (long)saddr;
	pptr->pstklen = ssize;
	pptr->plimit = (long) ( saddr - ssize + 1);
	pptr->pargs = nargs;
	pptr->paddr = (long)procaddr;
	pptr->sys_sem = screate(0);
	pptr->tdir    = NULL;		/* for chtdir, which is not finished */

	sp = sp_top = saddr - nargs - 12;

	proctab[pid].pregs[AP] = (long) (sp_top + 5);
	proctab[pid].pregs[FP] = proctab[pid].pregs[SP] = (long) sp_top;
	proctab[pid].pregs[PS] = ENABLE;

	*sp++ = (long) 0;			/* set up stack area to	*/
	*sp++ = (long) 04000000000;		/* point to two 'calls'	*/
	*sp++ = (long) (sp_top + 11);		/* frames.  One is to	*/
	*sp++ = (long) (sp_top + 6);		/* start the process at	*/
	*sp++ = ((long) procaddr) + 2;		/* the initial point.	*/
	*sp++ = (long) 0;			/* The other is for a	*/
	*sp++ = (long) 0;			/* procedure return to	*/
	*sp++ = (long) 04000000000;		/* INITRET().		*/
	*sp++ = (long) saddr;			/*			*/
	*sp++ = (long) saddr;			/* Frame is set up as:	*/
	*sp++ = ((long) INITRET) + 2;		/*		   0	*/
	*sp++ = (long) nargs;			/*	040000000000	*/
	for ( i = 1 ; i <= nargs ; i++)		/*	      old AP	*/
	    *sp++ = (long) *pargs++;		/*	      old FP	*/
						/*	          PC	*/
						/*	       #args	*/
						/*	        args	*/

	restore(ps);
	return(pid);
}

/*------------------------------------------------------------------------
 * newpid  --  obtain a new (free) process id
 *------------------------------------------------------------------------
 */
LOCAL	newpid()
{
	int	pid;			/* process id to return		*/
	int	i;

	for (i=0 ; i<NPROC ; i++) {	/* check all NPROC slots	*/
		if ( (pid=nextproc--) <= 0)
			nextproc = NPROC-1;
		if (proctab[pid].pstate == PRFREE)
			return(pid);
	}
	return(SYSERR);
}
