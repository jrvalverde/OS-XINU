
/* 
 * io.c - This file contain unix to xinu io routines
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Tue Apr 17 15:02:53 1990
 *
 * Copyright (c) 1990 Jim Griffioen
 */

#include	<sys/xinusys.h>		/* xinu header file		*/
#include	<sys/file.h>		/* unix header file		*/
#include	<sys/termio.h>		/* unix header file		*/
#include	<sys/errno.h>		/* unix header file		*/
#include	"unixkernel.h"		/* simulated kernel structures	*/

/* struct taken from <sys/uio.h> */
struct iovec {
	char	*iov_base;
	int	iov_len;
};

/* simulated user's unix device table */
struct devicetable dtable[DEVTBLSIZE] = {
	{ 1, GUESS_CONSOLE_NUM, 0},		/* stdin */
	{ 1, GUESS_CONSOLE_NUM, 0},		/* stdout */
	{ 1, GUESS_CONSOLE_NUM, 0},		/* stderr */
};	/* per proc unix device table	*/

struct	devfnametbl dfnametable[DEVTBLSIZE]; /* per proc file name tbl	*/

/* estimated 'large' number of Xinu devices */
#define XDEVMAX 300
int xdevrefcount[XDEVMAX];

extern long errno;			/* system error value		*/

/*---------------------------------------------------------------------------
 * These functions simulate the unix io system calls.  It keeps a dtable
 * containing all the current process's open file descriptors.
 * A routine to initialize the dtable is included, but must somehow be
 * called by the program.  If the dtable initialization routine is not
 * invoked by the program, then it gets the CONSOLE as the default
 * stdin, stdout, and stderr devices.
 *---------------------------------------------------------------------------
 */

close(dev)
int dev;
{
	int status;

	if (--xdevrefcount[dtable[dev].xinudevnum] != 0)
	    return(0);
	
	status = xclose(dtable[dev].xinudevnum);
	dtable[dev].inuse = 0;
	if (status == SYSERR)
	    return(-1);
	else
	    return(0);
}

open(name, flags, mode)
char	*name;
int	flags;
int	mode;
{
	char xflags[4];
	int fd;
	int slot;
	char realfilename[512];
	char *tmpstr;
	char truncflag = 0;

	kstrcpy(xflags, "");

	if (flags == O_RDONLY) kstrcat(xflags,"r");
	else if (flags & O_WRONLY) kstrcat(xflags,"w");
	else if (flags & O_RDWR) kstrcat(xflags,"rw");
	else kstrcat(xflags, "w");

	if (flags & O_TRUNC) truncflag = 1;
	
	if (flags & O_EXCL) kstrcat(xflags, "n");
	else if (! (flags & O_CREAT)) kstrcat(xflags,"o");

	if (*name == '/') {		/* absolute file name */
		kstrcpy(realfilename, name);
	}
	else {				/* relative to PWD */
		if ((tmpstr=((char *)kgetenv("PWD"))) != (char *) NULL) {
			kstrcpy(realfilename, tmpstr);
			if (tmpstr[kstrlen(tmpstr)-1] != '/')
			    kstrcat(realfilename, "/");
			kstrcat(realfilename, name);
		}
		else {
			kstrcpy(realfilename, name);
		}
	}

	if (truncflag) {
		if ((fd = xopen(NAMESPACE, realfilename, "o")) != SYSERR) {
			/* file exists so remove it and make zero len file */
			xclose(fd);
			xremove(realfilename);
			if ((fd = xopen(NAMESPACE, realfilename, "n"))
			    == SYSERR) {
				return(-1);
			}
			xclose(fd);
		}
	}
	
	/* try full path name */
	if ((fd = xopen(NAMESPACE, realfilename, xflags)) == SYSERR) {
		return(-1);
	}
	/* the open succeeded */
	if ((slot = find_dtbl_slot()) != -1) {
		dtable[slot].inuse = 1;
		dtable[slot].xinudevnum = fd;
		++xdevrefcount[dtable[slot].xinudevnum];
		kstrcpy(dfnametable[slot].fname, name);
		return(slot);
	}
	else
	    return(-1);
}

