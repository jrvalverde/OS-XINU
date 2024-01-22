/*--------------------------------------------------------------------------
 * ftutil.c --  frame table utilities. 
 *
 * Author:  	Jim Griffioen
 * Date:  	5-23-90
 * 		John Lin, 12-29-93 major modification
 *
 * NOTE:
 *    Frame table structure (ft[]) is a global structure accessed by many
 *    different processes. Make sure to pretect it using disable().
 *    The following utility routines assume the disable() is enforced.
 *---------------------------------------------------------------------------
 */

#include <conf.h>
#include <kernel.h>
#include <vmem.h>
#include <frame.h>
#include <proc.h>

/* when the kernel is fully debugged, remove all the elaborate error	*/
/* checking from these routines						*/
#define SHOWFATALERRS
#define SHOWPOSIBLERRS
/*#define DEBUG*/

/*-----------------------------------------------------------------------
 * ftnewlist -- initialize a list
 *-----------------------------------------------------------------------
 */
LOCAL ftnewlist(hdnum, tlnum, list)
     int	hdnum;
     int	tlnum;
     int	list;
{
    struct fte *hptr;
    struct fte *tptr;

    hptr = &ft[hdnum];	/* head */
    tptr = &ft[tlnum];	/* tail */
    hptr->next = tlnum;
    hptr->prev = EMPTY;
    hptr->bits.st.used = TRUE;
    hptr->bits.st.shared = FALSE;
    hptr->bits.st.wanted = FALSE;
    hptr->bits.st.pgout = FALSE;
    hptr->bits.st.list = list;
    tptr->next = EMPTY;
    tptr->prev = hdnum;
    tptr->bits.st.used = TRUE;
    tptr->bits.st.shared = FALSE;
    tptr->bits.st.wanted = FALSE;
    tptr->bits.st.pgout = FALSE;
    tptr->bits.st.list = list;
    return(OK);
}

/*-----------------------------------------------------------------------
 * ftinit -- initialize Reclaim,Active and Modified lists
 *-----------------------------------------------------------------------
 */
ftinit()
{
    ftnewlist(FTRHD, FTRTL, FTRLIST);	
    ftnewlist(FTAHD, FTATL, FTALIST);	
    ftnewlist(FTMHD, FTMTL, FTMLIST);	
}

/*-----------------------------------------------------------------------
 * ftinaf -- insert frame findex AFTER  pos 
 *-----------------------------------------------------------------------
 */
ftinaf(findex, pos)
     int     findex;
     int     pos; 
{
    PStype      ps;
    struct	fte *tptr;
    struct	fte *fteptr;

    disable(ps);
    if (isbadframe(findex) || (!ftlocked(findex))) {
#ifdef SHOWFATALERRS
	kprintf("ftinaf: called with illegal frame\n");
	kprintf("ftinaf: frame = %d\n", findex);
	panic("ftinaf: list = %d\n", ftlist(findex));
#endif
	restore(ps);
	return(SYSERR);
    }

    tptr = &ft[pos];
    fteptr = &ft[findex];
    ftdeclist(fteptr->bits.st.list);	/* decrement list count */
    fteptr->bits.st.list = tptr->bits.st.list;
    fteptr->bits.st.used = TRUE;
    ftinclist(fteptr->bits.st.list);
    fteptr->next = tptr->next;
    fteptr->prev = pos;
    ft[tptr->next].prev = findex;
    tptr->next = findex;
    restore(ps);
    return(OK);
}


/*-----------------------------------------------------------------------
 * ftinbf -- insert frame findex BEFORE pos 
 *-----------------------------------------------------------------------
 */
ftinbf(findex, pos)
     int findex;		/* index into frame table */
     int pos; 
{
    PStype      ps;
    struct	fte *tptr;
    struct	fte *fteptr;
    
    disable(ps);
    if (isbadframe(findex) || (!ftlocked(findex))) {
#ifdef SHOWFATALERRS
	kprintf("ftinbf: called with illegal frame\n");
	kprintf("ftinbf: frame = %d\n", findex);
	panic("ftinbf: list = %d\n", ftlist(findex));
#endif
	restore(ps);
	return(SYSERR);
    }
    tptr = &ft[pos];
    fteptr = &ft[findex];
    ftdeclist(fteptr->bits.st.list);	/* decrement list count */
    fteptr->bits.st.list = tptr->bits.st.list;
    fteptr->bits.st.used = TRUE;
    ftinclist(fteptr->bits.st.list);	/* increment list count */
    fteptr->next = pos;
    fteptr->prev = tptr->prev;
    ft[tptr->prev].next = findex;
    tptr->prev = findex;
    restore(ps);
    return(OK);
}

