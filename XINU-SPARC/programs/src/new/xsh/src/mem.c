
/* 
 * mem.c - my mem routines for debugging purposes
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Mon Jul 23 14:23:11 1990
 *
 * Copyright (c) 1990 Patrick A. Muckelbaer
 */

/*#define DEBUG*/

#include <stdio.h>

#define	FREE_ENAMBLE

extern char *malloc(), *realloc();

/*
 * xmalloc - allocate at leaset size bytes of memory
 */
char * xmalloc(size)
     int size;
{
	char *new;

	if ( (new = malloc(size)) == NULL ) {
		FatalError("malloc returned NULL", NULL);
	}
	return(new);
}


/*
 * xfree - free the allocated block
 */
xfree(ptr)
     char *ptr;
{


	if ( ptr == NULL )
	    return;
	
#ifdef FREE_ENAMBLE 
	free(ptr);
#endif
}


/*
 * xrealloc - realloc the memeory space
 */
char *xrealloc(old, size)
     char *old;
     unsigned int size;
{
	char *new;

	
	if ( (new = realloc(old, size)) == NULL ) {
		FatalError("realloc returned NULL", NULL);
	}
	
	return(new);
}


