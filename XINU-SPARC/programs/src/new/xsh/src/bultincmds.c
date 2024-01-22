/* 
 * bultincmds.c - process the builtin commands
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 */

/*#define DEBUG*/

#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>

#include "xsys.h"

#include "main.h"
#include "command.h"
#include "utils.h"
#include "hash.h"

extern	char	**environ;
int source_num;
extern int xsh_debug;
    

/*
 * ====================================================================
 * UnsetEnv - process unsetenv built-in command calls.  Do this by,
 * removing the entry in environ and shifting all the others up one.
 * ====================================================================
 */
UnsetEnv(key)
char *key;
{
	char	**p;
	int	i, len;
	char	*newkey;

	if (getenv(key) != NULL) {
		len = strlen(key);
		for (p = environ, i = 0; *p != NULL; p++, i++)
		    if ((!strncmp(key, *p, len)) && ((*(*p+len)) == '=' ) )
			break;
		
		/* i is the count of where var is found */
		for (p = environ + i; *p != NULL; i++, p++) 
		    environ[i] = environ[i+1];
		
		if (strequ(key, "PATH")) {
		    	v_delete("path");
			c_flush();
		}
		else if (strequ(key, "TERM")) {
			v_delete("term");
			UnsetEnv("TERMCAP");
			
			rl_reset_terminal("dumb");
			
		}
		else if (strequ(key, "USER")) {
			v_delete("user");
		}
	}	
}


/*
 * ====================================================================
 * SetEnv - process setenv built-in command.  Insert the key/val pair
 *          into the environment. 
 * ====================================================================
 */
SetEnv(key, val)
char *key, *val;
{
	char	*s;
	

	if ( !isalpha(*key) ) {
		ReportError("variable must start with a letter:", key, NULL);
		return;
    	}
	for ( s = key; *s != '\0'; s++ )
	    if ( !isalnum(*s) ) {
		    ReportError("variable must be alphanumeric:", key, NULL);
		    return;
	    }

	putenv(MakeEnvString(key, val));
	
	if (strequ(key, "PATH")) {
		v_insert("path", val);
		c_flush();
	}
	else if (strequ(key, "TERM")) {
		v_insert("term", val);
		UnsetEnv("TERMCAP");
		rl_reset_terminal(val);
	}
	else if (strequ(s, "USER")) {
		v_insert("user", val);
	}
}


/*
 * ====================================================================
 * Export - process an export built-in command.  Export the local variable 
 *	    to the environment.
 * ====================================================================
 */
Export(key)
char *key;
{
	char	*val;
	char	*newstr;
	char 	*s;

	SetEnv(key, v_xfind(key));
}


/*
 * ====================================================================
 * Unset - process an unset built-in command.  Unset the local variable.
 * ====================================================================
 */
Unset(key)
char *key;
{
	if (strequ(key, "path")) {
	    	UnsetEnv("PATH");
	}
	else if (strequ(key, "term")) {
		UnsetEnv("TERM");
	}
	else if (strequ(key, "user")) {
		UnsetEnv("USER");
	}
	else {
		v_delete(key);
 	}
}


/*
 * ====================================================================
 * Set - process a set built-in command.  Set the local variable.
 * ====================================================================
 */
Set(key, val)
     char *key, *val;
{
	char *s;
	
	if ( strequ(key, "path") ) {
		SetEnv("PATH", val);
		return;
	}
	else if ( strequ(key, "term") ) {
		SetEnv("TERM", val);
		return;
	}
	else if ( strequ(key, "user") ) {
		SetEnv("USER", val);
		return;
	}
	if ( !isalpha(*key) ) {
		ReportError("variable must start with a letter:", key, NULL);
		return;
	}
	for (s=key ; *s != '\0'; s++ )
	    if ( !isalnum(*s) ) {
		    ReportError("variable must be letters and/or numbers:",
				key, NULL);
	    	    return;
    	    }
	v_insert(key, val);
}


/*
 * ====================================================================
 * PrintEnv - process a printenv built-in command.  Print the environment
 *	      variable.
 * ====================================================================
 */
PrintEnv(name)
     char *name;
{
	fprintf(stdout, "%s\n",	xgetenv(name));
}

