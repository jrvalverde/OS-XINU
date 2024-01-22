
/* 
 * commands.c - process the built-in commands
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 */

/*#define DEBUG*/

#include <stdio.h>

#include "parse.h"
#include "command.h"
#include "utils.h"

/*
 * ====================================================================
 * ProcessBuiltIn - handle all the dispatching of processing builtin
 * shell commands and error checking.
 * ====================================================================
 */
ProcessBuiltIn(comm_num, cmd)
int	comm_num;
struct cmd_rec *cmd;
{
	extern int Unset(), UnsetEnv(), Export(), Print(), PrintEnv();
	extern int Unalias();
	
#ifdef BUILTIN_ECHO
	extern int Echo();
#endif

	char *p;
	
#ifdef DEBUG	
	printf("processing a built-in command\n");
#endif
	
	switch(comm_num) {
	      case SET :
		    if ( cmd->args == NULL ) {
			    Usage(SET);
			    return;
		    }
		    p = MakeOneWord(cmd->args->next);
		    Set(cmd->args->arg, p);
		    xfree(p);
		    break;
		    
	      case UNSET : 
		    LoopList(Unset, cmd->args);
		    break;
		    
	      case SETENV :
		    if ( cmd->args == NULL ) {
			    Usage(SETENV);
			    return;
		    }
		    p = MakeOneWord(cmd->args->next);
		    SetEnv(cmd->args->arg, p);
		    xfree(p);
		    break;
		    
	      case UNSETENV : 
		    LoopList(UnsetEnv, cmd->args);
		    break;
		    
	      case EXPORT : 
		    LoopList(Export, cmd->args);
		    break;
		    
	      case PRINT :
		    if ( cmd->args == NULL )
			PrintTbl();
		    else
		        LoopList(Print, cmd->args);
		    break;
		    
	      case PRINTENV :
		    if ( cmd->args == NULL )
			PrintEnvTbl();
		    else
		        LoopList(PrintEnv, cmd->args);
		    break;

#ifdef BUILTIN_ECHO
	      case ECHO :
		    LoopList(Echo, cmd->args);
		    fprintf(stdout, "\n");
		    break;
#endif

	      case XSH_DEBUG :
		    if ( cmd->args == NULL ) 
			Xsh_debug(NULL);
		    else if ( cmd->args->next != NULL )
		        Usage(XSH_DEBUG);
		    else 
		        Xsh_debug(cmd->args->arg);
		    break;
		      
	      case CD :
		    if ( cmd->args == NULL )
			Cd(NULL);
		    else if ( cmd->args->next != NULL ) 
		        Usage(CD);
		    else
		        Cd(cmd->args->arg);
		    break;
		    
	      case SOURCE :
		    if ( cmd->args == NULL )
			Usage(SOURCE);
		    else if ( cmd->args->next != NULL )
			Usage(SOURCE);
		    else
		        Source(cmd->args->arg);
		    break;
		    
	      case BYE :
		    exit(0);
	    
	      case  PWD :
		    if ( cmd->args != NULL )
			Usage(PWD);
		    else
		        Pwd();
		    break;
		    
	      case ALIAS :
		    if ( cmd->args == NULL )
			PrintAlias();
		    else if ( cmd->args->next == NULL )
			Alias(cmd->args->arg, NULL);
		    else {
			    p = MakeOneWord(cmd->args->next);
			    Alias(cmd->args->arg, p);
			    xfree(p);
		    }
		    break;
		    
	      case UNALIAS :
		    LoopList(Unalias, cmd->args);
		    break;

	      case HELP :
		    Help();
		    break;

	      default :
		    FatalError("logic error ProcessBultin", NULL);
		    break;
	  }
}


/*
 * ====================================================================
 * LoopList - call the function 'func' for every one of the arguments.
 * ====================================================================
 */
static LoopList(func, args)
     int (*func)();
     struct arglist *args;
{
	for(; args != NULL; args = args->next )
	    func(args->arg);
}
	 