read(desc, ptr, count)
     int desc;
     char *ptr;
     int count;
{
	int n, askfor, sofar;

	if (!dtable[desc].inuse || count < 0)
	    return(-1);

	if (count == 0)
	    return(0);
	
	sofar = 0;

	do {
		askfor = count < 512 ? count : 512;
		n = xread(readdtable(desc), ptr, askfor);
		if (n <= 0)
		    break;
		sofar += n;
		ptr += n;
		count -= n;
	} while ((count) && (n >= askfor));

	return(sofar);
}


lseek(dev, offset, whence)
int	dev;
long	offset;
long	whence;
{
	if (whence != 0)
	    return(-1);
	if (!dtable[dev].inuse)
	    return(-1);
	
	if (xseek(dtable[dev].xinudevnum, offset) == SYSERR)
	    return(-1);
	else
	    return(offset);
}

write(dev, buff, count)
int	dev;
char	*buff;
int	count;
{
	long len;

	if (!dtable[dev].inuse)
	    return(-1);
	
	if ((len=xwrite(dtable[dev].xinudevnum,buff,count)) == SYSERR)
	    return(-1);
	else
	    return(len);
}

writev(dev, iov, iovcnt)
int		dev;
struct iovec	*iov;
int		iovcnt;
{
	int i, len = 0, totlen = 0;
	
	for (i=0; i<iovcnt; i++) {
		if ((len =
		     write(dev, iov[i].iov_base, iov[i].iov_len)) == SYSERR)
		    return(-1);
		totlen += len;
	}
	return(totlen);
}


ioctl(d, request, argp)
int d, request;
char *argp;
{
	int i, j;
	char sb[4];
	
	if (request == (TCGETA)) {	/* Is device d a tty line? */
		if ((dtable[d].inuse) &&
		    (dtable[d].xinudevnum == GUESS_CONSOLE_NUM))
		    return(0);
		else {
			if (libwarning)
			    write(2,
				  "Illegal ioctl() TCGETA - return (-1)\n",
				  37);
			return(-1);
		}
	}
	else {
		if (libwarning) {
			write(2, "Illegal ioctl() - return (-1) request = ", 40);
			kprintx(request);
			write(2, ", des = ", 7);
			kprintx(d);
			write(2, "\n", 1);
		}
		return(-1);
	}
}



access(filename, mode)
char	*filename;
int	mode;
{
	char xmode[8];
	int fd;
	char *tmpstr;
	char realfilename[256];

	if (mode == 0) kstrcpy(xmode, "ro");
	else if (mode == 1) kstrcpy(xmode, "ro");
	else if (mode == 2) kstrcpy(xmode, "wo");
	else if (mode == 4) kstrcpy(xmode, "ro");


	if (*filename == '/') {		/* absolute file name */
		kstrcpy(realfilename, filename);
	}
	else {				/* relative to PWD */
		if ((tmpstr=((char *)kgetenv("PWD"))) != (char *) NULL) {
			kstrcpy(realfilename, tmpstr);
			if (tmpstr[kstrlen(tmpstr)-1] != '/')
			    kstrcat(realfilename, "/");
			kstrcat(realfilename, filename);
		}
		else {
			kstrcpy(realfilename, filename);
		}
	}

	/* If I am not mistaken, the remote file server does	*/
	/* not implement access - it accepts access requests,	*/
	/* but always returns SYSERR, so this is kinda useless	*/
	/* But in future versions of the remote fs ...		*/
	
	if (xaccess(realfilename,xmode) == SYSERR) {
		/* because the fs is flaky, try this */
		if ((fd = xopen(NAMESPACE, realfilename, xmode)) == SYSERR) {
			errno = EACCES;
			return(-1);
		}
		xclose(fd);
		return(0);
	}
	else
	    return(0);
}


unlink(filename)
char	*filename;
{
	char *tmpstr;
	char realfilename[256];

	if (*filename == '/') {		/* absolute file name */
		kstrcpy(realfilename, filename);
	}
	else {				/* relative to PWD */
		if ((tmpstr=((char *)kgetenv("PWD"))) != (char *) NULL) {
			kstrcpy(realfilename, tmpstr);
			if (tmpstr[kstrlen(tmpstr)-1] != '/')
			    kstrcat(realfilename, "/");
			kstrcat(realfilename, filename);
		}
		else {
			kstrcpy(realfilename, filename);
		}
	}

	if (xremove(realfilename) == SYSERR)
	    return(-1);
	else
	    return(0);
}

