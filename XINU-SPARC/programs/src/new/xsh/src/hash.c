
/* 
 * hash.c - allocate hash table variables
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 *
 */

#include <stdio.h>
#include "hash.h"

NodePtr	vartbl[v_HTSize];		/* variable hash table */
NodePtr aliastbl[a_HTSize];		/* aliasing hash table */
NodePtr cachetbl[c_HTSize];		/* command cache table */