/*-----------------------------------------------------------------------
 * ftrm -- remove frame from the list on which it exists
 *-----------------------------------------------------------------------
 */
ftrm(findex)
     int findex;
{
    PStype      ps;
    struct fte *fteptr;

    disable(ps);

    if (isbadframe(findex) || (ftlocked(findex))) {
#ifdef SHOWFATALERRS
	kprintf("ftrm: called with illegal frame\n");
	kprintf("ftrm: frame = %d\n", findex);
	panic("ftrm: list = %d\n", ftlist(findex));
#endif
	restore(ps);
	return(SYSERR);
    }

    fteptr = &ft[findex];
    ftdeclist(fteptr->bits.st.list);
    ft[fteptr->prev].next = fteptr->next;
    ft[fteptr->next].prev = fteptr->prev;
    fteptr->bits.st.list = FTNLIST;	/* lock the frame */
    ftinclist(FTNLIST);
    fteptr->next = fteptr->prev = FTINVALIDFRAME;
    restore(ps);
    return(findex);
}

/*-----------------------------------------------------------------------
 * ftrmhd -- remove head frame from the list and returns findex
 *-----------------------------------------------------------------------
 */
ftrmhd(list)
     int	list; 
{
    PStype      ps;
    struct fte *fteptr;
    int next, ret;

    disable(ps);
    if (!isfthead(list)) {
#ifdef SHOWFATALERRS
	kprintf("ftrmhd: called with illegal head pointer\n");
	panic("ftrmhd: list = %d\n", list);
#endif
	restore(ps);
	return(SYSERR);
    }

    next = ft[list].next;
    if (isfttail(next)) {
	restore(ps);
	return (SYSERR); /* empty list */
    }

    ret = ftrm(next);
    restore(ps);
    return(ret);
}


/*
 *------------------------------------------------------------------------
 * freeframe - free N frame
 *------------------------------------------------------------------------
 */
freenframe(count, farray)
     int count;
     int farray[];
{
    int i, findex, fcount;
    PStype      ps;

    disable(ps);
    fcount = 0;
    for (i = 0; i < count; i++) {
	findex = farray[i];
	/* only free locked frames. Others may have been changed */
	if (ftlocked(findex)) {
	    ftinbf(findex, FTRTL);	/* to reclaim list */
	    fcount++;
	}
    }
    restore(ps);
    signaln(ftfreesem, fcount);		/* Wake up any waiting process */
}

/*-----------------------------------------------------------------------
 * ftnext -- get next fte pointer on the list
 * N.B. Assume disable() called 
 *-----------------------------------------------------------------------
 */
LOCAL ftnext(findex)
     int findex;
{
    int     next;

    next = ft[findex].next;

    if (next == EMPTY || isfttail(next)) {
	if (ftonalist(findex))
	    next = ft[FTAHD].next;
	else if (ftonmlist(findex))
	    next = ft[FTMHD].next;
	else if (ftonrlist(findex))
	    next = ft[FTRHD].next;
    }
	
    if (next == EMPTY || isfttail(next)) {
	panic("ftnext: empty list %d\n", ftlist(next));
	return(SYSERR);	/* empty list */
    }

    return(next);
}


/*-----------------------------------------------------------------------
 * ftanext -- get frame following findex on the active list
 *-----------------------------------------------------------------------
 */
ftanext(findex)
     int	findex;
{
    PStype      ps;
    int		ret;

    disable(ps);
    if ((!isfthdortl(findex)) && (isbadframe(findex))) {
#ifdef SHOWFATALERRS
	panic("ftanext: illegal frame number (%d)\n", findex);
#endif
	restore(ps);
	return(SYSERR);
    }
    else if (!ftonalist(findex)) {
#ifdef SHOWFATALERRS
	kprintf("ftanext: illegal frame number = %d\n", findex);
	panic("ftanext: list = %d\n", ftlist(findex));
#endif
	restore(ps);
	return(SYSERR);
    }

    ret = ftnext(findex);
    restore(ps);
    return(ret);
}

/*-----------------------------------------------------------------------
 * ftobsolete  -- if frame is obsolete return TRUE else FALSE
 *-----------------------------------------------------------------------
 */
