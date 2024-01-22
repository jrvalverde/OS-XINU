/* 
 * dummy.c - dummy routines
 * 
 * Author:	
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Wed Jun 27 10:22:56 1990
 *
 * Copyright (c) 1990 
 */

#include <stdio.h>
#include <ctype.h>

#include "main.h"
#include "hash.h"
#include "utils.h"

/*
 * chmod - not implemented
 */
chmod()
{
	fprintf(stdout, "chmod not implemented\n");
	return(0);
}


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
	    	if ( ( dir = getenv("HOME")) == NULL )
		    dir == "";
		putenv(MakeEnvString("PWD", dir));
		return(0);
	}
	
	if ( (pwd = getenv("PWD")) == NULL )
	    pwd = DEFAULTDIR;
	len = strlen(pwd);

	buffer = xmalloc(strlen(dir) + len + 5);
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

/*---------------------------------------------------------------------------
 * sigsetmask - bogus function not implemented
 *---------------------------------------------------------------------------
 */
sigsetmask()
{
	return(0);
}
