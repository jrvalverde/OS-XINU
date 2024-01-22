/* stnew.c - stnew */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <netmon.h>

/*------------------------------------------------------------------------
 *  stnew  -  create a station structure
 *------------------------------------------------------------------------
 */
struct station *
stnew(ifnum, ea)
int	ifnum;
Eaddr	ea;
{
	struct station	*pst;

	pst = (struct station *)getbuf(Netmon.nm_pool);
	if ((int)pst == SYSERR)
		return 0;
	bzero(pst, sizeof(struct station));
	blkcopy(pst->st_ea, ea, EP_ALEN);
	pst->st_ifnum = ifnum;
	return pst;
}
