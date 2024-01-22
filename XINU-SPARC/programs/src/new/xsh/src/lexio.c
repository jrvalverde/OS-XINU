
/* 
 * lexio.c - lex io routines
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Mon Aug  6 18:40:18 1990
 *
 * Copyright (c) 1990 Patrick A. Muckelbauer
 */
/*#define DEBUG*/

#include <stdio.h>

#include "main.h"
#include "utils.h"
#include "hash.h"

static char *input_line = NULL, *input_scan;

/*
 * ====================================================================
 * lexIOinit - initialize global variables.
 * ====================================================================
 */
lexIOinit(line)
     char *line;
{
	input_scan = input_line = line;
}

/*
 * ====================================================================
 * FreeLexIO - free the allocated lines
 * ====================================================================
 */
FreeLexIO()
{
	xfree(input_line);
}

/*
 * ====================================================================
 * input - input routine called by yylex
 * ====================================================================
 */
input()
{
	if (*input_scan == '\0') { /* incase readline is used */
	 	return(LEXEOF);
	}
	return(*input_scan++);
}
	
	

/*
 * ====================================================================
 * unput - unput character called by yylex
 * ====================================================================
 */
unput(ch)
     int ch;
{
#ifdef DEBUG
	fprintf(stdout, "unput....%d\n", ch);
#endif
	if ( ch <= 0 )
	    return;
	if ( (--input_scan) < input_line ) {
		FatalError("unput()...drop character", NULL);
	}

	*input_scan = ch;
}


	
