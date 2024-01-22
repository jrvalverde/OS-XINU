/* ospfinit.c - ospfinit */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <ospf.h>

/*------------------------------------------------------------------------
 * ospfinit - initialize OSPF data structures
 *------------------------------------------------------------------------
 */
ospfinit()
{
	struct ospf_ar	*par;
	int		ifn, arn;

	ospf_lspool = mkpool(sizeof(struct ospf_db), LSABUFS);

	for (arn=0; arn<NAREA; ++arn) {	/* init per-area data */
		par = &ospf_ar[arn];
		db_init(par);
	}
	for (ifn=0; ifn<NIF; ++ifn) {	/* init per-interface data */
		if (ifn == NI_LOCAL)
			continue;
		ospfifinit(0, ifn);
	}
}

struct ospf_ar	ospf_ar[NAREA];
int		ospf_lspool;
