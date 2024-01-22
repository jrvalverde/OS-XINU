/* perror.c - perror() */

#include <stdio.h>

extern int errno;

perror(sb)
char *sb;
{
	if (sb != NULL)
	    fprintf(stderr, "%s: errno = %d\n", sb, errno);
	else
	    fprintf(stderr, "perror(): errno = %d\n", sb, errno);
}
    
