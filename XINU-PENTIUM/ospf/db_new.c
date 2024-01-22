/* db_new.c - db_new */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <ospf.h>

/*------------------------------------------------------------------------
 *  db_new - create a new topological database entry
 *------------------------------------------------------------------------
 */
struct ospf_db *db_new(par, plsa)
struct ospf_ar	*par;
struct ospf_lsa	*plsa;
{
	struct ospf_db	*pdb;

	pdb = (struct ospf_db *)getbuf(ospf_lspool);
	if (pdb == SYSERR)
		return 0;
	bzero(pdb, sizeof(struct ospf_db));
	pdb->db_lsa = *plsa;	/* structure copy */
	return pdb;
}
