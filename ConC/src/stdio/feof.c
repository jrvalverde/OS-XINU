/****************************************/
/*					*/
/*	User's version of feof		*/
/*					*/
/****************************************/

#include <stdio.h>

feof(p)
FILE *p;
{
	int ret_val;

	stdio_swait(p);

	ret_val = stdio_feof(p);

	stdio_ssignal(p);

	return(ret_val);
}
