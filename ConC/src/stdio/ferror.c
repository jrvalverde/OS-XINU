/****************************************/
/*					*/
/*	User's version of ferror	*/
/*					*/
/****************************************/

#include <stdio.h>

ferror(p)
FILE *p;
{
	int ret_val;

	stdio_swait(p);

	ret_val = stdio_ferror(p);

	stdio_ssignal(p);

	return(ret_val);
}
