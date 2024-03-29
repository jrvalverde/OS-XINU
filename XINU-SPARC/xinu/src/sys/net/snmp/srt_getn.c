/* srt_getn.c - srt_getn */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <snmp.h>
#include <mib.h>
#include <asn1.h>

/*------------------------------------------------------------------------
 * srt_getn - perform a getnext on a variable in the Routing Table
 *------------------------------------------------------------------------
 */
int srt_getn(bindl, mip, numifaces)
struct snbentry *bindl;
struct mib_info *mip;
int		numifaces;
{
	struct 	route	*rtp, *srt_findnext();
	int		rtl, field, oidi;

	if (srt_match(bindl, &rtp, &rtl, &field) == SYSERR)
		return SERR_NO_SUCH;
	if ((rtp = srt_findnext(rtp, &rtl)) == 0) {
		rtp = (struct route *) NULL;
		rtl = -1;	/* set route hash table list to -1 */
		rtp = srt_findnext(rtp, &rtl);
		if (++field > SNUMF_RTTAB) 
			return (*mip->mi_next->mi_func)
				(bindl, mip->mi_next, SOP_GETF);
	}
	oidi = SRT_OIDLEN;	/* 4.21.1 */

	bindl->sb_oid.id[oidi++] = field;
	sip2ocpy(&bindl->sb_oid.id[oidi], rtp->rt_net, IP_ALEN);
	bindl->sb_oid.len = oidi + IP_ALEN;

	return srt_get(bindl, numifaces);
}
