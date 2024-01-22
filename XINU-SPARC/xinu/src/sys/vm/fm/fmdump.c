/* fmdump.c - fmdump */

#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <vmem.h>
#include <frame.h>
#include <proc.h>
#include <sem.h> /* for debug */


/*------------------------------------------------------------------------
 *  fmdump  -	dump the contents of the frame table structure
 *		A id of -1 means to dump the entire table contents
 *------------------------------------------------------------------------
 */
fmdump(id)
int 	id;
{
    int	pid;
    int	asid;
    int	frame;
    struct	fte	*pfte;

    if ((id != (-1)) && isbadpid(id) && isbadasid(id)) 
	return(SYSERR);

    if (id == (-1)) {
	kprintf("\nEntire Frame Table contents are:\n\n");
    }
    else if (ispid(id)) {
	pid = id;
	asid = proctab[pidindex(id)].asid;
	kprintf("\nFrames owned by pid = %d in asid = %d\n\n",
		id, asid);
    }
    else {				/* is asid			*/
	pid = BADPID;
	asid = id;
	kprintf("\nFrames owned by asid = %d\n\n", id);
    }
    kprintf("frame # idtype  id   virt addr  used locked ");
    kprintf("shared wanted pgout tstamp list\n");
    kprintf("_______ ______ ____ ___________ ____ ______ ");
    kprintf("______ ______ _____ ______ ____\n");

	
    pfte = ft;
    for (frame = 0; frame < ftinfo.frameavail; ++frame, ++pfte) {
	if (id == (-1)) {
	    if (pfte->bits.st.used) {
		/*      frame#  idtype id  virtaddr used */
		kprintf(" %5d     %c   %4d  %09x    %1d  ",
			frame, (isasid(pfte->id)?'A':'P'),
			pfte->id, tovaddr(pfte->pageno),
			TRUE);
		/*      locked   shared   wanted    pgout*/
		kprintf("   %1d      %1d     %1d      %1d   ",
			ftlocked(frame),
			pfte->bits.st.shared,
			pfte->bits.st.wanted,
			pfte->bits.st.pgout);
		/*	tstamp list			*/
		kprintf("%6d   %c\n",
			pfte->tstamp,
			ftlocked(frame)?'N':
			(ftonalist(frame)?'A':
			 (ftonmlist(frame)?'M':'R')));
	    }
#ifdef LOTSOUTPUT			
	    else {			/* frame not in use - free frame */
		/*      frame#  idtype id  virtaddr used */
		kprintf(" %5d     %c   %4d  %09x    %1d  ",
			frame, 'N', (-1), 0, FALSE);
		/*      locked   shared   wanted    pgout*/
		kprintf("   %1d      %1d     %1d      %1d   ",
			FALSE, FALSE, FALSE, FALSE);
		/*	tstamp list			*/
		kprintf("%6d   %c\n",
			0, 'R');
	    }
#endif
	}
	else if ((pid != BADPID) && (pid == pfte->id)) {
	    if (pfte->bits.st.used) {
		/*      frame#  idtype id  virtaddr used */
		kprintf(" %5d     %c   %4d  %09x    %1d  ",
			frame, 'P',
			pid, tovaddr(pfte->pageno),
			TRUE);
		/*      locked   shared   wanted    pgout*/
		kprintf("   %1d      %1d     %1d      %1d   ",
			ftlocked(frame),
			pfte->bits.st.shared,
			pfte->bits.st.wanted,
			pfte->bits.st.pgout);
		/*	tstamp list			*/
		kprintf("%6d   %c\n",
			pfte->tstamp,
			ftlocked(frame)?'N':
			(ftonalist(frame)?'A':
			 (ftonmlist(frame)?'M':'R')));
	    }
	}
	else if ((asid != BADASID) && (asid == pfte->id)) {
	    if (pfte->bits.st.used) {
		/*      frame#  idtype id  virtaddr used */
		kprintf(" %5d     %c   %4d  %09x    %1d  ",
			frame, 'A',
			asid, tovaddr(pfte->pageno),
			TRUE);
		/*      locked   shared   wanted    pgout*/
		kprintf("   %1d      %1d     %1d      %1d   ",
			ftlocked(frame),
			pfte->bits.st.shared,
			pfte->bits.st.wanted,
			pfte->bits.st.pgout);
		/*	tstamp list			*/
		kprintf("%6d   %c\n",
			pfte->tstamp,
			ftlocked(frame)?'N':
			(ftonalist(frame)?'A':
			 (ftonmlist(frame)?'M':'R')));
	    }
	}
    }
    return(OK);
}


