
/* 
 * cmdlist.c - routines to fork off process
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Copyright (c) 1988 Victor Norman
 */

/*#define DEBUG*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "xsys.h"

#include "main.h"
#include "parse.h"

extern	char	**environ;


/*
 * ====================================================================
 * ProcessDynamic - handle a dynamically loaded command
 * ====================================================================
 */
ProcessDynamic(cmd, xstdin, xstdout, xstderr)
struct	cmd_rec	*cmd;
int	xstdin, xstdout, xstderr;
{
	int	pid;
	char	**args, **ConvertArgs();
	int	child;
	

#ifdef DEBUG		
	printf("Command before %s\n", cmd->cmd);
#endif


	if (!CheckCommand(cmd))
	    return;
	
#ifdef DEBUG		
	printf("Command after %s\n", cmd->cmd);
#endif

	/* child initialization */
	if ( (args = ConvertArgs(cmd)) == NULL ) {
		ReportError("too many arguments", NULL);
		return;
	}
	
#ifdef DEBUG
	printf("xstdin(%d) xstdout(%d) xstderr(%d)\n",
	       xstdin, xstdout, xstderr);
	printf("priority(%d) command(%s) numargs(%d)\n",
	       xgetprio(xgetpid()), cmd->cmd.value, numargs(args));
#endif
	
	xcontrol(readdtable(0), TCINT, xgetpid());
	xrecvclr(xgetpid());
	child = xxinu_execv(cmd->cmd.value, xgetprio(xgetpid()) - 1 , xstdin,
		xstdout, xstderr, numargs(args), args);
	if ( child == SYSERR ) {
		ReportError("Could not execute:", cmd->cmd.value, NULL);
	        xcontrol(readdtable(0), TCNOINT, xgetpid());
		xclose(xstdin);
		xclose(xstdout);
		xclose(xstderr);
                return;
	}
	if (cmd->backgrounding) {
		fprintf(stdout, BGMSG, child);
		xsetnok(BADPID, child);
		xresume(child);
	}
	else {
		xresume(child);
		if ( xreceive() == INTRMSG ) {
			fprintf(stdout, BGMSG, child);
			xsetnok(BADPID, child);
		}
	}
	xcontrol(readdtable(0), TCNOINT, xgetpid());
}



static char *args[MAXARGS + 1 + 1];
static char env_buff[256];

/*
 * ====================================================================
 * ConvertArgs - convert the linked list of argument strings into an
 * array of pointers to strings that can be used with execve.  Note:
 * the environment must be tacked onto the beginning.
 * ====================================================================
 */
static char **ConvertArgs(cmd)
struct	cmd_rec	*cmd;
{
	/* convert cmd and argslist to null terminated array of pointers
	   to null terminated strings */
	
	struct arglist *ca;
	char	**pa;
	char 	**penviro;
	int i;
	
	pa = args;
	pa++;
	i = 0;
	
	penviro = environ;
	for(; (i < (MAXARGS))&& (*penviro != NULL);) {
		*pa++ = *penviro++;
		i++;
	}
	if ( i >= (MAXARGS) )
	    return(NULL);

	sprintf(env_buff, "%s%d",  MAGIC_ENV_STR, i+1);
	args[0] = env_buff;
		
	*pa++ = cmd->cmd.value;
	
	ca = cmd->args;
	for (; (i < (MAXARGS))&&(ca != NULL); ca = ca->next) {
		*pa++ = ca->arg;
		i++;
	}
	if ( i >= (MAXARGS) )
	    return(NULL);

	*pa = NULL;

#ifdef DEBUG	
	for (pa = args; *pa != NULL; pa++)
	    printf("arg = %s\n", *pa);
#endif	    

	return args;
}
	

/*
 * ====================================================================
 * CheckCommand - before forking any processes, check the command to see
 *		  it can be executed.
 * ====================================================================
 */
static int CheckCommand(cmd)
struct	cmd_rec	*cmd;
{
	char	*FileExists();
	char	*old;

	old = cmd->cmd.value;
	if ((cmd->cmd.value = FileExists(old)) == NULL) {
		ReportError("command not found: ", old, NULL);
		xfree(old);
		return(FALSE);
	}
	xfree(old);
	return(TRUE);
}


