
/* $Header$ */

/* 
 * fastputc.c - do buffered writes using putc
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Tue Sep 11 11:12:24 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log$
 */

#ifdef XINU8

#include "def.h"

#define BUFSIZE	512

static char 	buffer[BUFSIZE];
static int 	nextchar = 0;

int fastputc(c, ffp)
     int c;
     FILE *ffp;
{
    int status;

    if (nextchar >= BUFSIZE) {
	/* write out the buffer to the file */
	if ((status = flushputc(ffp)) < 0) {
	    return(status);
	}
    }

    buffer[nextchar++] = c & 0xff;
    return(c);
}

flushputc(ffp)
     FILE *ffp;
{
    int status;

    /* write out the buffer to the file */
    if ((status = fwrite(buffer, 1, nextchar, ffp)) < 0)
	return(status);
    nextchar = 0;
}

#endif
