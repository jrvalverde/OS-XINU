/* 
 * hash.h - hash table for Oracle header file
 * 
 * Author:	Victor Norman
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Mon Nov  7 16:31:35 1988
 *
 * Copyright (c) 1988 Victor Norman
 */

typedef struct nodel {
	char 	*key;
	char	*value;
	int	mark;
	struct	nodel 	*next;
} NodeType;

typedef NodeType *NodePtr;

#define v_HTSize	53
#define a_HTSize	53
#define c_HTSize	53

/* macros for variable hash routines */
#define v_inittbl()		g_inittbl(vartbl, v_HTSize)
#define v_insert(key, value)	g_inserth(key, value, vartbl, v_HTSize)
#define v_delete(key)		g_deleteh(key, vartbl, v_HTSize)
#define v_find(key)		g_find(key, vartbl, v_HTSize)
#define v_xfind(key)		g_xfind(key, vartbl, v_HTSize)
#define v_node(key)		g_node(key, vartbl, v_HTSize)
#define v_mfind(key)		g_mfind(key, vartbl, v_HTSize)
#define v_unmarktbl(key)	g_unmarktbl(vartbl, v_HTSize)
#define v_printtbl()		g_printtbl(vartbl, v_HTSize)
#define v_flush()		g_flush(vartbl, v_HTSize)

/* macros for alias hash routines */
#define a_inittbl()		g_inittbl(aliastbl, a_HTSize)
#define a_insert(key, value)	g_inserth(key, value, aliastbl, a_HTSize)
#define a_delete(key)		g_deleteh(key, aliastbl, a_HTSize)
#define a_find(key)		g_find(key, aliastbl, a_HTSize)
#define a_xfind(key)		g_xfind(key, aliastbl, a_HTSize)
#define a_node(key)		g_node(key, aliastbl, a_HTSize)
#define a_mfind(key)		g_mfind(key, aliastbl, a_HTSize)
#define a_unmarktbl(key)	g_unmarktbl(aliastbl, a_HTSize)
#define a_printtbl()		g_printtbl(aliastbl, a_HTSize)
#define a_flush()		g_flush(aliastbl, a_HTSize)

/* macros for cache hash routines */
#define c_inittbl()		g_inittbl(cachetbl, c_HTSize)
#define c_insert(key, value)	g_inserth(key, value, cachetbl, c_HTSize)
#define c_delete(key)		g_deleteh(key, cachetbl, c_HTSize)
#define c_find(key)		g_find(key, cachetbl, c_HTSize)
#define c_xfind(key)		g_xfind(key, cachetbl, c_HTSize)
#define c_node(key)		g_node(key, cachetbl, c_HTSize)
#define c_mfind(key)		g_mfind(key, cachetbl, c_HTSize)
#define c_unmarktbl(key)	g_unmarktbl(cachetbl, c_HTSize)
#define c_printtbl()		g_printtbl(cachetbl, c_HTSize)
#define c_flush()		g_flush(cachetbl, c_HTSize)

extern int g_hash();
extern char *g_find(), *g_xfind(), *g_mfind();
extern NodePtr g_node();

extern NodePtr	vartbl[];	/* varialbe table 	*/
extern NodePtr  aliastbl[];	/* alias table	  	*/
extern NodePtr  cachetbl[];	/* cache command table 	*/
