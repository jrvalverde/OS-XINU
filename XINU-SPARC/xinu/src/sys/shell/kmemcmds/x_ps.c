/* x_ps.c - x_ps */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <vmem.h>

LOCAL	char	hd1[] =
	"pid   name     asid typ state prio mem/usd mem/res %cpu stksz sem msg context\n";
LOCAL	char	hd2[] =
	"--- ---------- ---- --- ----- ---- ------- ------- ---- ----- --- --- ------\n";
LOCAL	char	*pstnams[] = {"curr ","free ","ready","recv ",
			    "sleep","susp ","wait ","rtim "};


#define PSMODEMASK 	0x2000
/*------------------------------------------------------------------------
 *  x_ps  -  (command ps) format and print process table information
 *------------------------------------------------------------------------
 */
COMMAND	x_ps(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
    int	pid;
    struct	pentry	*pptr;
    unsigned long currstk;
    int	pgs_mapped;
    int	pgs_inmem;
	
    write(stdout, hd1, strlen(hd1));
    write(stdout, hd2, strlen(hd2));
    for (pid=0 ; pid<NPROC ; pid++) {
	if ((pptr = &proctab[pid])->pstate == PRFREE)
	    continue;
	fprintf(stdout, "%3d %10s %4d %3s %s ",
		pid,
		pptr->pname,
		pptr->asid,
		iskernproc(pid)?" K ":" U ",
		pstnams[pptr->pstate-1]);
	fprintf(stdout, "%4d ", pptr->pprio);
	if (!iskernproc(pid) &&
	    (memusage(pid,0,vmaddrsp.maxheap ,&pgs_mapped,&pgs_inmem) == OK))
	    fprintf(stdout, "%7x %7x ", pgs_mapped * PGSIZ, pgs_inmem * PGSIZ);
	else
	    fprintf(stdout, "     -       -  ");

	fprintf(stdout, "     ");
	/* stack size */
	if (pid != 0) {	/* skip null proc */
	    currstk = pptr->pregs[SP];
	    fprintf(stdout, "%5x ", vmaddrsp.kernstk - currstk);
	}
	else
	   fprintf(stdout, "  ?   ");
	
	if (pptr->pstate == PRWAIT)
	    fprintf(stdout, "%3d ", pptr->psem);
	else
	    fprintf(stdout, " -  ");
	if (pptr->phasmsg)
	    fprintf(stdout, "%3x ", pptr->pmsg);
	else
	    fprintf(stdout, " -  ");

	if (pptr->ctxt >= 0)
	    fprintf(stdout, "%4d", pptr->ctxt);
	else
	    fprintf(stdout, "%s","  no");
	fprintf(stdout, "\n");
    }
}





