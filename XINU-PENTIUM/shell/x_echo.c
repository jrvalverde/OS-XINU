/* x_echo.c - x_echo */
#define ETHERCARD

#include <conf.h>
#include <kernel.h>

#include <mem.h>
#include <network.h>
#include <ee.h>

/*------------------------------------------------------------------------
 *  x_echo  -  (command echo) echo arguments separated by blanks
 *------------------------------------------------------------------------
 */
COMMAND	x_echo(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	char	str[80];
	int	i;

	if (nargs == 1)
		str[0] = NULLCH;
	else {
		for (strcpy(str, args[1]),i=2 ; i<nargs ; i++) {
			strcat(str, " ");
			strcat(str, args[i]);
		}
	}
	strcat(str, "\n");
	write(stdout, str, strlen(str));
	write(TTY0, str, strlen(str));
#ifdef BOOTP
	{
	struct ep *pep, *mkbootp();
	pep = mkbootp(1);
	if ((int)pep !=  SYSERR)
		pdump(pep);
	}
#endif
	return(OK);
}
