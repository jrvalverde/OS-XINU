/* getenv.c - getenv() */

#include <sys/xinusys.h>

extern char *kgetenv();
extern char **environ;

/*---------------------------------------------------------------------------
 * getenv - return a pointer to the requested environment variable
 *---------------------------------------------------------------------------
 */
char *getenv(name)
char *name;
{
	return(kgetenv(name));
}


/*---------------------------------------------------------------------------
 * putenv - add a string to the environment
 *---------------------------------------------------------------------------
 */
putenv(str)
     char *str;
{
	unsigned int nbytes, findblklen();
	char *equal, **scan;
	int len, nspaces, i;

	nbytes = findblklen(environ);

	nspaces = ( nbytes / sizeof(char *) ) - 1;

	for(len=1, equal=str; *equal != '\0' &&
	    *equal != '='; equal++, len++) ;
	if ( len == 1 || *equal != '=' ) {
		return(-1);
	}
	for ( i=0, scan = environ; *scan != NULL; scan++, i++ ) {
		if ( !strncmp(*scan, str, len) ) {
			free(*scan);
			*scan = str;
			return(0);
		}
	}
	if ( i >= nspaces ) {
		if ( ( equal=(char *)realloc(environ, nbytes * 2)) == NULL ) {
			return(-1);
		}
		environ = (char **) equal;
	}
	scan = environ + i;
	*scan++ = str;
	*scan = NULL;
	return(0);
}

