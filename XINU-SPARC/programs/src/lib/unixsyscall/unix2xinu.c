/* 
 * unix2xinu.c - convert unix system calls to xinu system calls
 *		This file contains the neccesary variable declarations
 *		as well as a few simple unix system calls that are
 *		trivially implemented in terms of xinu system calls
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Mon Feb  6 09:14:20 1989
 *
 * Copyright (c) 1989 Jim Griffioen
 */

#include	<sys/xinusys.h>		/* xinu header file		*/
#include	<sys/file.h>		/* unix header file		*/
#include	<sys/errno.h>		/* unix header file		*/
#include	"unixkernel.h"		/* simulated kernel structures	*/

extern char *kstrcpy();

#define PGSIZ		0x1000		/* SPARC page size  = 4k */
#define rndtopg(x)	(((PGSIZ-1) + (int)(x)) & (~(PGSIZ-1)))
#define ONE_MEG		(1024*1024)
#define HALF_MEG	(ONE_MEG/2)

/*---------------------------------------------------------------------------
 * The following are variable used in a unix kernel - we simulate
 * them at the library level
 *---------------------------------------------------------------------------
 */
long	errno;				/* stores error code 		*/

static char	*strtbreak;		/* initial break point		*/
static char	*curbreak;		/* current break point		*/
static char	*maxbreak;		/* maximum break point		*/

#define MAGIC_ENV_STR	"ENVCNT="	/* first arg that changes environ */

char libwarning=TRUE;			/* print library error messages	*/


/*---------------------------------------------------------------------------
 * The following routines emulate unix routines using xinu routines
 *---------------------------------------------------------------------------
 */

execv(name, argv)
char *name;
char *argv[];
{
	int i;
#ifdef DOESNTWORK
	i = 0;
	while (argv[i] != NULL)
	    i++;
	xxinu_execv(name, xgetprio(xgetpid()),
		    readdtable(0), readdtable(1), readdtable(2),
		    i, argv);
	kill(xgetpid());
#else
	xwrite(0, "execv does not work yet\n", 26);
#endif
}


/*-------------------------------------------------------------------------
 * initheap - 
 *-------------------------------------------------------------------------
 */
initheap()
{
    char *ptr;

    /* this sets the curbreak to a page boundary */
    ptr = (char *) xgetheap(PGSIZ);
    if (ptr == (char *) SYSERR) {
	kprints("initheap: xgetheap failed\n");
	return(-1);
    }
    curbreak = (char *) rndtopg(((long) ptr));
    strtbreak = curbreak;
    xfreeheap( curbreak, (PGSIZ-(curbreak-ptr)) );
}

sbrk(bytes)
int  bytes;
{
	char *oldbreak;
	char *tmp;

	if (bytes < 0) {
		write(2, "sbrk error\n", 11);
		return(-1);
	}
	bytes = rndtopg((bytes));
	if (bytes > 0) {
		if ((tmp=(char *)xgetheap(bytes)) == (char *) SYSERR) {
			write(2, "sbrk: out of mem\n", 17);
			return(-1);
		}
		if (tmp != curbreak) {
			kprints("sbrk: tmp != curbreak: somethings wrong\n");
		}
	}
	oldbreak = curbreak;
	curbreak += bytes;
	return((int) oldbreak);
}

brk(newbreak)
char *newbreak;
{
	newbreak = (char *) rndtopg((newbreak));
	if (newbreak < strtbreak) {
		kprints("brk: invalid break point\n");
		return(-1);
	}
	if (newbreak < curbreak) {	/* free some space */
		xfreeheap(newbreak, curbreak-newbreak);
	}
	else if (newbreak > curbreak) {
		if (sbrk(newbreak-curbreak) == (-1)) {
			return(-1);
		}
	}
	curbreak = newbreak;
	return(0);
}

getpagesize()
{
	return(PGSIZ);
}

getpid()
{
	return(xgetpid());
}

kill(pid)
int 	pid;
{
	if (pid == xgetpid())
	    unixshutmedown();
	else
	    return(xkill(pid));
}

_exit(status)
int 	status;
{
	unixshutmedown();
}

