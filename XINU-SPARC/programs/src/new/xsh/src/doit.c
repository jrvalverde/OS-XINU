
/* 
 * doit.c - source the particular file
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Mon Aug  6 11:18:42 1990
 *
 * Copyright (c) 1990 Patrick A. Muckelbauer
 */

/*#define DEBUG*/

#include <stdio.h>
#include <sys/file.h>

#include "main.h"
#include "parse.h"
#include "utils.h"
#include "hash.h"
#include "tn.h"

extern int linecnt;
extern char *gets_cooked(), *AliasFind();
extern struct	cmd_rec	*parse();
extern char *VarSub();

extern struct cmd_rec *g_cmd;

/*
 * doit -  source the particular file.  Repeatidly Read a line of input
 * 	   untill eof is detected, parse the line, and execute the
 *	   commands.
 */
doit(fd)
     FILE *fd;
{
	int prev_linecnt, len;
	char *line, *alias;
	struct	cmd_rec	*cmdlist;
		
	prev_linecnt = linecnt;
	linecnt = 0;
	while ( (line = gets_cooked(fd)) != NULL ) {
		cmdlist = parse(line);
		HandleAlias(cmdlist, g_cmd, TRUE, FALSE);
		FreeCmdList(cmdlist);
	}
	linecnt = prev_linecnt;
}

/* 
 * HandleAlias - Take each command, do variable and parameter substitution,
 *		 and process the command.  If an alias exist, reparse
 *		 and call HandleAlias recursively.  Note old_cmd is
 *		 used for parameter substitution.
 */ 
static HandleAlias(cmdlist, old_cmd, unmark, tackon)
     struct cmd_rec *cmdlist, *old_cmd;
     int unmark, tackon;
{
	struct cmd_rec *cmd, *aliaslist;
	char *alias;
	int cmdc;

	cmdc = 0;
	for (cmd = cmdlist; cmd != NULL; cmd = cmd->next ) {
		if ( unmark )
		    a_unmarktbl();
		FixCmd(cmd, old_cmd, cmdc, tackon);
		if ( (cmd->cmd.type != SWORD) &&
		  ((alias = AliasFind(cmd->cmd.value)) != NULL) ) {
			aliaslist = parse(alias);
			HandleAlias(aliaslist, cmd, FALSE, TRUE);
			FreeCmdList(aliaslist);
		}
		else {
	    		ProcessCmd(cmd);
		}
		cmdc++;
	}
}

/*
 * FixCmd - Do the variable substituion and parameter substitution.
 */
static FixCmd(cmd, old_cmd, cmdc, tackon)
     struct cmd_rec *cmd, *old_cmd;
     int cmdc, tackon;
{
	int subs;
	struct arglist *args;

	subs = FALSE;
	if ( cmd->cmd.type != SWORD ) 
	    cmd->cmd.value = VarSub(cmd->cmd.value, old_cmd, &subs);
	
	if ( cmd->input.value != NULL && cmd->input.type != SWORD )
	    cmd->input.value = VarSub(cmd->input.value, old_cmd, &subs);
	
	if ( cmd->output.value != NULL && cmd->output.type != SWORD )
	    cmd->output.value = VarSub(cmd->output.value, old_cmd, &subs);
		    
	subs = FixArgs(cmd, old_cmd);
	CompressArgs(cmd);

	if ( cmd->input.value == NULL && cmdc == 0 ){
		cmd->input.value = strsave(old_cmd->input.value);
	}
		    
	if ( cmd->output.value == NULL && cmd->next == NULL ){
		cmd->output.value = strsave(old_cmd->output.value);
	}
	if ( cmd->next == NULL && old_cmd->backgrounding )
	    cmd->backgrounding = TRUE;

	if ( tackon ) { /* tackon the previous old_cmd's arguments to cmd */
		
		if ( !subs && cmdc == 0 && cmd->next == NULL ) {
			if ( cmd->args == NULL )
			    cmd->args = old_cmd->args;
			else {
			    	for ( args = cmd->args; args->next != NULL;
					 args = args->next)
					;
				args->next = old_cmd->args;
			}
			old_cmd->args = NULL;
		}
		
	}
}


/*
 * FixArgs - Make variable substitutions on the arguments.  Also handles
 * 	     the '$*' parameter.
 */
static int FixArgs(cmd, old_cmd)
     struct cmd_rec *cmd, *old_cmd;
{
 	struct arglist *args, *tmp, *c_args;
	int tsubs, subs;

	tsubs = subs = FALSE;
	for (args = cmd->args; args != NULL; args = args->next) 
	    if ( args->type != SWORD ) {
	    	if ( strequ(args->arg, "$*") ) {
		    	xfree(args->arg);
		    	args->arg = NULL;
		    	tmp = args->next;
		    	for(c_args = old_cmd->args; c_args != NULL;
			    	c_args = c_args->next) 			{
			    	args->next = (struct arglist *)
			    		malloc(sizeof(struct arglist));
			    	args = args->next;
			    	args->arg = strsave(c_args->arg);
		    		args->next = NULL;
		    	}
		    	args->next = tmp;
			subs = TRUE;
		}
		else
		    args->arg = VarSub(args->arg, old_cmd, &subs);
		if ( subs )
		    tsubs = TRUE;
	    }
	return(tsubs);
}


/*
 * AliasFind - find the alias and terminate with a newline (for parser)
 */
static char *AliasFind(name)
     char *name;
{
	char *p;
	int len;
	
	if ( (p = a_mfind(name)) != NULL ) {
		p = strsave(p);
		len = strlen(p);
		if ( (len == 0) || (p[len - 1] != '\n') ) {
			p = xrealloc(p, len + 1 + 1);
			p[len] = '\n';
			p[len+1] = '\0';
		}
	}
	return(p);
}


/*
 * CompressArgs - remove NULL arguments from the list 
 */
static CompressArgs(cmd)
     struct cmd_rec *cmd;
{
 	struct arglist *args, *argsN, *argsF, *tmp;

	argsF = argsN = NULL;
	for ( args = cmd->args; args != NULL; ) {
		if ( args->arg == NULL ) {
			tmp = args;
			args = args->next;
			FreeArg(tmp);
		}
		else {
			if ( argsF == NULL ) {
				argsF = argsN = args;
			}
			else {
				argsN->next = args;
				argsN = args;
			}
			args = args->next;
		}
	}
	if ( argsN != NULL )
	    argsN->next = NULL;
	cmd->args = argsF;
}