rename(filename, newfname)
char	*filename;
char	*newfname;
{
	char *tmpstr;
	char realfilename[256];
	char realnewfname[256];

	if (*filename == '/') {		/* absolute file name */
		kstrcpy(realfilename, filename);
	}
	else {				/* relative to PWD */
		if ((tmpstr=((char *)kgetenv("PWD"))) != (char *) NULL) {
			kstrcpy(realfilename, tmpstr);
			if (tmpstr[kstrlen(tmpstr)-1] != '/')
			    kstrcat(realfilename, "/");
			kstrcat(realfilename, filename);
		}
		else {
			kstrcpy(realfilename, filename);
		}
	}

	if (*newfname == '/') {		/* absolute file name */
		kstrcpy(realnewfname, newfname);
	}
	else {				/* relative to PWD */
		if ((tmpstr=((char *)kgetenv("PWD"))) != (char *) NULL) {
			kstrcpy(realnewfname, tmpstr);
			if (tmpstr[kstrlen(tmpstr)-1] != '/')
			    kstrcat(realnewfname, "/");
			kstrcat(realnewfname, newfname);
		}
		else {
			kstrcpy(realnewfname, newfname);
		}
	}

	if (xrename(realfilename, realnewfname) == SYSERR)
	    return(-1);
	else
	    return(0);
}


stat(path, buf)
char *path;
int *buf;
{
	if (libwarning)
	    write(2, "stat() not implemented quite right\n", 35);
	return(access(path,4));
}



fstat(fd, buf)
int fd;
int *buf;
{
	if (!dtable[fd].inuse)
	    return(-1);
	
	return(stat(dfnametable[fd].fname, buf));
}


dup(oldd)
int oldd;
{
	int slot;

	if (!dtable[oldd].inuse)
	    return(-1);
	
	if ((slot=find_dtbl_slot()) == -1)
	    return(-1);
	else {
		dtable[slot].inuse = 1;
		dtable[slot].xinudevnum = dtable[oldd].xinudevnum;
		++xdevrefcount[dtable[slot].xinudevnum];
		return(slot);
	}
}

dup2(oldd,newd)
int oldd, newd;
{
	int slot;

	if ((!dtable[oldd].inuse) || (newd >= DEVTBLSIZE))
	    return(-1);
	
	dtable[newd].inuse = 1;
	dtable[newd].xinudevnum = dtable[oldd].xinudevnum;
	++xdevrefcount[dtable[newd].xinudevnum];
	return(newd);
}


getdtablesize()
{
	return(DEVTBLSIZE);
}


find_dtbl_slot()
{
	int i;

	for (i=0;i<DEVTBLSIZE; i++)
	    if (!dtable[i].inuse)
		return(i);
	return(-1);
}

readdtable(index)
int index;
{
	if (dtable[index].inuse)
	    return(dtable[index].xinudevnum);
	else
	    return(-1);
}

installdevice(xinudev,filesize,name)
     int xinudev;
     int filesize;
     char *name;
{
	int slot;

	if (xinudev < 0)
	    return(-1);

	slot = find_dtbl_slot();
	if (slot == -1)
	    return(-1);

	dtable[slot].inuse = 1;
	dtable[slot].xinudevnum = xinudev;
	dtable[slot].filesize = filesize;
	if (name)
	    kstrcpy(dfnametable[slot].fname, name);

	xdevrefcount[xinudev]++;
	return(slot);
}

initdtable(xstdin, xstdout, xstderr)
int xstdin, xstdout, xstderr;
{
	int i;
	
	dtable[0].inuse = 1; dtable[0].xinudevnum = xstdin;
	dtable[1].inuse = 1; dtable[1].xinudevnum = xstdout;
	dtable[2].inuse = 1; dtable[2].xinudevnum = xstderr;
	for (i=3; i<DEVTBLSIZE; i++)
	    dtable[i].inuse = 0;
	for (i=0; i<XDEVMAX; i++)
	    xdevrefcount[i] = 0;
	++xdevrefcount[xstdin];
	++xdevrefcount[xstdout];
	++xdevrefcount[xstderr];
}

closedtable()
{
	int i;

	for (i=0; i<DEVTBLSIZE; i++)
	    close(i);
}


