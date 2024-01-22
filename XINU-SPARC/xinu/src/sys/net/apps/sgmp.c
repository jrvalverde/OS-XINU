/* sgmp.c - sgmp */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <sgmp.h>

/*------------------------------------------------------------------------
 * sgmp  --  Do Simple Gateway Monitoring Protocol functions
 *------------------------------------------------------------------------
 */
SYSCALL sgmp(op, dest, var, str)
int	op;
IPaddr	dest;
struct	sgmpvt	*var;
char	*str;
{
	if (op == SG_SET || !gateway)
		return OK;
	/* else SG_GET */

	switch(var->sv_type) {
	case STRING:
		sprintf(str, "%s\n", var->sv_value);
		break;
	case INTEGER:
		sprintf(str, "%d\n", *((int *)var->sv_value));
		break;
	default:
		sprintf(str, "Not Implemented\n");
		break;
	}
	return OK;
}

int	nnets;

struct sgmpvt sgmpvar[] = {

	{ "_GW_version_id",	3,	"\01\01\01",
		vers,		0,	STRING },
	{ "_GW_cfg_nnets",	3,	"\01\02\01",
		(char *)&nnets,	sizeof(int),	INTEGER},
#ifdef	notyet
	{ "_GW_net_if_type",	4,	"\01\03\01\03",
		integer
	{ "_GW_net_if_speed",	4,	"\01\03\01\04",
		integer
	{ "_GW_net_if_in_pkts",	5,	"\01\03\01\01\01",
		integer
	{ "_GW_net_if_out_pkts",5,	"\01\03\01\02\01",
		integer
	{ "_GW_net_if_in_bytes",5,	"\01\03\01\01\02",
		integer
	{ "_GW_net_if_out_bytes",5,	"\01\03\01\02\02",
		integer
	{ "_GW_net_if_in_errors",5,	"\01\03\01\01\03",
		integer
	{ "_GW_net_if_out_errors",5,	"\01\03\01\02\03",
		integer
	{ "_GW_net_if_in_qdrops",5,	"\01\03\01\01\04",
		integer
	{ "_GW_net_if_out_qdrops",5,	"\01\03\01\02\04",
		integer
	{ "_GW_net_if_in_qlen",	5,	"\01\03\01\01\05",
		integer
	{ "_GW_net_if_out_qlen",5,	"\01\03\01\02\05",
		integer
	{ "_GW_net_if_status",	4,	"\01\03\01\05",
		integer
	{ "_GW_net_if_mtu",	4,	"\01\03\01\06",
		integer
	{ "_GW_pr_ip_addr_value",5,	"\01\04\01\01\01",
		ipaddr
#endif	notyet
};

int	nsgmpvar = sizeof(sgmpvar)/sizeof(sgmpvar[0]);
