/* network.h */

/* All includes needed for the network */

#include <deqna.h>
#include <ether.h>
#include <ip.h>
#include <icmp.h>
#include <udp.h>
#include <net.h>
#include <dgram.h>
#include <arp.h>
#include <fserver.h>
#include <rfile.h>
#include <domain.h>

/* Declarations data conversion and checksum routines */

extern	short		hs2net();	/* host to network short	*/
extern	short		net2hs();	/* network to host short	*/
extern	long		hl2net();	/* host to network long		*/
extern	long		net2hl();	/* network to host long		*/
extern	short		cksum();	/* 1s comp of 16-bit 2s comp sum*/

#ifndef	INTERNET
#define	INTERNET ""
#endif
#ifndef Ndg
#define Ndg	1
#endif
#ifndef	ETHER
#define	ETHER	SYSERR
#endif
