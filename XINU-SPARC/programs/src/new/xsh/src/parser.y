%{
/* 
 * parser.y - parser for nut shell
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 *
 */

#include <stdio.h>
#include "main.h"
#include "parse.h"
#include "utils.h"
#include "hash.h"
	
extern struct	cmd_rec *cmd_node;

static struct cmd_rec *currCmd, *AddToCmdList();
static struct arglist *currArg, *AddToArgList();


%}

%token SWORD DWORD WORD REDIRIN REDIROUT BACKGROUND NEWLINE SEMI

%union {
	char 	*s;
}

%token <s> WORD DWORD SWORD
    
%%
inputline	:	commandlist NEWLINE
    		|
    			NEWLINE
    		|
    			error NEWLINE
				{
					yyerrok;
				}
		;

commandlist	:	commandlist SEMI command
    		|
    			command  
    		;

command		:	cmd arglist redirin redirout background
    		;

cmd		:	WORD
				{
					currCmd = AddToCmdList($1, WORD);
				}
		|
			DWORD
				{
					currCmd = AddToCmdList($1, DWORD);
				}
		|
			SWORD
				{
					currCmd = AddToCmdList($1, SWORD);
				}
		;

arglist		:	arglist arg
    		|
    			/* empty */
    		;

arg		:	WORD
				{
					currArg = AddToArgList($1, WORD);
				}
		|
			DWORD
				{
					currArg = AddToArgList($1, DWORD);
				}
		|
			SWORD
				{
					currArg = AddToArgList($1,SWORD);
				}
		;


redirin		:	REDIRIN WORD
				{
					currCmd->input.value = $2;
					currCmd->input.type = WORD;
				}
    		|
			REDIRIN DWORD
				{
					currCmd->input.value = $2;
					currCmd->input.type = DWORD;
				}
		|
			REDIRIN SWORD
				{
					currCmd->input.value = $2;
					currCmd->input.type = SWORD;
				}
		|
    			/* empty */
    		;

redirout	:	REDIROUT WORD
				{
					currCmd->output.value = $2;
					currCmd->output.type = WORD;
				}
    		|
			REDIROUT DWORD
				{
					currCmd->output.value = $2;
					currCmd->output.type = DWORD;
				}
		|
			REDIROUT SWORD
				{
					currCmd->output.value = $2;
					currCmd->output.type = SWORD;
				}
		|
    			/* empty */
    		;

background	:	BACKGROUND
				{
					currCmd->backgrounding = TRUE;
				}
    		|
    			/* empty */
    		;

%%

    
/*
 * yyerror - needed by the parser
 */
yyerror(s)
char	*s;
{
	FreeCmdList(cmd_node);
	cmd_node = NULL;
	ReportError(s, NULL);
}


/*
 * ====================================================================
 * AddToArgList - add a new node to the argument list for this command
 * ====================================================================
 */
static struct arglist *AddToArgList(arg, type)
char	*arg;
int	type;
{
	struct arglist	*p;

	if ( currCmd->args == NULL ) {
		p = currCmd->args = (struct arglist *)
		    xmalloc(sizeof(struct arglist));
		p->arg = arg;
		p->type = type;
		p->next = NULL;
		return(p);
	}
	p = currArg->next = (struct arglist *) xmalloc(sizeof(struct arglist));
	p->arg = arg;
	p->type = type;
	p->next = NULL;
	return(p);
}


/*
 * ====================================================================
 * AddToCmdList - add a new node to the command list
 * ====================================================================
 */
static struct cmd_rec *AddToCmdList(arg, type)
char	*arg;
int 	type;
{
	struct cmd_rec *p;

	if ( cmd_node == NULL ) {
		p = cmd_node = (struct cmd_rec *)
		    xmalloc(sizeof(struct cmd_rec));
		nodeinit(cmd_node);
		cmd_node->cmd.value = arg;
		cmd_node->cmd.type = type;
		return(cmd_node);
	}
	p = currCmd->next = (struct cmd_rec *) xmalloc(sizeof(struct cmd_rec));
	nodeinit(p);
	p->cmd.value = arg;
	p->cmd.type = type;
	return(p);
}


/*
 * ====================================================================
 * nodeinit -  initialize the command node 
 * ====================================================================
 */
static nodeinit(cmd)
    struct cmd_rec *cmd;
{
	cmd->cmd.value = NULL;
	cmd->args = NULL;
	cmd->input.value = NULL;
	cmd->output.value = NULL;
	cmd->backgrounding = FALSE;
	cmd->next = NULL;
}

