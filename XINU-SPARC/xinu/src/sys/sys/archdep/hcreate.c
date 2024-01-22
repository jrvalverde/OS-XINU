/* 
 * hcreate.c - High level creation procedures
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Wed Apr 20 13:41:43 1988
 *
 * Copyright (c) 1988 Jim Griffioen
 */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <mem.h>
#include <vmem.h>

/*#define DEBUG*/

#define NUMARGS		64
#define	TMPSTRSIZE	4*1024

/*------------------------------------------------------------------------
 *  xinu_execv
 *------------------------------------------------------------------------
 */
COMMAND	xinu_execv(filename, prio, stdin, stdout, stderr, nargs, args)
char	*filename;
int	prio;
int	stdin, stdout, stderr, nargs;
char	*args[];
{
    int		pid, i, ptr_size;
    int 	asid;
    int		strlng, len;	/* length of string argument and total length*/
    int		argary[5];	/* argument array to be copied into U-stack */
    char 	*argaddr;
    char	*sbrsaddr;	/* start of param string pointer addresses */
    char 	temp[TMPSTRSIZE], *chptr;
    /* string argument temp. buffer and pointer */
    int		argvarray[NUMARGS];	/* temp array of string ptrs	*/
    char	runname[PNMLEN];	/* run name for process table	*/
    unsigned 	entry;
    struct	mblock	tmpmblock;
    char	*vaddr;

#ifdef DEBUG
    kprintf("in xinu_execv(%s, prio=%d, ", filename, prio);
    kprintf("stdin=%d, stdout=%d, stderr=%d, nargs=%d)\n",
	    stdin, stdout, stderr, nargs);
#endif
	
    argary[0] = stdin;
    argary[1] = stdout;
    argary[2] = stderr;
    argary[3] = nargs;
    for (i = 0, chptr = temp, len = 0; i < nargs; i++) {
	strlng = strlen(args[i]) + 1;
	if (chptr + strlng > temp + TMPSTRSIZE) {
	    kprintf("xinu_execv: out of space for copying args\n");
	    return(SYSERR);
	}
	strcpy(chptr, args[i]);
	/* copy string to temp. buffer first */
	argvarray[i] = (unsigned) len;
	/* the string will be copied at this addr */
	len += strlng;			/* increase the total length of string args */
	chptr += strlng;
    }

    /* make len to be a multiple of word-length */
    len = ((len + sizeof(int) - 1) & ~(sizeof(int) - 1));
		
    /* extracting the filename suffix */
    mkrunname(filename, runname);
    if ((asid = ascreate(filename, &entry)) == SYSERR) {
	return(SYSERR);
    }
	
    /* now that the as is created we can find the start of the heap, and
       put the arguments there (and update all the pointers we are using
       here). */
    argaddr = (char *) vmemlist[asidindex(asid)].mnext;	/* start of heap */
    for (i = 0; i < nargs; i++)
	argvarray[i] += (unsigned) argaddr;
    
    argary[4] = (unsigned) argaddr + len;
    sbrsaddr = (char *)  argary[4];

    /* setup process table and stuff */
    pid = procreate(asid, (int *)entry, prio, runname, 5, argary);

    if (pid == SYSERR)
	return (SYSERR);
    
    /* move the start of the heap: first, get orig. heap record */
    vbcopy(argaddr, pid, &tmpmblock, currpid, sizeof(struct mblock), TRUE);

    ptr_size = nargs * sizeof(int);	/* to store string pointers */
    vmemlist[asidindex(asid)].mnext = (struct mblock *)
	(((unsigned long) vmemlist[asidindex(asid)].mnext) + len + ptr_size);
    vaddr = (char *) vmemlist[asidindex(asid)].mnext;
    tmpmblock.mlen -= (unsigned) (len + ptr_size);
    
    /* write first heap block into new location */
    vbcopy(&tmpmblock, currpid, vaddr, pid, sizeof(struct mblock), TRUE);

    /* copy argument strings to new proc's as */
    vbcopy(temp, currpid, argaddr, pid, len, TRUE);
    /* copy argument string pointers to new proc's as */
    vbcopy(argvarray, currpid, sbrsaddr, pid, ptr_size, TRUE);
    setdev(pid, stdin, stdout);
    setnok(getpid(), pid);
    recvclr();
    return(pid);
}


/*---------------------------------------------------------------------------
 *  create - create a user process
 *---------------------------------------------------------------------------
 */
create(fname, priority, name, nargs, argsp)
    char  *fname;
    int   priority;
    char  *name;
    int   nargs;
    int   *argsp;
{
    int   asid, pid;
    unsigned  entry;

    if ((asid = ascreate(fname, &entry)) == SYSERR) {
	return(SYSERR);
    }
    if ((pid = procreate(asid, (int *)entry, priority, name,
			 nargs, argsp)) == SYSERR ) {
	return(SYSERR);
    }

    /* everything is ok - return newly created pid */
    return(pid);
}

/*---------------------------------------------------------------------------
 *  tcreate - create a thread within a address space
 *---------------------------------------------------------------------------
 */
tcreate(procaddr, priority, name, nargs, args)
    int   *procaddr;
    int   priority;
    char  *name;
    int   nargs;
    int   args;
{
    int   asid, pid;

    asid = proctab[currpid].asid;

    if ((pid = procreate(asid, procaddr, priority, name, nargs, &args))
	!= SYSERR )
	return(pid);

    return(SYSERR);
}

/*---------------------------------------------------------------------------
 * mkrunname - name the process to be run (by extracting the filename suffix)
 *---------------------------------------------------------------------------
 */
mkrunname(filename, runname)
char *filename;
char *runname;
{
    int len;
    char *tmpsb;
    char *sb;

    len = strlen(filename)+1;		/* 1 for the '/0' */
    if (len > (PNMLEN)) 
	tmpsb = filename + (len-PNMLEN);
    else
	tmpsb = filename;
    /* check if name has a '/' in it - take everything after '/' */
    sb = tmpsb+strlen(tmpsb);
    while((*sb != '/') && (sb != tmpsb))
	sb--;
    if (*sb == '/')
	sb++;
    strcpy(runname, sb);
}
