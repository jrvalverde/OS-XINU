/* tcpswitch.c */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>

char	*tcperror[] = {
/*  0 */	"no error",
/*  1 */	"unknown error",
/*  2 */	"unknown error",
/*  3 */	"unknown error",
/*  4 */	"unknown error",
/*  5 */	"unknown error",
/*  6 */	"unknown error",
/*  7 */	"unknown error",
/*  8 */	"unknown error",
/*  9 */	"unknown error",
/* 10 */	"connection reset",		/* TCPE_RESET		*/
/* 11 */	"connection refused",		/* TCPE_REFUSED		*/
/* 12 */	"not enough buffer space",	/* TCPE_TOOBIG		*/
/* 13 */	"connection timed out",		/* TCPE_TIMEDOUT	*/
/* 14 */	"urgent data pending",		/* TCPE_URGENTMODE	*/
/* 15 */	"end of urgent data",		/* TCPE_NORMALMODE	*/
	};
/* SEGMENT ARRIVES state processing */

int	tcpclosed(), tcplisten(),tcpsynsent(), tcpsynrcvd(),
	tcpestablished(), tcpfin1(), tcpfin2(), tcpclosewait(),
	tcpclosing(), tcplastack(), tcptimewait();

int	(*tcpswitch[NTCPSTATES])() = {
	ioerr,				/* TCPS_FREE		*/
	tcpclosed,			/* TCPS_CLOSED 		*/
	tcplisten,			/* TCPS_LISTEN 		*/
	tcpsynsent,			/* TCPS_SYNSENT 	*/
	tcpsynrcvd,			/* TCPS_SYNRCVD 	*/
	tcpestablished,			/* TCPS_ESTABLISHED 	*/
	tcpfin1,			/* TCPS_FINWAIT1 	*/
	tcpfin2,			/* TCPS_FINWAIT2 	*/
	tcpclosewait,			/* TCPS_CLOSEWAIT 	*/
	tcplastack,			/* TCPS_LASTACK 	*/
	tcpclosing,			/* TCPS_CLOSING 	*/
	tcptimewait,			/* TCPS_TIMEWAIT 	*/
};
/* Output event processing */

int	tcpidle(), tcppersist(), tcpxmit(), tcprexmt();

int	(*tcposwitch[NTCPOSTATES])() = {
	tcpidle,			/* TCPO_IDLE		*/
	tcppersist,			/* TCPO_PERSIST		*/
	tcpxmit,			/* TCPO_XMIT		*/
	tcprexmt,			/* TCPO_REXMT		*/
};
