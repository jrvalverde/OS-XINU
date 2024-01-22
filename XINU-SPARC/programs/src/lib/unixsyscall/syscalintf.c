/* 
 * syscalintf.c - System Call Interface for Sun-4c
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 */

#include	<sys/xinusys.h>
#include	<sys/syscall.h>
#include	<varargs.h>

#define IP_ALEN		4
typedef char 		IPaddr[IP_ALEN];

/*----------------------------------------------------------------------
 * syscall - all system calls go through this routine to copy
 *	     arguments to rsa area and call systrap which executes
 *	     trap instruction
 *
 *  Syscall stack format        IN                 OUT
 *       RSASTRTADDR   ==>  function number     return value
 *       RSASTRTADDR+1 ==>  arg 1                  --
 *       RSASTRTADDR+2 ==>  arg 2                  --
 *       RSASTRTADDR+3 ==>  arg 3                  --
 *       RSASTRTADDR+4 ==>  arg 4                  --
 *       RSASTRTADDR+5 ==>  arg 5                  --
 *       RSASTRTADDR+6 ==>  arg 6                  --
 *       RSASTRTADDR+7 ==>  arg 7                  --
 *       RSASTRTADDR+8 ==>  arg 8                  --
 *
 *---------------------------------------------------------------------- */
syscall(fnum, nargs, va_alist)
     int	fnum;		/* kernel function number 	*/
     int	nargs;		/* # of arguments 		*/
     va_dcl
{
    int i, *rsa;
    va_list args;
    
#ifdef LOG_SYSCALLS
    {
	static int stop = FALSE;
	
	if (!stop) {
	    stop = TRUE;
	    printf("syscall(%d,,) called\n", fnum);
	    stop = FALSE;
	}
    }
#endif
    
    if (nargs > 8) {		/* max number of arguments is 8, now*/
	return(SYSERR);
    }
    
    va_start(args);

    /* use RSA area to pass system call arguments */
    rsa = (int *)RSASTRTADDR;
    *rsa++ = fnum;
    
    for( i= 0; i < nargs; i++)
	*rsa++ = va_arg(args,int);
    
    systrap();
    
    return(*(int *)RSASTRTADDR);	/* Xinu put return value here */
}

/*---------------------------------------------------------------------------
 * The following routines turn all system calls to function number, number
 * of arguments and a list of arguments, then call syscall routine
 *-------------------------------------------------------------------------*/

xaccess(filename, mode)
     char	*filename, *mode;
{
    return(syscall(SYSC_ACCESS, 2, filename, mode));
}

xascreate(fname, entryp)
     char	*fname;
     unsigned *entryp;
{
    return(syscall(SYSC_ASCREATE, 2, fname, entryp));
}


xchprio(pid, newprio)
     int	pid, newprio;
{
    return(syscall(SYSC_CHPRIO, 2, pid, newprio));
}

xclose(dev)
     int dev;
{
    return(syscall(SYSC_CLOSE, 1, dev));
}

xcontrol(dev, function, arg1, arg2)
     int dev, function, arg1,arg2;
{
    return(syscall(SYSC_CONTROL, 4, dev, function, arg1, arg2));
}


xcreate(fname, priority, name, nargs, va_alist)
     char 	*fname;
     int	priority;
     char 	*name;
     int	nargs;
     va_dcl
{
    int	asid, pid;
    unsigned entry;
    va_list args;
    
    va_start (args);
    asid = xascreate(fname, &entry);
    pid = xprocreate(asid, (char *)entry, priority, name, nargs, args);
    va_end(args);
    if (asid != SYSERR && pid != SYSERR )
	return(pid);
    else
	return(SYSERR);
}

xdvlookup(devname)
     char  *devname;
{
    return(syscall(SYSC_DVLOOKUP, 1, devname));	
}

xfreeheap(ptr, bytes)
     int  *ptr;
     int  bytes;
{
    return(syscall(SYSC_FREEHEAP, 2, ptr, bytes));	
}

xgetaddr(ip)
     IPaddr ip;
{
    return(syscall(SYSC_GETADDR, 1, ip));	
}

#ifdef SHAWNS_VERSION
xgetname(name)
     char *name;
{
    return(syscall(SYSC_GETNAME, 1, name));	
}
#endif

xgetc(dev)
     int dev;
{
    return(syscall(SYSC_GETC, 1, dev));
}

xgetheap(bytes)
     int  bytes;
{
    return(syscall(SYSC_GETHEAP, 1, bytes));	
}

