
/* 
 * cat.c - a simple cat program
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Apr 28 14:35:51 1989
 *
 * Copyright (c) 1989 J. Griffioen
 */

#include <stdio.h>			/* unix stdio.h */
#include <sys/file.h>

main(argc, argv)
int argc;
char *argv[];
{
	char	buf[560];
	int	i, j;
	int	fd;
	int	len;

	if ((argc > 1) && (*(argv[1]) == '-')) {
		fprintf(stderr, "usage: %s -h [filenames]\n", argv[0]);
		exit(1);
	}

	if (argc == 1) {
		while ( (len=read(0, buf, 512)) > 0)
			write(1, buf, len);
	}
	for (i=1; i<argc; i++) {
		if ((fd = open(argv[i], O_RDONLY, 0666)) == NULL) {
			fprintf(stderr, "error opening file %s\n", argv[i]);
			exit(1);
		};

		while ((len=read(fd, buf, 512)) > 0)
		    write(1, buf, len);

		close(fd);
	}

}