ftobsolete(findex)
     int 	findex;
{
    PStype      ps;
    struct	fte *fteptr;
    int		id, ret;

    disable(ps);
    if (isbadframe(findex)) {
#ifdef SHOWFATALERRS
	panic("ftobsolete: bad frame %d\n", findex);
#endif
	restore(ps);
	return(SYSERR);
    }

    fteptr = &ft[findex];
    if (!fteptr->bits.st.used) {
	restore(ps);
	return(TRUE);
    }

    /* obsolete "old" frames, possibly the stack frames of user threads */
    id = fteptr->id;
    if (ispid(id)) {
	ret = (fteptr->tstamp != proctab[id].tstamp);
	restore(ps);
	return(ret);
    }

    if (isasid(id)) {
	ret = fteptr->tstamp != addrtab[asidindex(id)].tstamp;
	restore(ps);
	return(ret);
    }

#ifdef SHOWFATALERRS
    panic("ftobsolete: error: unknown id %d, frame=%d\n", id, findex);
#endif
    restore(ps);
    return(SYSERR);
}

/*---------------------------------------------------------------------------
 * ftinclist - increment a list count - alist, mlist, or rlist
 *---------------------------------------------------------------------------
 */
LOCAL ftinclist(list)
     int list;
{
    if (list == FTALIST)
	ftinfo.alen++;
    else if (list == FTRLIST)
	ftinfo.rlen++;
    else if (list == FTMLIST)
	ftinfo.mlen++;
    else if (list == FTNLIST)
	ftinfo.locked++;
    else {
#ifdef SHOWFATALERRS
	panic("ftinclist: bad list %d\n", list);
#endif
	return(SYSERR);
    }

    return(OK);
}

/*---------------------------------------------------------------------------
 * ftdeclist - decrement a list count - alist, mlist, or rlist
 *---------------------------------------------------------------------------
 */
LOCAL ftdeclist(list)
     int list;
{
    if (list == FTALIST)
	ftinfo.alen--;
    else if (list == FTRLIST)
	ftinfo.rlen--;
    else if (list == FTMLIST)
	ftinfo.mlen--;
    else if (list == FTNLIST)
	ftinfo.locked--;
    else {
#ifdef SHOWFATALERRS
	panic("ftdeclist: bad list %d\n", list);
#endif
	return(SYSERR);
    }
    return(OK);
}

/*---------------------------------------------------------------------------
 *  mvtoalist -- move a frame to the active list
 *		 MACHINE DEPENDENT
 *---------------------------------------------------------------------------
 */
mvtoalist(findex)
     int findex;		/* index into frame table */
{
    PStype      ps;

    disable(ps);
    if (!ftlocked(findex) || !ftonalist(fminfo.hand2)) {
	panic("mvtoalist: frame=%d not locked or hand2 l=%d\n",
	      findex, ftlist(fminfo.hand2));
    }
    /* insert it to the tail of the active list */
    ftinbf(findex, fminfo.hand2);
    restore(ps);
    return(OK);
}

/*---------------------------------------------------------------------------
 * ftgetppte - get ppteptr associated with the frame
 *---------------------------------------------------------------------------
 */
ftgetppte(findex, ppteptr)
     int findex;
     struct ppte *ppteptr;
{
    PStype      ps;
    int ret;

    disable(ps);
    ret = getppte(ft[findex].id, ft[findex].pageno * PGSIZ, ppteptr);
    restore(ps);
    return(ret);
}

/*
 *--------------------------------------------------------------------------
 * fteclear() - clear a frame table entry - to keep this routine machine
 *		independent we do not zero out the ppte, since the other
 *		fields are zeroed it will never be looked at anyway
 *--------------------------------------------------------------------------
 */
fteclear(fteptr)
     struct fte *fteptr;
{
    PStype      ps;

    disable(ps);
    fteptr->id = BADPID;
    fteptr->next = FTINVALIDFRAME;
    fteptr->prev = FTINVALIDFRAME;
    fteptr->tstamp = 0;
    fteptr->pageno = 0;
    fteptr->bits.st.used = FALSE;
    fteptr->bits.st.shared = FALSE;
    fteptr->bits.st.wanted = FALSE;
    fteptr->bits.st.pgout = FALSE;
    fteptr->bits.st.list = FTNLIST;
    restore(ps);
}

