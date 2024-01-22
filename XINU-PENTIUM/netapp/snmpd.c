/* snmpd.c - snmpd */

#include <conf.h>
#include <kernel.h>
#include <network.h>

#ifdef	SNMP

#include <snmp.h>
#include <asn1.h>

/*------------------------------------------------------------------------
 * snmpd - open the SNMP port and handle incoming queries
 *------------------------------------------------------------------------
 */
int snmpd()
{
	int 		snmpdev, len;
	struct xgram	*query;
	struct req_desc	rqd;

	sninit();
	query = (struct xgram *) getmem(sizeof (struct xgram));
	/* open the SNMP server port */
	if ((snmpdev = open(UDP, ANYFPORT, SNMPPORT)) == SYSERR)
		return SYSERR;
	while (TRUE) {
		/*
		 * In this mode, give read the size of xgram, it returns
		 * number of bytes of *data* in xgram.
		 */
		len = read(snmpdev, query, sizeof(struct xgram));
		/* parse the packet into the request desc. structure */
		if (snparse(&rqd, query->xg_data, len) == SYSERR) {
			snfreebl(rqd.bindlf);
			continue;
		}
		/* convert ASN.1 representations to internal forms */
		if (sna2b(&rqd) == SYSERR) {
			snfreebl(rqd.bindlf);
			continue;
		}
		if (snrslv(&rqd) == SYSERR) {
			query->xg_data[rqd.pdutype_pos] = PDU_RESP;
			query->xg_data[rqd.err_stat_pos] = rqd.err_stat;
			query->xg_data[rqd.err_idx_pos] = rqd.err_idx;
			if (write(snmpdev, query, len) == SYSERR)
				return SYSERR;
			snfreebl(rqd.bindlf);
			continue;
		}
		len = mksnmp(&rqd, query->xg_data, PDU_RESP);
		if (len == SYSERR) {
			query->xg_data[rqd.pdutype_pos] = PDU_RESP;
			query->xg_data[rqd.err_stat_pos] = rqd.err_stat;
			query->xg_data[rqd.err_idx_pos] = rqd.err_idx;
			if (write(snmpdev, query, len) == SYSERR)
				return SYSERR;
			snfreebl(rqd.bindlf);
			continue;	
		}
		if (write(snmpdev, query, len) == SYSERR) 
			return SYSERR;
		snfreebl(rqd.bindlf);
	}
}
#endif	/* SNMP */
