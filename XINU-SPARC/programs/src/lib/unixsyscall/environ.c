/* environ.c - declare space for the environment variable */

#include "unixkernel.h"

char *envspace[MAXENVIRON];		/* the unix environment variable */
					/* initialize in unix startup	*/
char **environ=envspace;