sleep(secs)
int	secs;
{
	return(xsleep(secs));
}

signal(signum, procaddr)
int signum;
char *procaddr;
{
	if (libwarning)
	    write(2, "signal() not implemented\n", 25);
	return((int)procaddr);		/* bogus - but leave for the moment */
}


/*-------------------------------------------------------------------------
 * initenviron - 
 *-------------------------------------------------------------------------
 */
initenviron(argc, argv)
int argc;
char *argv[];
{
    int i;

    for (i=0; i<MAXENVIRON; i++)
	environ[i]=NULL;

    if (argc == 0) {
	defaultenv();
    }
    else {
	for (i=0; (i<argc) && (i<MAXENVIRON); i++)
	    environ[i] = argv[i];
    }
}


/*-------------------------------------------------------------------------
 * defaultenv - 
 *-------------------------------------------------------------------------
 */
defaultenv()
{
    environ[0] = (char *) xgetheap(40);
    kstrcpy(environ[0], "PATH=:/u/u14/lin/xinu");
	
    environ[1] = (char *) xgetheap(40);
    kstrcpy(environ[1], "HOME=/u/u14/lin");
	
    environ[2] = (char *) xgetheap(40);
    kstrcpy(environ[2], "TERM=xterms");
	
    environ[3] = (char *) xgetheap(700);
    kstrcpy(environ[3], "TERMCAP=v2|xterms|vs100s:co#80:li#24:cr=^M:do=^J:nl=^J:bl=^G:le=^H:ho=\\E[H:cl=\\E[H\\E[2J:bs:am:cm=\\E[%i%d;%dH:nd=\\E[C:up=\\E[A:ce=\\E[K:cd=\\E[J:so=\\E[7m:se=\\E[m:us=\\E[4m:ue=\\E[m:md=\\E[1m:mr=\\E[7m:me=\\E[m:ku=\\EOA:kd=\\EOB:kr=\\EOC:kl=\\EOD:kb=^H:k1=\\EOP:k2=\\EOQ:k3=\\EOR:k4=\\EOS:ta=^I:pt:sf=\\n:sr=\\EM:al=\\E[L:dl=\\E[M:ic=\\E[@:dc=\\E[P:MT:ks=\\E[?1h\\E=:ke=\\E[?1l\\E>:is=\\E[r\\E[m\\E[2J\\E[H\\E[?7h\\E[?1;3;4;6l:rs=\\E[r\\E<\\E[m\\E[2J\\E[H\\E[?7h\\E[?1;3;4;6l:xn:AL=\\E[%dL:DL=\\E[%dM:IC=\\E[%d@:DC=\\E[%dP:ti=\\E7\\E[?47h:te=\\E[2J\\E[?47l\\E8:hs:ts=\\E[?E\\E[?%i%dT:fs=\\E[?F:es:ds=\\E[?E:");
	
    environ[4] = (char *) xgetheap(40);
    kstrcpy(environ[4], "USER=somebody");
}


/*-------------------------------------------------------------------------
 * unixstartup - 
 *-------------------------------------------------------------------------
 */
unixstartup(xstdin, xstdout, xstderr, argc, argv)
int xstdin;
int xstdout;
int xstderr;
int argc;
char *argv[];
{
    int envcnt, argcnt;

    if ((argc > 0) &&
	(kstrncmp(MAGIC_ENV_STR, argv[0], kstrlen(MAGIC_ENV_STR)) == 0)) {
	envcnt = katoi(((char *) argv[0]) + kstrlen(MAGIC_ENV_STR));
	argcnt = argc - envcnt; 
    }
    else {
	envcnt = 0;
	argcnt = argc;
    }
    initenviron(envcnt, argv);
    initheap();
    initdtable(xstdin, xstdout, xstderr);
    main(argcnt, &(argv[envcnt]), environ);
    unixshutmedown();
}


unixshutmedown()			/* clean up at exit time	*/
{
	closedtable();			/* close all open files		*/
	xuserret();			/* trap to kernel and exit	*/
}




/*---------------------------------------------------------------------------
 * The following functions make this library of simulated unix system
 * calls completely self contained.
 *---------------------------------------------------------------------------
 */

