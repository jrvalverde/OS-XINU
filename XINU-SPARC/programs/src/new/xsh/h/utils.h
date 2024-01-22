/* 
 * utils.h - utility routines
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Tue Jul 10 12:08:28 1990
 *
 * Copyright (c) 1990 Patrick A. Muckelbauer
 */

extern char *strsave(), *strrepl(), *MakeEnvString(), *MakeOneWord();
extern char *getenv(), *index(), *xgetenv();
extern int  bultin(), numargs();
extern void transenv();

#define strequ(s1, s2) (!strcmp(s1, s2))

extern char *xmalloc(), *xrealloc();
