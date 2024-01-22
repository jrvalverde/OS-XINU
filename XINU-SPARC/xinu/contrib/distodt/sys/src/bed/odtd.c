/* 
 * odtd.c - This is the odt daemon forked by the 'bed' process
 * 
 * Author:	Shawn Oostermann/Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Jun  3 14:14:53 1988
 *
 * Copyright (c) 1988 Shawn Oostermann/Jim Griffioen
 */
#include <stdio.h>
#include <errno.h>
#include <ctype.h>

#ifdef HPPA
#include <termios.h>
#endif

#include <sgtty.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <netinet/in.h>

#include "bed.h"
#include "bedata.h"
#include "bmutils.h" 
#include "netutils.h"
#include "utils.h"
#include "odtd.h"
#include "baud.h"

#define BUFSIZE 100
#define SECS_TO_CONNECT 10

struct	sgttyb	Lttymode;		/* line tty mode upon entry	*/
struct	sgttyb	lttymode;		/* line tty mode for gtty/stty	*/
struct option opt;
int errno;	        		/* error number when system err	*/


/*---------------------------------------------------------------------------
 * odtd - odtd daemon forked off by the BED process
 *---------------------------------------------------------------------------
 */
main(argc, argv)
     int argc;
     char **argv;
{
	int fdTCP, devfd;
	struct sockaddr_in sa;
	char mesg[ MAXDETAIL ];
	int addrlen = sizeof( struct sockaddr_in );
	int Quit();
	int Timeout();
	int fdTcp;
	struct bedata *beconfig, bedata;
	int i;
	char **ptr;
	
#ifdef DEBUG
	opt.verbose = TRUE;
#else
	opt.verbose = FALSE;
#endif
	opt.logtype = ALOGFILE;
	
	beconfig = &bedata;
	fdTcp = atoi( argv[ 1 ] );
	
	ptr = (char **) &( beconfig->data );
	for( i = 0; i < NUM_BED_DATA; i++, ptr++ ) {
		*ptr = argv[ i + 2 ];
	}
	
	signal( SIGALRM, Quit );
	
	settimer( SECS_TO_CONNECT );
	fdTCP = accept( fdTcp, &sa, &addrlen );
	canceltimer();
	
	sprintf( mesg,"Using %s%c%c", beconfig->data.ttyname, '\015',
		'\012' );
	write( fdTCP, mesg, strlen( mesg ) );
	
	devfd = SetTtyLine( beconfig->data.ttyname, beconfig->data.ttyrate );
	
	TCPxTTY( devfd, fdTCP, fdTcp );
	
	quit();
}

/*---------------------------------------------------------------------------
 * 
 *---------------------------------------------------------------------------
 */
static 
SetTtyLine( dev, baudrate )
     char *dev;
     char *baudrate;
{
	int devfd;
	
	VERBOSE(Log( "set tty line %s with baudrate %d", dev, baudrate ));
	if ( ( devfd = open( dev, O_RDWR, 0 ) ) < 0 ) {
		Error( "error: cannot open %s", dev );
	}
	if ( ioctl( devfd, TIOCGETP, &Lttymode ) < 0 )  {
		Error( "error: cannot get tty mode\n" );
	}
	lttymode = Lttymode;
	
	{
		int baudindex;
		
		baudindex = lookupBaudRate( baudrate );
		if ( baudindex != NOBAUD ) {
			lttymode.sg_ispeed = baudRate( baudindex );
			lttymode.sg_ospeed = baudRate( baudindex );
		}
	}
	
	lttymode.sg_flags |= RAW;
	lttymode.sg_flags &= ~ECHO;
/*	lttymode.sg_flags |= TANDEM;*/
	
	if ( ioctl( devfd, TIOCSETP, &lttymode ) < 0 ) {
		Error( "error: cannot set tty mode\n" );
	}
	return( devfd );
}

/*---------------------------------------------------------------------------
 * 
 *---------------------------------------------------------------------------
 */
static
TCPxTTY( fdtty, fdtcp, fdListen )
     int fdtty;
     int fdtcp;
     int fdListen;
{
	int bmaskRead;
	int bmaskResultR;
	int zero = 0;
	int len;
	char buf[BUFSIZE + 1];
	int i;
	int esc;
	
	VERBOSE(Log( "TCPxTTY..." ));
	bmaskRead = BITMASK( fdtty ) | BITMASK( fdtcp );
	esc = FALSE;
	
	for (;;) {
		
		bmaskResultR = bmaskRead;
		do {
			i = select( 32, &bmaskResultR, &zero, &zero, 0 );
		} while ( i == -1 && errno == EINTR );
		if ( i < 0 ) {
			Error( "error: select failed" );
		}
		
		if ( ( bmaskResultR & BITMASK( fdtcp ) ) != 0 ) {
			if ( ( len = read( fdtcp, buf, BUFSIZE ) ) <= 0 ) {
				if ( len != 0 ) {
					Log( "error: bad return from read(fdtcp)" );
				}
				quit();
			}
			for ( i = 0; i < len; ++i ) {
				if ( esc ) {
					esc = FALSE;
					if ( *(buf+i) == ODTD_BREAKCH ) {
						sendbrk( fdtty );
					}
					else if ( write( fdtty, buf + i, 1 )
						 <= 0 ) {
						Error( "error: write(tty) failed" );
					}
				}
				else if ( *(buf+i) == ODTD_ESCCH ) {
					esc = TRUE;
				}
				else if ( write( fdtty, buf + i, 1) <= 0 ) {
					Error( "error: write(tty) failed" );
				}
			}
		}
		if ( ( bmaskResultR & BITMASK( fdtty ) ) != 0 ) {
			if ( ( len = read( fdtty, buf, BUFSIZE) ) <= 0 ) {
				if ( len != 0 )  {
					Log( "error: bad return from read(fdtty)" );
				}
				quit();
			}
			if ( ( len = write( fdtcp, buf, len ) ) <= 0 )  {
				Error( "error: bad return from write(fdtcp)" );
			}
		}
	}
}

/*---------------------------------------------------------------------------
 * Quit - handle signal
 *---------------------------------------------------------------------------
 */
Quit( msg )
     int msg;
{
	int i;
	
	switch ( msg ) {
	      case SIGALRM:
		Error( "error: timeout after %d secs", SECS_TO_CONNECT );

	      default:
		quit();
      }
}

/*---------------------------------------------------------------------------
 * quit - 
 *---------------------------------------------------------------------------
 */
quit()
{
	ioctl( fileno( stdin ), TIOCSETP, &Lttymode );
	exit( 0 );
}

/*---------------------------------------------------------------------------
 * sendbrk 
 *---------------------------------------------------------------------------
 */
static 
sendbrk( fdtty )
     int fdtty;
{
#if defined(HPPA)
	ioctl( fdtty, TCSBRK, NULL );
#endif
#if defined(SUN) || defined(DEC) || defined(VAX) || defined(SEQ)
	ioctl( fdtty, TIOCSBRK, NULL );
	sleep( 1 );
	ioctl( fdtty, TIOCCBRK, NULL );
#endif
}

static int
lookupBaudRate( baudName )
     char *baudName;
{
	int i;
	
	for( i = 0; i < NBAUD; i++ ) {
		if ( strequ( baudName, baudlist[i].name ) ) {
			return( i );
		}
	}
	return( NOBAUD );
}

static int
baudRate( index )
     int index;
{
	return( baudlist[ index ].rate );
}
