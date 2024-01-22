
/* 
 * params.c - test argc, argv
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Mon Dec 11 17:09:03 1989
 *
 * Copyright (c) 1989 Jim Griffioen
 */

#include <stdio.h>

main(argc, argv)
int argc;
char *argv[];
{
	int i;

	printf("argc = %d\n", argc);
	for (i=0; i<argc; i++) {
		printf("argv[%d] = %s\n", i, argv[i]);
	}
}
