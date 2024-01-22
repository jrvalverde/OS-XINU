/* cp.c - cp */

#include <stdio.h>
#include <fcntl.h>

static	char	errfmt[] = "Cannot open %s\n";

/*------------------------------------------------------------------------
 *  cp  -  (xinu dynamic copy command) copy one file to another
 *------------------------------------------------------------------------
 */
main(nargs, args)
int	nargs;
char	*args[];
{
	char	*buf;
	int	from, to;
	int	ret;
	int	len;

	if (nargs != 3) {
		fprintf(stderr, "usage: cp file1 file2\n");
		exit(1);
	}
	if ( (from=open(args[1], O_RDONLY)) < 0) {
		fprintf(stderr, errfmt, args[1]);
		exit(1);
	}
	if ( (to=open(args[2], O_CREAT | O_TRUNC | O_WRONLY, 0666)) < 0) {
		close(from);
		fprintf(stderr, errfmt, args[2]);
		exit(1);
	}
	if ( ((int) (buf = (char *)malloc(512)) ) == (int)NULL) {
		fprintf(stderr, "no memory\n");
		close(from);
		close(to);
		exit(1);
	} else {
		while ( (len = read(from, buf, 512)) > 0 )
			write(to, buf, len);
		free(buf, 512);
	}
	close(from);
	close(to);
}
