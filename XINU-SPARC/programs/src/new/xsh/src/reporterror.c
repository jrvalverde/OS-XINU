/* 
 * main.c - main program
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 */

#include <stdio.h>
#include "main.h"

extern	int	linecnt;
extern 	int	source_num;
extern	int	errors;

/*
 * ====================================================================
 * ReportError - print out the error string and the line it occurred on.
 * ====================================================================
 */
ReportError(str)
char	*str;
{
	char **scan;

	errors++;
	if ( source_num != 0 )
	    fprintf(stderr, "Error in line %d: ", linecnt);
	else
	    fprintf(stderr, "Error: ");

	for( scan = &str; *scan != NULL; scan++ )
	    fprintf(stderr, "%s ", *scan);
	fprintf(stderr, "\n");
	fflush(stderr);
}

/*
 * ====================================================================
 * FatalError - Fatal error occured
 * ====================================================================
 */
FatalError(str)
char	*str;
{
	char **scan;

	if ( source_num != 0 )
	    fprintf(stderr, "Fatal in line %d: ", linecnt);
	else
	    fprintf(stderr, "Fatal: ");

	for( scan = &str; *scan != NULL; scan++ )
	    fprintf(stderr, "%s ", *scan);
	fprintf(stderr, "\n");
	fflush(stderr);
	exit(1);
}