xgetnet(ip)
     IPaddr	ip;
{
    return(syscall(SYSC_GETNET, 1, ip));	
}


xgetpid()
{
    return(syscall(SYSC_GETPID,0,0));
}

xgetasid()
{
    return(syscall(SYSC_GETASID,0,0));
}

xgetprio(pid)
     int pid;
{
    return(syscall(SYSC_GETPRIO, 1, pid));
}

xgetutim(timvar)
     long	*timvar;
{
    return(syscall(SYSC_GETUTIM, 1, timvar));
}

#ifdef SHAWNS_VERSION
xgetutimus(psecs,pusecs)
     u_long	*psecs;
     u_long	*pusecs;
{
    return(syscall(SYSC_GETUTIMUS, 2, psecs, pusecs));
}
#endif

xgetrealtim(timvar)
     long	*timvar;
{
    return(syscall(SYSC_GETREALTIM, 1, timvar));
}

xip2name(ip, name)
     IPaddr	ip;
     char	*name;
{
    
    return(syscall(SYSC_IP2NAME, 2, ip, name));
    
}

xkill(pid)
     int 	pid;
{
    return(syscall(SYSC_KILL, 1, pid));
}

xlogin(dev)
     int 	dev;
{
    return(syscall(SYSC_LOGIN, 1, dev));
}

xmount(prefix, dev, replace)
     char 	*prefix;
     int	dev;
     char 	*replace;
{
    return(syscall(SYSC_MOUNT, 3, prefix, dev, replace));
}

xname2ip(ip, name)
     IPaddr	ip;
     char	*name;
{
    
    return(syscall(SYSC_NAME2IP, 2, ip, name));
    
}

xopen(dev, name, mode)
     int	dev;
     char	*name;
     char	*mode;
{
    return(syscall(SYSC_OPEN, 3, dev, name, mode));
}

xpanic(message)
     char	*message;
{
    return(syscall(SYSC_PANIC, 1, message));
}

xpcount(portid)
     int	portid;
{
    return(syscall(SYSC_PCOUNT, 1, portid));
}

xpcreate(count)
     int	count;
{
    return(syscall(SYSC_PCREATE, 1, count));
}

xpdelete(portid, dispose)
     int	portid;
     int	(*dispose)();
{
    return(syscall(SYSC_PDELETE, 2, portid, dispose));
}

char    *xpreceive(portid)
     int	portid;
{
    
    return((char *)syscall(SYSC_PRECEIVE, 1, portid));
}

xpreset(portid, dispose)
     int	portid;
     int	(*dispose)();
{
    return(syscall(SYSC_PRESET, 2, portid, dispose));
}

xprocreate(asid, procaddr, priority, name, nargs, argpt)
     int	asid;
     int	*procaddr;
     int	priority;
     char	*name;
     int	nargs;
     int	*argpt;
{
    return(syscall(SYSC_PROCREATE, 6, asid, procaddr,
		   priority, name, nargs, argpt));
}

xpsend(portid, message)
     int	portid;
     char	*message;
{
    return(syscall(SYSC_PSEND, 2, portid, message));
}

xputc(dev, ch)
     int	dev;
     int	ch;
{
    return(syscall(SYSC_PUTC, 2, dev, ch));
}

xread(dev, buffer, numchars)
     int	dev;
     char	*buffer;
     int	numchars;
{
    return(syscall(SYSC_READ, 3, dev, buffer, numchars));
}

xreceive()
{
    return(syscall(SYSC_RECEIVE, 0, 0));
}

xrecvclr()
{
    return(syscall(SYSC_RECVCLR, 0, 0));
}

xrecvtim(maxwait)
     int	maxwait;
{
    return(syscall(SYSC_RECVTIM, 1, maxwait));
}

#ifdef SHAWNS_VERSION
xrecvtimus(secs, usecs)
     int secs;
     int usecs;
{
    return(syscall(SYSC_RECVTIMUS, 2, secs, usecs));
}
#endif

xremove(filename, key)
     char	*filename;
     int	key;
{
    return(syscall(SYSC_REMOVE, 2, filename, key));
}

xrename(filename, newfname)
     char	*filename;
     char	*newfname;
{
    return(syscall(SYSC_RENAME, 2, filename, newfname));
}

xresume(pid)
     int	pid;
{
    return(syscall(SYSC_RESUME, 1, pid));
}

