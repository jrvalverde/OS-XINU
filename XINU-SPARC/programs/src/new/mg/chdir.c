
#ifdef XINU8
#ifndef NO_DIR
#include <stdio.h>
#include <ctype.h>

#define xfree(x)	free(x)
#define NULL		0

extern char *MakeEnvString();

/*
 * chdir - change the current working directory
 */
chdir(dir)
     char *dir;
{
    char *buffer;
    char *pwd;
    int  len;

    if ( dir == NULL ) {
	if ( ( dir = (char *) getenv("HOME")) == NULL )
	    dir == "";
	putenv(MakeEnvString("PWD", dir));
	return(0);
    }

    if ( (pwd = (char *)getenv("PWD")) == NULL )
	pwd = "";
    len = strlen(pwd);

    buffer = (char *) xmalloc(strlen(dir) + len + 5);
    if ( *dir != '/' ) {
	strcpy(buffer, pwd);
	buffer[len] = '/';
	strcpy(buffer+len+1, dir);
    }
    else
	strcpy(buffer, dir);
    CompressDir(buffer);	/* with trailing back slash */
    if ( access(buffer, 0) != 0) {
	xfree(buffer);
	return(-1);
    }
    len = strlen(buffer);
    if ( len > 1 )
	buffer[len-1] = '\0';
    putenv(MakeEnvString("PWD", buffer));
    xfree(buffer);
    return(0);
}


/*
 * CompressDir - compress the path ( process '.' and '..' )
 */
static CompressDir(dir)
     char *dir;
{
    char *old, *new;
    int len;

    if ( *dir != '/' )
	return;
    len = strlen(dir);
    if ( len == 0 || dir[len - 1] != '/' ) {
	dir[len] = '/';
	dir[len+1] = '\0';
    }
    for (old = new = (dir + 1); *old != '\0';new++, old++) {
	*new = *old;
	if ( *old == '/' ) {
	    if ( !strncmp(new-1, "//", 2) ) {
		new = new - 1;
	    }
	    else
		if ( !strncmp(new-3, "/../", 4) ) {
		    new = new - 4;
		    while ( new > dir && *new != '/' )
			new--;
		    if ( new < dir )
			new = dir;
		}
		else
		    if ( !strncmp(new-2, "/./", 3) ) {
			new = new - 2;
		    }
	}
    }
    *new = '\0';
}

/*
 * ====================================================================
 * MakeEnvString - take two strings, key and val, and return a string
 *                 that looks like key=val.
 * ====================================================================
 */
char *MakeEnvString(key, val)
     char    *key;
     char    *val;
{
    char    *newstr;
    int i;

    newstr = (char *) xmalloc(strlen(key) + 1 + strlen(val) + 1);
    strcpy(newstr, key);
    strcat(newstr, "=");

    strcat(newstr, val);
    return newstr;
}
#endif
#endif
