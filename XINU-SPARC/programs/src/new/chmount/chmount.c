/* 
 * chmounts.c - Change the default Xinu mounts
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Mon Dec 11 17:09:03 1989
 *
 * Copyright (c) 1989 Jim Griffioen
 */

#include <sys/xinusys.h>
#include <stdio.h>

/*---------------------------------------------------------------------------
 * This program mounts a prefix on the Remote file system (or namespace)
 *---------------------------------------------------------------------------
 */
main(argc, argv)
int argc;
char *argv[];
{
	if ((argc < 3) || (argc > 4)) {
		printf("usage: %s prefix mountpoint [n]\n", argv[0]);
		exit(1);
	}

	if (argc == 3) {
		xmount(argv[1], xdvlookup(RFILSYS), argv[2]);
		printf("mounted %s on (%s) %s\n", argv[1], RFILSYS, argv[2]);
	}
	else {				/* argc > 3 => mount on namespace */
		xmount(argv[1], xdvlookup(NAMESPACE), argv[2]);
		printf("mounted %s on (%s) %s\n", argv[1], NAMESPACE, argv[2]);
	}
}
