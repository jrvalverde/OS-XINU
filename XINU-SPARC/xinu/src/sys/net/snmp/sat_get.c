/* sat_get.c - sat_get */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <snmp.h>
#include <mib.h>
#include <asn1.h>

/*------------------------------------------------------------------------
 * sat_get - do a get on a variable in the Address Translation Table
 *------------------------------------------------------------------------
 */
int sat_get(bindl, numifaces)
struct snbentry	*bindl;
int		numifaces;
{
	int 	iface, entry, field;

	if (sat_match(bindl, &iface, &entry, &field, numifaces) == SYSERR)
		return SERR_NO_SUCH;
	switch(field) {
	case 1:		/* atIfIndex */
		SVTYPE(bindl) = ASN1_INT;
		SVINT(bindl) = iface;
		return SNMP_OK;
	case 2:		/* atPhysAddress */
		SVTYPE(bindl) = ASN1_OCTSTR;
		SVSTR(bindl) = (char *) getmem(EP_ALEN);
		blkcopy(SVSTR(bindl), arptable[entry].ae_hwa,
			EP_ALEN);
		SVSTRLEN(bindl) = EP_ALEN;
		return SNMP_OK;
	case 3:		/* atNetAddress */
		SVTYPE(bindl) = ASN1_IPADDR;
		blkcopy(SVIPADDR(bindl), arptable[entry].ae_pra,
			IP_ALEN);
		return SNMP_OK;
	default:
		break;
	}
	return SERR_NO_SUCH;
}
