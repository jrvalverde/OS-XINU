/* x_clock.c - x_clock */

#include <conf.h>
#include <kernel.h>
#include <q.h>
#include <date.h>
#include <sleep.h>
#include <clock.h>
#include <proc.h>

#define boolstr(bool) (bool?"true":"false")

LOCAL void scatprintf();
LOCAL	char	*pstnams[] = {"curr ","free ","ready","recv ",
			    "sleep","susp ","wait ","rtim "};

/*------------------------------------------------------------------------
 *  x_clock  -  Print info about the system clock and sleep queues
 *------------------------------------------------------------------------
 */
COMMAND	x_clock(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	STATWORD ps;
	u_long a,c;
	u_long sum;
	int qix;
	char datebuf[40];
	char nambuf[40];
	char buf[80*(NPROC+5)];
	
	fprintf(stdout, "  System Clock Info:\n");
	fprintf(stdout, "    clkruns: %s\n", boolstr(clkruns));
	fprintf(stdout, "    defclk:  %s", boolstr(defclk));
	fprintf(stdout, "    (clkdiff: %d)\n", clkdiff);

	disable(ps);
	a = clktime;  c = realtime;
	restore(ps);

	fprintf(stdout, "  Time Keeping:\n");
	fprintf(stdout, "    clktime (secs):     %d\n", a);
	fprintf(stdout, "    realtime (msecs):   %d\n", c);
	gettime(&a); ascdate(a,datebuf);
	fprintf(stdout, "    local time:         %s\n", datebuf);
	fprintf(stdout, "    hours west of GMT:  %d\n", TIMEZONE);
	

	fprintf(stdout, "  Sleep Queue:\n");
	disable(ps);
	*buf = NULLCH;
	qix = clockq;
	sum = 0;

	scatprintf(buf, "    slnempty: %s\n", boolstr(slnempty));
	scatprintf(buf,
		   "      pid    name    state  delta (usecs)  wakeup\n");
	scatprintf(buf,
		   "      ---  --------  -----  -------------  --------------------\n");
	while (nonempty(qix)) {
		qix = q[qix].qnext;
		sum += q[qix].qkey;
		ascdate(ut2ltim(clktime+(sum/1000000)),datebuf);
		strcpy(nambuf,proctab[qix].pname); nambuf[8] = NULLCH;
		scatprintf(buf, "      %3d  %8s  %-5s  %13d  %20s\n",
			qix, nambuf,
			pstnams[proctab[qix].pstate-1],
			q[qix].qkey, datebuf);
	}
	restore(ps);
	write(stdout,buf,strlen(buf));

	return(OK);
}

static void
scatprintf(str, fmt, a1,a2,a3,a4,a5,a6)
     char *str;
     char *fmt;
{
	char *pch;
	pch = &str[strlen(str)];
	sprintf(pch,fmt,a1,a2,a3,a4,a5,a6);
}
