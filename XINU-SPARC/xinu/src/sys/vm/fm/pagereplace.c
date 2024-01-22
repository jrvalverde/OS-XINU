/*---------------------------------------------------------------------------
 * pagereplace.c - Page replacement algorithm using two-handed
 *                 global clock algorithm.
 *
 * Notes:
 * ------
 *	Although the 2-handed global clock routine is reasonably easy to
 * understand, this routine is actually rather complicated. There are
 * several subtlies that occur here.
 *
 *      First note that we must be certain that we do not page fault at
 * any time.  Second note that we would like to avoid context switching
 * as much as possible for at least 2 reasons:
 *
 * 	(a) to find as many frames as possible all at once (don't want to
 * context switch on every frame we find), and
 *	(b) every time we context switch, we allow other processes to play
 * with the frame lists (in particular, by the time we get to run again,
 * the 2 hands may not even point to frames on the active list any more).
 *
 * 	The third thing to note is that we do not want to disable interrupts
 * for very long.  This sort of contradicts our goal of reducing context
 * switches.  So, two words of warning to those who modify this.
 * First, at any point in the routine that a context switch can occur, you
 * must add code to figure out what happened since the time you got context
 * switched.  Second, note that freeframe signals waiting process which
 * causes a context switch.
 *
 * Major revision: 01/94, John Lin (lin@cs.purdue.edu)
 *---------------------------------------------------------------------------
 */

#include <conf.h>
#include <kernel.h>
#include <vmem.h>
#include <frame.h>
#include <proc.h>
#include <pgio.h>

int	fmgr;			/* frame manager process id		*/

LOCAL int hand_dist;		/* clock hand distance in pages */
LOCAL int hand_dist2;		/* clock hand safe distance in pages */

#define	MAX_RECLAIMED	 16	/* max frames to free to the reclaim list */
LOCAL saved_frames[MAX_RECLAIMED];

/*-----------------------------------------------------------------------
 * init_hands - init clock hands 
 *-----------------------------------------------------------------------
 */
void init_hands()
{
    /* init */
    fminfo.hand1 = FTAHD;
    fminfo.hand2 = FTATL;
    hand_dist = min(FMCLKDIST, ftinfo.frameavail >> 2);
    hand_dist2 = hand_dist + 2;
}

/*-----------------------------------------------------------------------
 *  pagereplace -- global clock page replacement algorithm 
 *-----------------------------------------------------------------------
 */
pgrplce()
{
    PStype	ps;                          
    struct  	ppte ppte, *ppteptr;
    int		savcount = 0, findex, desire_mlen;

    /* Wait for the alist to get long enough */
    while(ftinfo.alen < (hand_dist << 1)) {
	recvtim(FMSLEEP);
    }

    /* Initialize both hands */
    disable(ps);
    /* hand2 follows hand1 */
    fminfo.hand2 = ft[FTAHD].next;
    fminfo.hand1 = fmftrav_alist(fminfo.hand2, hand_dist);
    restore(ps);

    savcount = 0;
    while (TRUE) {			/* loop forever */
	
	/* the reclaim list is long enough, don't bother */
	while (ftinfo.rlen > FM_HIGH)
	    recvtim(FMSLEEP);

	/*
	 * Note that hand2 and had1 are traversing a dynamically changing
	 * active list. Thus, hand1 will be right behind hand2 if the length
	 * of the active list is exactly hand_dist.
	 * It causes hand2 to modify the frame hand1 is about to point to.
	 * The following guarantees that it will not happen. --lin
	 */
	/* if the active list is too short */
	while (ftinfo.alen < hand_dist2 || ftinfo.mlen > FM_HIGH) {
	    desire_mlen = ftinfo.mlen - MAX_PAGEOUT;		
	    pgiowakeup();	/* wakeup page server to do the work */
	    while (ftinfo.mlen > desire_mlen)
		recvtim(1);
	}

	disable(ps);
	/* start scanning active list */

	findex = fminfo.hand1;
	/* move hand1 one frame forward */
	fminfo.hand1 = ftanext(findex);
	if (!ftobsolete(findex)) {
	    /* Turn off Reference bit in first hand */
	    ppteptr = &ppte;
	    ftgetppte(fminfo.hand1, ppteptr);
	    setreference(ppteptr, FALSE);
	}
	
	/* check hand2 */
	findex = fminfo.hand2;
	
	/* move hand2 one frame forward */
	fminfo.hand2 = ftanext(findex);

	if (ftobsolete(findex)) {
	    ftrm(findex);		/* locked it */
	    saved_frames[savcount++] = findex;
	}
	else {				/* not obsolete */
	    /* Check if page was referenced */
	    ppteptr = &ppte;
	    ftgetppte(findex, ppteptr);
	    
	    if (!getreference(ppteptr)) { /* not referenced */
		/* begin stealing frame */
		ftrm(findex);		/* remove it from the active list */
		if (getmod(ppteptr))
		    ftinbf(findex, FTMTL); /* moved to modified list */
		else			/* frame not mod & not ref */
		    saved_frames[savcount++] = findex;
		sethvalid(ppteptr, FALSE); /* invalid it */
	    } /* if (!getreference) */
	} /* if (ftobsolete(findex)) */

	if (savcount == MAX_RECLAIMED) {
	    /* free them in one shot */
	    freenframe(MAX_RECLAIMED, saved_frames);
	    savcount = 0;
	}

	restore(ps);
    } /*  while (TRUE) */
}


/*---------------------------------------------------------------------------
 * fmftrav_alist - traverse forward on the alist maxlen frames or till start
 *---------------------------------------------------------------------------
 */
LOCAL fmftrav_alist(start, maxlen)
     int start, maxlen;
{
    int		i;
    int		j;
    int		lasti;

    if ((!ftonalist(start)) || (maxlen < 0)) {
	return(SYSERR);
    }

    if (maxlen == 0)
	return(start);

    lasti = start;
    i = ftanext(start);
    j = 0;
    while ((j < maxlen) && (i != start)) {
	lasti = i;
	i = ftanext(i);
	j++;
    }
    return(lasti);
}
