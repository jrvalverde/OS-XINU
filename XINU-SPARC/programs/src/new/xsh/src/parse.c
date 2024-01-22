
/* 
 * parse.c - routines to do the parsing of input
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 */

/*#define DEBUG*/

#include <stdio.h>

#include "main.h"
#include "parse.h"
#include "utils.h"
#include "hash.h"

struct cmd_rec *cmd_node = NULL;
int errors = 0;

/*
 * ====================================================================
 * parse - initialize data structures and call yyparse
 * ====================================================================
 */
struct cmd_rec *parse(line)
     char *line;
{

#ifdef DEBUG
	fprintf(stdout, "parse line: %s", line);
#endif
	cmd_node = NULL;
	errors = 0;
	
	lexIOinit(line);		/* initialize input line 	*/
	yyparse(); 			/* parse the input 		*/
	FreeLexIO();			/* free the input line		*/
	
	if (errors != 0 ) {
		FreeCmdList(cmd_node);
		return(NULL);
	}
	return(cmd_node);
}


/*
 * ====================================================================
 * FreeCmdList - free the command list
 * ====================================================================
 */
FreeCmdList(cmd)
    struct cmd_rec *cmd;
{
	struct cmd_rec  *curr, *next;
	
	for( curr = cmd; curr != NULL; ) {
		next = curr->next;
		FreeCmd(curr);
		curr = next;
	}
}

/*
 * ====================================================================
 * FreeCmd - free the command record
 * ====================================================================
 */
static FreeCmd(cmd)
    struct cmd_rec *cmd;
{
	if ( cmd == NULL )
	    return;
	xfree(cmd->cmd.value);
	xfree(cmd->input.value);
	xfree(cmd->output.value);
	FreeArgList(cmd->args);
	xfree(cmd);
}

	
/*
 * ====================================================================
 * FreeArgList - free the argument list
 * ====================================================================
 */
static FreeArgList(args)
    struct arglist *args;
{
	struct arglist *curr, *next;
	
	for( curr = args; curr != NULL; ) {
		next = curr->next;
		FreeArg(curr);
		curr = next;
	}
}

/*
 * ====================================================================
 * FreeArg - free the argument record
 * ====================================================================
 */
FreeArg(arg)
     struct arglist *arg;
{
	if ( arg == NULL )
	    return;
	xfree(arg->arg);
	xfree(arg);
}
	
