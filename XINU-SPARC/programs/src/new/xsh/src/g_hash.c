/* 
 * g_hash.c - generic hash routines
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jul  5 12:27:59 1990
 *
 * Copyright (c) 1990 Patrick A. Muckelbauer
 */
#include <stdio.h>


#include "main.h"
#include "utils.h"
#include "hash.h"


/*
 * ====================================================================
 * g_hash - hash function
 * ====================================================================
 */
int g_hash(str, size)
char 	*str;
int 	size;
{
	int	i;
	int	t = 0;
	char	*pstr = str;

	for (i = 0; i < strlen(str); i++)
	    t += (int) *pstr++;
	return(t % size);
}

/*
 * ====================================================================
 * g_inserth - inserth a key-value pair into the hash table.  If the key-
 *	     value pair already exists, do nothing.
 * ====================================================================
 */
g_inserth(key, value, hashtbl, size)
char 	*key;
char	*value;
NodePtr	hashtbl[];
int 	size;
{
	int	htindex;
	NodePtr	new_node;
	NodePtr	p;

	/* check if the key value pair exists already		*/
	for (p = hashtbl[htindex = g_hash(key, size)]; p != NULL; p = p->next){
		if ( strequ(p->key, key) ) {
			xfree(p->value);
			p->value = strsave(value);
		 	return;
		}
	}
	new_node = (NodePtr) xmalloc(sizeof(NodeType));
	new_node->key = strsave(key);
	new_node->value = strsave(value);
	new_node->next = hashtbl[htindex];
	hashtbl[htindex] = new_node;
}


/*
 * ====================================================================
 * g_deleteh - delete a key-value pair from the hash table.
 * ====================================================================
 */
g_deleteh(key, hashtbl, size)
char 	*key;
NodePtr	hashtbl[];
int 	size;
{
	int	htindex;
	NodePtr	ptr, prev;

	prev = NULL;
	for (ptr = hashtbl[htindex = g_hash(key, size)];
	     ptr != NULL; prev = ptr, ptr = ptr->next) 
		if (strequ(ptr->key, key))
		    break;

	if (ptr == NULL) 	/* no node found */ 
	    return;
	else			/* found the node */
	    if (prev == NULL)  	/* deleting first node */ 
		hashtbl[htindex] = ptr->next;
	    else 
		prev->next = ptr->next;
	xfree(ptr->key);
	xfree(ptr->value);
	xfree(ptr);
}


/*
 * ====================================================================
 * g_node - find a node
 * ====================================================================
 */
NodePtr g_node(key, hashtbl, size) 
char	*key;
NodePtr	hashtbl[];
int 	size;
{
	NodePtr p;
	char	*t;

	for (p = hashtbl[g_hash(key, size)]; p != NULL; p = p->next)
	    if (strequ(key, p->key)) {
		    return(p);
	    }
	return(NULL);
}


/*
 * ====================================================================
 * g_find - find a name
 * ====================================================================
 */
char *g_find(key, hashtbl, size)
char	*key;
NodePtr	hashtbl[];
int 	size;
{
	NodePtr p;

	if ( (p = g_node(key, hashtbl, size)) == NULL )
	    return(NULL);
	return(p->value);
}


/*
 * ====================================================================
 * g_xfind - find a name.  If none exist return empty string
 * ====================================================================
 */
char *g_xfind(key, hashtbl, size)
char	*key;
NodePtr	hashtbl[];
int 	size;
{
	NodePtr p;

	if ( (p = g_node(key, hashtbl, size)) == NULL )
	    return("");
	return(p->value);
}


/*
 * ====================================================================
 * g_mfind - find an unmarked name.
 * ====================================================================
 */
char *g_mfind(key, hashtbl, size)
char	*key;
NodePtr	hashtbl[];
int 	size;
{
	NodePtr p;

	if ( (p = g_node(key, hashtbl, size)) == NULL )
	    return(NULL);
	
	if ( p->mark == TRUE )
	    return(NULL);
	p->mark = TRUE;
	return(p->value);
}


/*
 * ====================================================================
 * g_inittbl - initialize table.
 * ====================================================================
 */
g_inittbl(hashtbl, size)
     NodePtr	hashtbl[];
     int 	size;
{
	int	i;
	
	for (i = 0; i < size; i++)
	    hashtbl[i] = NULL;
}



/*
 * ====================================================================
 * g_flush - flush table.
 * ====================================================================
 */
g_flush(hashtbl, size)
     NodePtr	hashtbl[];
     int 	size;
{
	NodePtr p, next;
	int	i;

	for (i = 0; i < size; i++) {
		if ((p = hashtbl[i]) != NULL) {
			for (; p != NULL; ) {
				xfree(p->key);
				xfree(p->value);
				next = p->next;
				xfree(p);
				p = next;
			}
		}
		hashtbl[i] = NULL;
	}
}

/*
 * ====================================================================
 * g_unmarktbl - unmark the table.
 * ====================================================================
 */
g_unmarktbl(hashtbl, size)
NodePtr	hashtbl[];
int 	size;
{
	NodePtr p;
	int	i;

	for (i = 0; i < size; i++) {
		if ((p = hashtbl[i]) != NULL) {
			for (; p != NULL; p = p->next)
				p->mark = FALSE;
		}
	}
}



/*
 * ====================================================================
 * g_printtbl - print table.
 * ====================================================================
 */
g_printtbl(hashtbl, size)
NodePtr	hashtbl[];
int 	size;
{
	NodePtr p;
	int	i;

	for (i = 0; i < size; i++) {
		if ((p = hashtbl[i]) != NULL) {
			for (; p != NULL; p = p->next)
				printf("\t%s=%s\n", p->key, p->value);
		}
	}
}


