
/* $Header$ */

/* 
 * xmalloc.c - do a malloc and check the results
 * 
 * Programmer:	Scott M Ballew
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Sep  6 16:56:48 1990
 *
 */

/*
 * $Log$
 */

void *xmalloc(size)
     register int size;
{
    register void *retval;

    retval = (void *) malloc(size);
    if (!retval) {
	panic("xmalloc: out of memory");
    }
    return(retval);
}
