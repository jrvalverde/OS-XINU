/****************************************/
/*					*/
/*	User's version of fileno	*/
/*					*/
/****************************************/

#include <stdio.h>

fileno(p)
FILE *p;
{
	int ret_val;

	stdio_swait(p);

	ret_val = stdio_fileno(p);

	stdio_ssignal(p);

	return(ret_val);
}