/*
 * Concatenate s2 on the end of s1.  S1's space must be large enough.
 * Return s1.
 */

char *
kstrcat(s1, s2)
register char *s1, *s2;
{
	register char *os1;

	os1 = s1;
	while (*s1++)
		;
	--s1;
	while (*s1++ = *s2++)
		;
	return(os1);
}

/*
 * Copy string s2 to s1.  s1 must be large enough.
 * return s1
 */

char *
kstrcpy(s1, s2)
register char *s1, *s2;
{
	register char *os1;

	os1 = s1;
	while (*s1++ = *s2++)
		;
	return(os1);
}

/*
 * Returns the number of
 * non-NULL bytes in string argument.
 */

kstrlen(s)
register char *s;
{
	register n;

	n = 0;
	while (*s++)
		n++;

	return(n);
}

/*
 * Compare strings:  s1>s2: >0  s1==s2: 0  s1<s2: <0
 */

kstrcmp(s1, s2)
register char *s1, *s2;
{

	while (*s1 == *s2++)
		if (*s1++=='\0')
			return(0);
	return(*s1 - *--s2);
}

/*
 * Compare strings (at most n bytes):  s1>s2: >0  s1==s2: 0  s1<s2: <0
 */

kstrncmp(s1, s2, n)
register char *s1, *s2;
register n;
{

	while (--n >= 0 && *s1 == *s2++)
		if (*s1++ == '\0')
			return(0);
	return(n<0 ? 0 : *s1 - *--s2);
}

katoi(p)
register char *p;
{
	register int n;
	register int f;

	n = 0;
	f = 0;
	for(;;p++) {
		switch(*p) {
		case ' ':
		case '\t':
			continue;
		case '-':
			f++;
		case '+':
			p++;
		}
		break;
	}
	while(*p >= '0' && *p <= '9')
		n = n*10 + *p++ - '0';
	return(f? -n: n);
}


/*---------------------------------------------------------------------------
 * print a number as decimal to stderr
 *---------------------------------------------------------------------------
 */
kprintd(n)
int n;
{
	int i,j;
	char sb[4];
	
	j = 10000000;
	while (n > 0) {
		i = n / j;
		sb[0] = (char) ('0' + ((char)i));
		write(2, sb, 1);
		n = n % j;
		j = j / 10;
	}
}


/*---------------------------------------------------------------------------
 * print a number as hexidecimal to stderr
 *---------------------------------------------------------------------------
 */
kprintx(n)
int n;
{
	int i,j;
	char sb[4];
	
	write(2, "0x", 2);
	j = 0x10000000;
	while (n > 0) {
		i = n / j;
		if (i < 10)
		    sb[0] = (char) ('0' + ((char)i));
		else if (i < 16)
		    sb[0] = (char) ('a' + ((char)(i-10)));
		else
		    sb[0] = '?';
		write(2, sb, 1);
		n = n % j;
		j = j / 0x10;
	}
}


/*---------------------------------------------------------------------------
 * print a string
 *---------------------------------------------------------------------------
 */
kprints(sb)
char *sb;
{
	write(2, sb, kstrlen(sb));
}



static char *knvmatch();

/*---------------------------------------------------------------------------
 * kgetenv - get a specific environment variable
 *---------------------------------------------------------------------------
 */
char *
kgetenv(name)
register char *name;
{
	register char *v, **p=environ;

	if(p == (char **) NULL)
		return(NULL);
	while(*p != NULL)
		if((v = knvmatch(name, *p++)) != NULL)
			return(v);
	return(NULL);
}

/*---------------------------------------------------------------------------
 * knvmatch - Find matching environment variable
 *---------------------------------------------------------------------------
 *	s1 is either name, or name=value
 *	s2 is name=value
 *	if names match, return value of s2, else NULL
 *	used for environment searching: see getenv
 */
static char *
knvmatch(s1, s2)
register char *s1, *s2;
{
	while(*s1 == *s2++)
		if(*s1++ == '=')
			return(s2);
	if(*s1 == '\0' && *(s2-1) == '=')
		return(s2);
	return(NULL);
}