xscount(sem)
     int	sem;
{
    return(syscall(SYSC_SCOUNT, 1, sem));
}

xscreate(count)
     int	count;
{
    return(syscall(SYSC_SCREATE, 1, count));
}

xsdelete(sem)
     int	sem;
{
    return(syscall(SYSC_SDELETE, 1, sem));
}

xseek(dev, buffer, position)
     int	dev;
     char	*buffer;
     long	position;
{
    return(syscall(SYSC_SEEK, 3, dev, buffer, position));
}

xsend(pid, msg)
     int	pid, msg;
{
    return(syscall(SYSC_SEND, 2, pid, msg));
}

xsendf(pid, msg)
     int	pid, msg;
{
    return(syscall(SYSC_SENDF, 2, pid, msg));
}

xsetdev(pid, dev1, dev2)
     int	pid, dev1, dev2;
{
    return(syscall(SYSC_SETDEV, 3, pid, dev1, dev2));
}

xsetnok(nok, pid)
     int	nok, pid;
{
    return(syscall(SYSC_SETNOK, 2, nok, pid));
}

xshell(dev)
     int	dev;
{
    return(syscall(SYSC_SHELL, 1, dev));
}

xsignal(sem)
     int	sem;
{
    return(syscall(SYSC_SIGNAL, 1, sem));
}

xsignaln(sem, count)
     int	sem, count;
{
    return(syscall(SYSC_SIGNALN, 2, sem, count));
}

xsleep(secs)
     int	secs;
{
    return(syscall(SYSC_SLEEP, 1, secs));
}

xsleep10(tenths)
     int	tenths;
{
    return(syscall(SYSC_SLEEP10, 1, tenths));
}

#ifdef SHAWNS_VERSION
xsleepus(secs, usecs)
     int secs;
     int usecs;
{
    return(syscall(SYSC_SLEEPUS, 2, secs, usecs));
}
#endif

xsreset(sem, count)
     int	sem, count;
{
    return(syscall(SYSC_SRESET, 2, sem, count));
}

xsuspend(pid)
     int	pid;
{
    return(syscall(SYSC_SUSPEND, 1, pid));
}

xtcreate(procaddr, priority, name, nargs, va_alist)
     int   *procaddr;
     int   priority;
     char  *name;
     int   nargs;
     va_dcl
{
    va_list ap;
    int args[10];
    int i;
    
    va_start(ap);
    for (i=0; i < nargs; i++) {
	args[i] = va_arg(ap, int);
    }
    va_end(ap);
    return(syscall(SYSC_TCREATE, 5, procaddr, priority,
		   name, nargs,
		   args[0], args[1], args[2], args[3], args[4],
		   args[5], args[6], args[7], args[8], args[9]
		   ));
}

xunmount(prefix)
     char	*prefix;
{
    return(syscall(SYSC_UNMOUNT, 1, prefix));
}

xunsleep(pid)
     int	pid;
{
    return(syscall(SYSC_UNSLEEP, 1, pid));
}

xuserret()
{
    return(syscall(SYSC_USERRET, 0, 0));
}

xwait(sem)
     int	sem;
{
    return(syscall(SYSC_WAIT, 1, sem));
}

xwrite(dev, buff, count)
     int	dev;
     char	*buff;
     int	count;
{
    return(syscall(SYSC_WRITE, 3, dev, buff, count));
}


xxinu_execv(filename, prio, stdin, stdout, stderr, nargs, args)
     char	*filename;
     int	prio;
     int	stdin, stdout, stderr, nargs;
     char	*args[];
{
    return(syscall(SYSC_XINUEXECV, 7, filename, prio,
		   stdin, stdout, stderr, nargs, args));
}


#ifdef SHAWNS_VERSION
xselect(nbits, inputs, outputs, inresults, outresults, time)
     int nbits;
     fd_set *inputs, *outputs, *inresults, *outresults;
     int     time;
     
{
    return(syscall(SYSC_SELECT, 6, nbits,
		   inputs, outputs, inresults, outresults, time));
}
#endif


xnop()
{
    return(syscall(SYSC_NOP, 0, 0));
}

xstrlen(sb)
     char *sb;
{
    return(syscall(SYSC_STRLEN, 1, sb));
}


xkprintf(fmt, a1, a2, a3, a4, a5)
     char *fmt;
     int a1, a2, a3, a4, a5;
{
    return(syscall(SYSC_KPRINTF, 6, fmt, a1, a2, a3, a4, a5));
}

