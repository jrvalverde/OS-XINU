/* create.c - create, newpid */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <mem.h>

/*------------------------------------------------------------------------
 *  create  -  create a process to start running a procedure
 *------------------------------------------------------------------------
 */
SYSCALL create(procaddr,ssize,priority,name,nargs,args)
	int	*procaddr;		/* procedure address		*/
	int	ssize;			/* stack size in bytes		*/
	int	priority;		/* process priority > 0		*/
	char	*name;			/* name (for debugging)		*/
	int	nargs;			/* number of args that follow	*/
	int	args;			/* arguments (treated like an	*/
					/* array in the code)		*/
{
	int	pid;			/* stores new process id	*/
	struct	pentry	*pptr;		/* pointer to proc. table entry */
	int	i;
	int	*a;			/* points to list of args	*/
	int	*saddr;			/* stack address		*/
	int	INITRET();		/* process return point		*/
	PStype	ps;			/* saved processor status	*/

	disable(ps);
	ssize = (int) roundew(ssize);
	if ( ssize<MINSTK || ((saddr=(int *)getstk(ssize))==(int *)SYSERR ) ||
		(pid=newpid()) == SYSERR || isodd(procaddr) ||
		priority < 1 ) {
		restore(ps);
		return(SYSERR);
	}
	numproc++;
	pptr = &proctab[pid];
	pptr->pstate = PRSUSP;
	for (i=0 ; i<PNMLEN && (pptr->pname[i]=name[i])!=0 ; i++)
		;
	pptr->pprio = priority;
	pptr->pbase = (int)saddr;
	pptr->pstklen = ssize;
	pptr->psem = 0;
	pptr->phasmsg = FALSE;
	pptr->plimit = (int) ((unsigned)saddr - ssize + sizeof(int));
	*saddr-- = MAGIC;
	pptr->pargs = nargs;
	for (i=0 ; i<PNREGS ; i++)	/* init pcb register entries	*/
		pptr->pregs[i]=INITREG;
					/* move PC past entry mask	*/
	pptr->pregs[PCINDX] = pptr->paddr = (int)procaddr + sizeof(short);
	pptr->pregs[PSINDX] = INITPS;
	pptr->pregs[P0BRINDX] = pptr->pregs[P1BRINDX] = INITBR;
	pptr->pregs[P0LRINDX] = INITLR;
	pptr->pnxtkin = BADPID;
					/* simulate stack of CALLG w/o	*/
					/* any registers saved (mask==0)*/
	a = (&args) + (nargs-1);	/* point to last argument	*/
	for ( ; nargs > 0 ; nargs--)	/* machine dependent; copy args	*/
		*saddr-- = *a--;	/* onto created process' stack	*/
	*saddr = (int)INITRET+sizeof(short);/* push return address	*/
					/* ret to INITRET, past entry mask*/
	pptr->pregs[APINDX]= (int)saddr;/* AP points to location on 	*/
					/* stack of 1st arg, -4 bytes	*/
	for (i=0; i<PNRETPOP; i++)	/* push values RET pops other	*/
		*--saddr = INITREG;	/* than PC (FP,AP,mask&PSW)	*/
	pptr->pregs[SPINDX] = pptr->pregs[FPINDX] = (int)saddr;
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
