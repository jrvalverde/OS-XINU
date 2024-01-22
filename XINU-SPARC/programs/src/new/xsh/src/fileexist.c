
/* 
 * fileexist
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Wed Nov 16 08:28:35 1988
 *
 */

/*#define DEBUG*/
#define NEG_CACHEING

#include <stdio.h>
#include <ctype.h>
#include <sys/file.h>

#include "utils.h"
#include "hash.h"


#define deliminator(ch) ((ch) == ':' || isspace(ch))

/*
 * ====================================================================
 * FileExists - check the path to see if query_file_name executable
 * 		file is found in it; if so, return absolute pathname,
 *		else NULL.  Cache result in table.
 * ====================================================================
 */
char *FileExists(query_file_name)
char *query_file_name;
{
	char *path, *name, *pname, *qname;
	NodePtr node;

#ifdef DEBUG
	printf("FileExists for '%s' ?\n", query_file_name);
#endif
	if ( *query_file_name == '/' )
	    return(strsave(query_file_name));
	
	if ((path = getenv("PATH")) == NULL) {
	    	return(NULL);
	}

	if ( ( node = c_node(query_file_name)) != NULL ) {
#ifdef DEBUG
	printf("Cache Hit for '%s' '%s'\n", query_file_name, node->value);
#endif
		if ( node->value == NULL )
		    return(NULL);
		return(strsave(node->value));
	}

	name = (char *)xmalloc(strlen(path)+2+strlen(query_file_name));

	/* parse path */
	while( *path != '\0' ) {
		for(pname = name; *path != '\0' && !deliminator(*path) ; ) 
		    *pname++ = *path++;
		while(*path != '\0' && deliminator(*path))
		    path++;
		if ( *(pname-1) != '/' )
		    *pname++ = '/';
		for(qname = query_file_name; *qname != '\0'; )
		    *pname++ = *qname++;
		*pname = '\0';
		if (access(name, 0) == 0) {  /* check if file exists */
			c_insert(query_file_name, name);
			return(name);
		}
	}
	xfree(name);
#ifdef NEG_CACHEING	
	c_insert(query_file_name, NULL);
#endif
	return(NULL);
}
