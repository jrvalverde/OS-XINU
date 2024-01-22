/* 
 * utils.c 
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 */

#include <stdio.h>
#include <sys/file.h>

#include "parse.h"
#include "utils.h"
#include "command.h"

extern  char	**environ;

struct builtin BuiltIn[] = {	/* bultin commands */
	{ "help", HELP,		"help"				},
	{ "source", SOURCE ,	"source filename"		},
	{ "set", SET, 		"set variable [value]"		},
	{ "unset", UNSET , 	"unset [variable]*"		},
	{ "print", PRINT,	"print [variable]*"		}, 
	{ "setenv", SETENV,	"setenv variable [value]"	},
	{ "unsetenv", UNSETENV,	"unsetenv [variable]*"		}, 
	{ "printenv", PRINTENV,	"printenv [variable]*"		}, 
	{ "export", EXPORT , 	"export [variable]*"		},
	{ "alias", ALIAS ,	"alias [variable [value]]"	},
	{ "unalias", UNALIAS ,	"unalias [variable]*"		},
	
#ifdef BUILTIN_ECHO
	{ "echo", ECHO ,	"echo [variables]*"		},
#endif
	
	{ "cd", CD ,		"cd [path]"			},
	{ "debug", XSH_DEBUG ,	"debug [on | off]"		},
	{ "pwd", PWD ,		"pwd"				}, 
	{ "bye", BYE ,		"bye"				}, 
};


/*
 * ====================================================================
 * MakeOneWord - take the argument list and create one string with words
 *		 separated by a space.
 * ====================================================================
 */
char *MakeOneWord(argl)
struct arglist *argl;
{
	struct arglist *p;
	int	size = 0;
	int	i;
	char	*pc;
	char	*pt;
	char	*t;

	for (p = argl; p != NULL; p = p->next)
		size += strlen(p->arg) + 1;

	if ( size == 0 )
	    return(strsave(""));
	
	t = pt = (char *) xmalloc(size);
	for (p = argl; p != NULL; p = p->next) {
		pc = p->arg;
		for (i = strlen(p->arg); i > 0; i--) 
		    *pt++ = *pc++;
		*pt++ = ' ';
	}
	--pt;
	*pt = '\0';
	return(t);
}

/*
 * ====================================================================
 * strsave - allocate space for a string and copy the string into
 *	     this space and return it.
 * ====================================================================
 */
char *strsave(s)
char *s;
{
	char *t;

	if (s == NULL)
	    return NULL;
	t = xmalloc(strlen(s) + 1);
	strcpy(t, s);
	return t;
}

/*
 * ====================================================================
 * strrepl - replace the substr of s1 from pos to endpos with the
 *           string repl and return the result in res.
 * ====================================================================
 */
char *strrepl(str, start, end, repl)
char	*str, *start, *end, *repl;
{
	char	*pres;
	int	totlen;

#ifdef DEBUG
	fprintf(stdout, "strrepl: str (%s) start(%s) end(%s) repl(%s)\n",
		str, start, end, repl);
#endif
	
	if (repl == NULL) {
		repl = "";
	}
	if (str == NULL) {
		start = end = str = strsave("");
	}

	totlen = strlen(str) - ((end - start) + 1) + strlen(repl);
	
#ifdef DEBUG
	fprintf(stdout, "strrepl: total length = %d\n", totlen);
#endif

	    
	pres = xmalloc(totlen +1);

	*start = '\0';
	strcpy(pres, str);
	strcat(pres, repl);

	strcat(pres, (end+1));
	xfree(str);
	
#ifdef DEBUG
	fprintf(stdout, "strrepl: newstring(%s)\n", pres);
#endif


	return(pres);
}


/*
 * ====================================================================
 * MakeEnvString - take two strings, key and val, and return a string
 *                 that looks like key=val.
 * ====================================================================
 */
char *MakeEnvString(key, val)
char	*key;
char	*val;
{
	char	*newstr;
	int i;

	newstr = xmalloc(strlen(key) + 1 + strlen(val) + 1);
	strcpy(newstr, key);
	strcat(newstr, "=");
	
	strcat(newstr, val);
	return newstr;
}


/*
 * ====================================================================
 * builtin - if c points to a builtin string, return its code number,
 *           else -1.
 * ====================================================================
 */
int builtin(c)
char 	*c;
{
	int	p = 0;

	if ( c == NULL )
	    return(-1);
	while (p < NUMBUILTINS) {
		if (strequ(BuiltIn[p].name, c))
		    return(BuiltIn[p].value);
		p++;
	}
	return -1;
}

/*
 * ====================================================================
 * numargs - number of arguments in the array of character pointers
 *	     (array is null terminated)
 * ====================================================================
 */
int numargs(args)
     char *args[];
{
	int i;
	char **ch;

	if ( args == NULL )
	    return(0);
	
	ch = args;
	i = 0;
	while (ch[i] != NULL) {
		i++;
	}
	return(i);
}



/*
 * ====================================================================
 * transenv - needed for startup; copy environ
 * ====================================================================
 */
void transenv()
{
	char **scan1, **scan2;

	scan1 = environ;
	environ = (char **) xmalloc( (numargs(environ) + 1) * sizeof(char *) );

	for (scan2 = environ; *scan1 != NULL; scan1++, scan2++) {
		*scan2 = strsave(*scan1);
	}
	*scan2 = NULL;
}


/*
 * ====================================================================
 * xgetenv - check return value of xgetenv.
 * ====================================================================
 */
char *xgetenv(name)
     char *name;
{
	char *p;
	
	if ( ( p = getenv(name)) == NULL )
	    return("");
	return(p);
}


/*
 * ====================================================================
 * Help	- process a help built-in command.
 * ====================================================================
 */
Help()
{
	int i,j;

	fprintf(stdout, "GRAMMAR:\n");
	fprintf(stdout, "\tinput line -> command [; command]*\n");
	fprintf(stdout, "\tcommand    -> cmd [argument]* < inputfile > outputfile &\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "BUILTIN COMMANDS:\n");
	for ( j = 0, i = NUMBUILTINS; i > 0 ; i-- , j++)
	    fprintf(stdout, "\t%s\n", BuiltIn[j].usage);
	fprintf(stdout, "\n");
}


/*
 * ====================================================================
 * Usage - print usage message for a built-in command.
 * ====================================================================
 */
Usage(num)
     int num;
{
	
	int	p;

	for (p=0; p < NUMBUILTINS; p++) {
		if (BuiltIn[p].value == num) {
			ReportError("usage:", BuiltIn[p].usage, NULL);
			return;
		}
	}
}
