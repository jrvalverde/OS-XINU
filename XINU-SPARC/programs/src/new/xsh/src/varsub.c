/* 
 * varsub.c - routine to do variable substitution on a string
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 */

/*#define DEBUG*/


#include <stdio.h>
#include <ctype.h>

#include "main.h"
#include "hash.h"
#include "utils.h"
#include "parse.h"

extern  char	pidbuff[];
extern 	char 	*TildeExpression();

extern 	char	*VarStart(), *VarReplStr(), *VarEndpos();
extern 	char	*FindVar(), *xFindVar();

static struct cmd_rec *cmd;
static int subs;

/*
 * ====================================================================
 * VarSub - do variable substitution on the string str.  Return a new
 *	      string with the substitutions done and tilde exp done.
 * ====================================================================
 */
char *VarSub(str, cmd_node, psubs)
char	*str;
struct cmd_rec *cmd_node;
int *psubs;
{
	char *start, *end, *repl;
	int diff;

	cmd = cmd_node;
	subs = FALSE;
	if ( *str == '$' && (FindVar(str+1) == NULL) ) {
		if ( subs == TRUE ) {
			if ( psubs != NULL )
			    *psubs = TRUE;
		    	return(NULL);
		}
	}
	cmd = cmd_node;
	subs = FALSE;
	start = str;
	while ( (start = VarStart(start)) != NULL ) {
		end = VarEndpos(start);
	
		diff = start - str;		
		repl = VarReplStr(start, end);
		diff += strlen(repl);
		str  = strrepl(str, start, end, repl);
		start = str + diff;
	}
	if ( psubs != NULL )
	    *psubs = subs;
	return(TildeExpression(str));
}

/*
 * ====================================================================
 * VarStart - find the start of the variable
 * ====================================================================
 */
static char *VarStart(str)
char	*str;     
{
	char *scan;

	for (scan = str; *scan != '\0'; scan++)
	    if ( *scan == '$' ) {
		    scan++;
		    if ( *scan == '\0' )
			return(NULL);
		    if ( isalnum(*scan) || *scan == '$' )
			return(--scan);
		    if ( *scan == '-' &&
			 (*(scan+1) == '1' || *(scan+1) == '2') )
			return(--scan);
	    }
	return(NULL);
}

/*
 * ====================================================================
 * VarEndpos - find the end of the variable
 * ====================================================================
 */
static char *VarEndpos(str)
char	*str;     
{
	char *scan;
	
	scan = str+1;
	if ( *scan == '$' )
	    return(scan);
	if ( *scan == '-' )
	    return(scan+1);
	if ( isdigit(*scan) ) {
		for(; *scan != '\0'; scan++)
		    if ( !isdigit(*scan) ) 
			    return(--scan);
	}
	else {
		for(; *scan != '\0'; scan++)
		    if ( !isalnum(*scan) ) 
			    return(--scan);
	}
	return(--scan);
}


/*
 * ====================================================================
 * VarReplStr - replace the variable
 * ====================================================================
 */
static char *VarReplStr(start, end)
char   *start, *end;
{
	char *repl;
	int  holder;
	
	start = start + 1;
	end = end + 1;

	if ( *start == '$' ) {	/* get shellpid */
		return(pidbuff);
	}
	
	holder = *end;
	*end = '\0';
	repl = xFindVar(start);
	*end = holder;
	return(repl);
}
		   
	
/*
 * ====================================================================
 * TildeExpansion - do tilde expansion on a string.
 * ====================================================================
 */
static char *TildeExpression(str)
char	*str;
{

#ifdef DEBUG
	fprintf(stdout, "TildeExpression: str(%s)\n", str);
#endif

	if (*str == '~') {
		if (str[1] == '\0' || str[1] == '/') 
		    return(strrepl(str, str, str, getenv("HOME")));
		else
		    return(strrepl(str, str, str, DEFAULTDIR));
	}
	else
	    return (str);
}

/*
 * ====================================================================
 * FindVar - find the replacement string
 * ====================================================================
 */
static char *FindVar(key)
     char *key;
{
	char *ans, num;
	struct arglist *args;

	if ( strequ(key, "-1") ) {
		subs = TRUE;
		if ( cmd == NULL )
		    return(NULL);
		return(cmd->input.value);
	}
	if ( strequ(key, "-2") ) {
		subs = TRUE;
		if ( cmd == NULL )
		    return(NULL);
		return(cmd->output.value);
	}	
	if ( (num = Positive(key)) >= 0 ) {
		subs = TRUE;
		if ( cmd == NULL )
		    return(NULL);
		if ( num == 0 )
		    return(cmd->cmd.value);
		num--;
		for ( args = cmd->args; num > 0 && args != NULL;
		     num--, args = args->next)
		    /* null statement*/;
		if ( args == NULL ) 
		    return(NULL);
		else
		    return(args->arg);
	}
	if ( (ans = v_find(key)) == NULL ) {
		if ( (ans = getenv(key)) == NULL )
		    return(NULL);
	}
	return(ans);
}


/*
 * ====================================================================
 * xFindVar - check return value for  FindVar().
 * ====================================================================
 */
static char *xFindVar(key)
     char *key;
{
	char *ret;
	
	if ( (ret = FindVar(key)) == NULL )
	    return("");
	return(ret);
}


/*
 * ====================================================================
 * Positive - is the string 'str' a positive number.
 * ====================================================================
 */
static int Positive(str)
     char *str;
{
	int num;
	char *scan;
	
	if ( *str == '\0' )
	    return(-1);
	num = 0;
	for ( scan = str; *scan != '\0'; scan++) {
	    	if ( !isdigit(*scan) )
			return(-1);
		num = (num*10) + (*scan - '0') + num;
	}
	return(num);
}
	
