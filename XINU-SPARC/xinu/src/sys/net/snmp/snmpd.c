/* snmpd.c - snmpd */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <snmp.h>
#include <asn1.h>

/*#define DEBUG*/

/*------------------------------------------------------------------------
 * snmpd - open the SNMP port and handle incoming queries
 *------------------------------------------------------------------------
 */
int snmpd()
{
	int 		snmpdev, len;
	struct xgram	*query;
	struct req_desc	rqd;
	int rv;

	sninit();
	query = (struct xgram *) getmem(sizeof (struct xgram));
	/* open the SNMP server port */
	if ((snmpdev = open(UDP, ANYFPORT, SNMPPORT)) == SYSERR)
		return SYSERR;

	control( snmpdev , DG_SETMODE , DG_NMODE );
	
	while (TRUE) {
		/*
		 * In this mode, give read the size of xgram, it returns
		 * number of bytes of *data* in xgram.
		 */
		len = read(snmpdev, query, sizeof(struct xgram));
		
#ifdef DEBUG
		kprintf("snmpd(): got packet length (%d)\n", len);
		kprintf("\txgram->xg_fip = %d.%d.%d.%d\n",
			(unsigned char) query->xg_fip[0],
			(unsigned char) query->xg_fip[1],
			(unsigned char) query->xg_fip[2],
			(unsigned char) query->xg_fip[3] );
		kprintf("\txgram->xg_fport = %d\n", query->xg_fport);
		kprintf("\txgram->xg_lport = %d\n", query->xg_lport);
#endif
		
		/* parse the packet into the request desc. structure */
		if (snparse(&rqd, query->xg_data, len) == SYSERR) {
		    
#ifdef DEBUG
			kprintf("snmpd(): snparse() return error\n");
#endif
		
			snfreebl(rqd.bindlf);
			continue;
		}
		/* convert ASN.1 representations to internal forms */
		if (sna2b(&rqd) == SYSERR) {
		    
#ifdef DEBUG
			kprintf("snmpd(): sna2b() return error\n");
#endif
		
			snfreebl(rqd.bindlf);
			continue;
		}
		if (snrslv(&rqd) == SYSERR) {
		    
#ifdef DEBUG
			kprintf("snmpd(): snrslv() return error\n");
#endif
		
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
		    
#ifdef DEBUG
			kprintf("snmpd(): mksnmp() return error\n");
#endif
		
			query->xg_data[rqd.pdutype_pos] = PDU_RESP;
			query->xg_data[rqd.err_stat_pos] = rqd.err_stat;
			query->xg_data[rqd.err_idx_pos] = rqd.err_idx;
			if (write(snmpdev, query, len) == SYSERR)
				return SYSERR;
			snfreebl(rqd.bindlf);
			continue;	
		}
		
#ifdef DEBUG
		kprintf("snmpd(): send reply length (%d)\n", len);
		kprintf("\txgram->xg_fip = %d.%d.%d.%d\n",
			(unsigned char) query->xg_fip[0],
			(unsigned char) query->xg_fip[1],
			(unsigned char) query->xg_fip[2],
			(unsigned char) query->xg_fip[3] );
		kprintf("\txgram->xg_fport = %d\n", query->xg_fport);
		kprintf("\txgram->xg_lport = %d\n", query->xg_lport);
#endif
		
		if ( ( rv = write(snmpdev, query, len) ) == SYSERR) {
		    
#ifdef DEBUG
			kprintf("snmpd(): write() return error\n");
#endif
		
			return SYSERR;
		}
		
#ifdef DEBUG
		kprintf("snmpd(): write() return %d\n" , rv );
#endif
		
		snfreebl(rqd.bindlf);
	}
}
