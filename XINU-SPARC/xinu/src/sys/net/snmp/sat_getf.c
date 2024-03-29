/* sat_getf.c - sat_getf */
    
#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <snmp.h>
#include <mib.h>
#include <asn1.h>
#include <snhash.h>

/*------------------------------------------------------------------------
 * sat_getf - do a getfirst on a variable in the Address Translation Table
 *------------------------------------------------------------------------
 */
int sat_getf(bindl, mip, numifaces)
struct snbentry	*bindl;
struct mib_info	*mip;
int		numifaces;
{
	int 	iface, entry, oidi;
	
	for (iface=1; iface <= numifaces; ++iface)
		if ((entry = sat_findnext(-1, iface)) >= 0)
			break;
	if (iface > numifaces) {  /* no active interface found */
		if (mip->mi_next)
			return (*mip->mi_next->mi_func)
	 			(bindl, mip->mi_next, SOP_GETF);
		return SERR_NO_SUCH;		/* no next node */
	}
	blkcopy(bindl->sb_oid.id, mip->mi_objid.id, mip->mi_objid.len*2);
	oidi = mip->mi_objid.len;

	bindl->sb_oid.id[oidi++] = (u_short) 1;		/* field	*/
	bindl->sb_oid.id[oidi++] = (u_short) iface;
	bindl->sb_oid.id[oidi++] = (u_short) 1;
	sip2ocpy(&bindl->sb_oid.id[oidi], arptable[entry].ae_pra);
	bindl->sb_oid.len = oidi + IP_ALEN;

	return sat_get(bindl, numifaces);
}
