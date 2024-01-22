
/* 
 * commands.c - process the commands
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 */

/*#define DEBUG*/

#include <stdio.h>
#include <sys/file.h>		/* unix header file		*/
#include <fcntl.h>

#include "main.h"
#include "parse.h"

extern int xsh_debug;

/*
 * ====================================================================
 * ProcessCmd - process the command node
 * ====================================================================
 */	
ProcessCmd(cmd)
     struct cmd_rec *cmd;
{
	int n;
	int fdin, fdout, fderr;
	int xfdin, xfdout, xfderr;
	
	if ( cmd == NULL ) {
		FatalError("ProcessCmd()...null cmd", NULL);
	}
	if ( cmd->cmd.value == NULL ) {
		FatalError("ProcessCmd()...null cmd->cmd", NULL);
	}
	
	if ( xsh_debug ) 
	    PrintCmd(cmd);

	if ((n = builtin(cmd->cmd.value)) >= 0)  {
	    	ProcessBuiltIn(n, cmd);
	}
	else {

		if ( cmd->input.value != NULL ) {
			if ( (fdin = open(cmd->input.value, O_RDONLY)) < 0 ) {
				ReportError("counldn't redirect input", NULL);
				return;
			}
		}
		else
		   fdin = STDIN;
		xfdin = readdtable(fdin);
		if ( cmd->output.value != NULL ) {
			if ( (fdout=open(cmd->output.value,O_WRONLY|O_CREAT))<0 ) {
				ReportError("couldn't redirect output", NULL);
				close(fdin);
			}
		}
		else
		    fdout = STDOUT;
		xfdout = readdtable(fdout);
		fderr = STDERR;
		xfderr = readdtable(fderr);
	    	ProcessDynamic(cmd, xfdin, xfdout, xfderr);
	}
	fflush(stdout);
	fflush(stderr);
}

/*
 * ====================================================================
 * PrintCmd - debugging routine.  Called if xsh_debug is set.
 * ====================================================================
 */	
static PrintCmd(cmd)
     struct cmd_rec *cmd;
{
	struct arglist *a;
	int n;

	fprintf(stdout, "%s ", cmd->cmd.value);
	for (a = cmd->args; a != NULL ; a = a->next) {
		fprintf(stdout, "%s ", a->arg);
	}
	if (cmd->input.value != NULL)
	    fprintf(stdout, "< %s ", cmd->input.value);
	if (cmd->output.value != NULL)
	    fprintf(stdout, "> %s", cmd->output.value);
	fprintf(stdout, "\n");
	fflush(stdout);
}
