/* snclient.c - snclient */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <snmp.h>

/*------------------------------------------------------------------------
 * snclient - send an SNMP request and wait for the response
 *------------------------------------------------------------------------
 */
int snclient(rqdp, fport, stdout)
struct req_desc	*rqdp;
char		*fport;
int		stdout;
{
	struct snbentry	*bindl;
	u_char		*buff, reqidsave[10], reqidsavelen;
	int		snmpdev, len;

	buff = (u_char *) getmem( SNMPMAXSZ );
	rqdp->reqidlen = 0;
	rqdp->err_stat = 0;
	rqdp->err_idx = 0;

	if ((len = mksnmp(rqdp, buff, rqdp->reqtype)) == SYSERR)
	    return SCL_BADREQ;
	blkcopy(reqidsave, rqdp->reqid, rqdp->reqidlen);
	reqidsavelen = rqdp->reqidlen;

	/* open the SNMP port and put into data mode */
	if ((snmpdev = open(UDP, fport, ANYLPORT)) == SYSERR ||
	    control(snmpdev, DG_SETMODE, DG_DMODE | DG_TMODE) == SYSERR) {
		close(snmpdev);
		freemem( buff , SNMPMAXSZ );
		return SCL_OPENF;
	}
	if (write(snmpdev, buff, len) == SYSERR) {
		close(snmpdev);
		freemem( buff , SNMPMAXSZ );
		return SCL_WRITEF;
	}
	/* retry once, on timeout */
	if ((len = read(snmpdev, buff, SNMPMAXSZ)) == TIMEOUT)
		len = read(snmpdev, buff, SNMPMAXSZ);
	if (len == TIMEOUT) {
		close(snmpdev);
		freemem( buff , SNMPMAXSZ );
		return SCL_NORESP;
	} else if (len == SYSERR) {
		close(snmpdev);
		freemem( buff , SNMPMAXSZ );
		return SCL_READF;
	}
	if (snparse(rqdp, buff, len) == SYSERR) {
		close(snmpdev);
		freemem( buff , SNMPMAXSZ );
		return SCL_BADRESP;
	}
	if (reqidsavelen != rqdp->reqidlen ||
	    ! blkequ(reqidsave, rqdp->reqid, reqidsavelen)) {
		close(snmpdev);
		freemem( buff , SNMPMAXSZ );
		return SCL_BADRESP;
	}
	/* convert the sb_a1str's to objid's and their values */
	if (sna2b(rqdp) == SYSERR) {
		close(snmpdev);
		freemem( buff , SNMPMAXSZ );
		return SCL_BADRESP;
	}
	close(snmpdev);
	freemem( buff , SNMPMAXSZ );
	return SCL_OK;
}
