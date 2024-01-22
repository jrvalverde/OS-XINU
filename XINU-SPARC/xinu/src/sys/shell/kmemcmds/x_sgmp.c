/* x_sgmp.c - x_sgmp */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <sgmp.h>

static	int	usage(), help();
static	Bool	initdone;
extern	int	nsgmpvar;

/*------------------------------------------------------------------------
 *  x_sgmp  -  do SGMP get/set operations for a given IP address
 *------------------------------------------------------------------------
 */
COMMAND	x_sgmp(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	Bool	isget;
	char	str[80];
	struct	sgmpvt	*var;
	IPaddr	dest;
	int	i;

	if (nargs < 2)
		return usage(stderr);

	if (!initdone)
		sgmpinit();

	if (strcmp(args[1], "set") == 0)
		isget = FALSE;
	else if (strcmp(args[1], "get") == 0)
		isget = TRUE;
	else if (strcmp(args[1], "help") == 0)
		return help(stdout);
	else
		return usage(stderr);
	if ((isget && nargs != 4) || (!isget && nargs != 5))
		return usage(stderr);
	/* look up the name */

	var = 0;
	for (i=0; i<nsgmpvar; ++i)
		if (strcmp(args[2], sgmpvar[i].sv_name) == 0) {
			var = &sgmpvar[i];
			break;
		}
	if (var == 0) {
		sprintf(str, "sgmp: no such variable\n");
		write(stderr, str, strlen(str));
		return OK;
	}
	/* get the GW addr */

	dot2ip(dest, args[isget ? 3 : 4]);
	if (isget) {
		sgmp(SG_GET, dest, var, str);
		write(stdout, str, strlen(str));
	} else {
		sgmp(SG_SET, dest, var, args[3]);
	}

	return(OK);
}

static	int
help(stdout)
{
	char	str[80];
	int	i;

	sprintf(str, "name\n");
	write(stdout, str, strlen(str));
	for (i=0; i<nsgmpvar; ++i) {
		sprintf(str, "%s\n", sgmpvar[i].sv_name);
		write(stdout, str, strlen(str));
	}
	return OK;
}

static
usage(stderr)
{
	char *str = "usage:\tsgmp get <name> <gateway>\n\tsgmp set <name> <value> <gateway>\n\tsgmp help\n";

	write(stderr, str, strlen(str));
	return OK;
}

sgmpinit()
{
	extern int nnets;
	int	i;

	for (i=0; i<nsgmpvar; ++i)
		if (sgmpvar[i].sv_valsize == 0)
			sgmpvar[i].sv_valsize = strlen(sgmpvar[i].sv_value);
	nnets = Net.nif - 1;
	initdone = TRUE;
}
