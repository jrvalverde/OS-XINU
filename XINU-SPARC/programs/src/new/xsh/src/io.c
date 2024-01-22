/* 
 * io.c - io routines 
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Jun 29 13:26:47 1990
 *
 * Copyright (c) 1990 Patrick A. Muckelbauer
 */

/*#define DEBUG*/

#include <stdio.h>

#include "main.h"
#include "utils.h"
#include "hash.h"

extern char *readline();
extern int linecnt;

static char linebuff[512];

/*
 * ====================================================================
 * gets_raw - call readline 
 * ====================================================================
 */
static char *gets_raw(fd)
     FILE *fd;
{
	linecnt++;
	if (fd != stdin)  {
		if ( fgets(linebuff, sizeof(linebuff), fd) == NULL ) {
			return(NULL);
		}
		else
		    return(strsave(linebuff));
	}
	else {
		return(readline(v_xfind("prompt")));
	}
}


/*
 * ====================================================================
 * gets_cooked - get raw line and add newline if necessary.  Add to
 *		 history.
 * ====================================================================
 */
char *gets_cooked(fd)
     FILE *fd;
{
	int len;
	char *in;
	
	in = gets_raw(fd);
	if ( in == NULL ) 
	    return(NULL);
	if (*in)
	    add_history(in);
	
	len = strlen(in);
	if ( (len == 0) || (in[len - 1] != '\n') ) {
		in = xrealloc(in, len + 1 + 1);
		in[len] = '\n';
		in[len+1] = '\0';
	}
	
#ifdef DEBUG
	fprintf(stdout, "gets_cooked() ... '%s'\n", in);
#endif
	return(in);
}
	    