/*
 * ====================================================================
 * PrintEnvTbl - process a printenv built-in command.  Print the environment
 *	         table.
 * ====================================================================
 */
PrintEnvTbl()
{
	char **ch;
	
     	fprintf(stdout, "ENVIRONMENT VARIABLES\n");
	fprintf(stdout, "---------------------\n");
	for(ch = environ; *ch != NULL; ch++) {
		fprintf(stdout, "\t%s\n", *ch);
	}
	fprintf(stdout, "\n");
}


/*
 * ====================================================================
 * Print - process a print built-in command.  Print the local variable.
 * ====================================================================
 */
Print(name)
     char *name;
{
	fprintf(stdout, "%s\n",	v_xfind(name));
}


/*
 * ====================================================================
 * PrintTbl - process a print built-in command.  Print the local variables
 *            table.
 * ====================================================================
 */
PrintTbl()
{
	fprintf(stdout, "LOCAL VARIABLES\n");
	fprintf(stdout, "---------------\n");
	v_printtbl();
	fprintf(stdout, "\n");
}


/*
 * ====================================================================
 * Source - process a source built-incommand.  Open file and call doit.
 *	    Also checks if max sources is reached.
 * ====================================================================
 */
Source(filename)
     char *filename;
{
	FILE	*fopen(), *fp;
	extern int source_num;

	if ((fp = fopen(filename, "r")) == NULL) {
		ReportError("couldn't open source file:", filename, NULL);
		return;
	}
	if ( ( source_num++) > MAXSOURCE ) {
		fclose(fp);
		FatalError("Source()...recursive source detected??", NULL);
	}
	doit(fp);
	fclose(fp);
	source_num--;
}


/*
 * ====================================================================
 * Alias - process an alias built-in command.  Insert the alias into the
 *	   alias table.
 * ====================================================================
 */
Alias(key, val)
    char *key, *val;
{
	char	*p;

	if ( val == NULL ) {
		if ( (p = a_find(key)) != NULL )
		    fprintf(stdout, "%s\n", p);
		else
		    fprintf(stdout, "no alias for %s\n", key);
	}
	else {
		a_insert(key, val);
	}
}


/*
 * ====================================================================
 * PrintAlias - process an alias built-in command.  Print the alias table.
 * ====================================================================
 */
PrintAlias()
{
	a_printtbl();
	fprintf(stdout, "\n");
}


/*
 * ====================================================================
 * Unalias - process an unalias built-in commad.  Unalias the key.
 * ====================================================================
 */
Unalias(key)
char *key;
{
	a_delete(key);
}


#ifdef BUILTIN_ECHO
/*
 * ====================================================================
 * Echo - process an echo built-in command.
 * ====================================================================
 */
Echo(str)
     char *str;
{
	fprintf(stdout, "%s ", str);
}
#endif


/*
 * ====================================================================
 * Xsh_debug - process a debug built-in command.  Turns the debug variable
 	       on or off.
 * ====================================================================
 */
Xsh_debug(name)
     char *name;
{
	if ( name == NULL ) {
	 	xsh_debug = TRUE;
		fprintf(stdout, "debugging is on\n");
	}
	else
	if ( strequ(name, "off") ) {
		xsh_debug = FALSE;
		fprintf(stdout, "debugging is off\n");
	}
	else
	if ( strequ(name, "on") ) {
		xsh_debug = TRUE;
		fprintf(stdout, "debugging is on\n");
	}
	else
	    Usage(XSH_DEBUG);
}


/*
 * ====================================================================
 * Pwd - process a pwd built-in command.  Print the PWD environment
 	 variable.
 * ====================================================================
 */
Pwd()
{
	char *ch;

	if ( (ch = getenv("PWD")) == NULL ) {
		fprintf(stdout, "no current directory\n");
	}
	else {
		fprintf(stdout, "%s\n", ch);
	}
}


/*
 * ====================================================================
 * Cd - process a cd built-in command.
 * ====================================================================
 */
Cd(path)
     char *path;
{
    	if ( chdir(path) < 0 ) {
	    	ReportError("bad directory:", path, NULL);
		return;
	}
	c_flush();
	return;
}


/*
 * ====================================================================
 * Help - routine found in utils.c 
 * ====================================================================
 */

