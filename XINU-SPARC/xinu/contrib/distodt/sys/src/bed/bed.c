/* 
 * bed.c - BED (back end daemon) process for distributed odt/download
 * 
 * Author:	Jim Griffioen/Shawn Oosterman
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun  9 10:30:50 1988
 *
 * Copyright (c) 1988 Jim Griffioen/Shawn Oosterman
 */

#include <stdio.h>
#include <signal.h>

#include <errno.h>
#include <sys/wait.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <pwd.h>

#include "bed.h"
#include "utils.h"
#include "bmutils.h"
#include "netutils.h"
#include "bedata.h"

#ifdef PURDUE
struct passwd *getpwuid();			/* get password entry	*/
#endif

static int sock;				/* bed server socket	*/
static struct bedata *backends;			/* bedata data structure*/
static char machinename[ MAXMACHINENAME ];	/* name of this machine	*/

struct option opt;				/* command line options	*/
int errno;

/*---------------------------------------------------------------------------
 * main (csb, rgsb) -- fork process in background and wait for requests.
 *---------------------------------------------------------------------------
 */
main ( csb, rgsb )
     int csb;
     char *rgsb[];
{
	struct sockaddr_in sa;	/* sock addr of incoming dgram	*/
	int lsaT = lsa;		/* size of sa			*/
	struct bm bm;		/* buffer for incoming datagram	*/
	union wait status;	/* wait status			*/
	int pid;		/* pid returned from wait	*/
	char directory[ 256 ];	/* directory that must exits	*/
	int i;			/* counter variable		*/
	int startovercount;	/* number of times restarted	*/
#ifdef PURDUE
	struct passwd *pp;	/* password entry		*/
#endif
	char machines_file[ MAXFILENAME ];
	
	
#ifdef PURDUE
	pp = getpwuid( geteuid() );
	if ( pp == 0 || strcmp( pp->pw_name, "root" ) ) {
		fprintf( stderr,
			"bed: Intruder Alert must be 'root' to run bed\n" );
		opt.logtype = ALOGFILE;
		Log( "bed: Intruder Alert" );
		if ( pp ) {
			fprintf( stderr, " bed: Intruder by (%s)\n",
				pp->pw_name );
		}
		exit( 1 );
	}
#endif
	
	opt.verbose = FALSE;
	opt.logtype = ALOGFILE;
	
	strcpy( machines_file, MACHINES_FILE );
	for ( i = 1; i < csb; i++ ) {
		if ( strcmp( rgsb[ i ], "-v" ) == 0 ) {
			opt.verbose = TRUE;
		}
		else if ( strcmp( rgsb[ i ], " -lf " ) == 0 ) {
			opt.logtype = ALOGFILE;
		}
		else if ( strcmp( rgsb[ i ], "-lt" ) == 0 ) {
			opt.logtype = TERMINAL;
		}
		else if ( strcmp( rgsb[ i ], "-m" ) == 0 ) {
			if ( i == ( csb - 1 ) ) {
				fprintf( stderr, "missing machine name\n" );
				exit( 1 );
			}
			strcpy( machines_file, rgsb[ i++ ] );
		}
		else if ( strncmp( rgsb[ i ], "-m", 2 ) == 0 ) {
			strcpy( machines_file, rgsb[ i ] + 2 );
		}
		else {
			fprintf( stderr, "unexpected argument '%s'\n",
				rgsb[ i ] );
			exit( 1 );
		}
	}
	
	/* make sure all the necessary directories and files are there */
	strcpy( directory, BASEDIR );
	if ( *directory == '\0' ) {
		fprintf( stderr, "directory is NULL string\n" );
		exit( 1 );
	}
	if ( directory[ strlen( directory ) - 1] == '/' ) {
		directory[ strlen( directory ) - 1 ] = '\0';
	}
	if ( access( directory, F_OK ) != 0 ) {
		if ( mkdir( directory, 0777 ) != 0 ) {
			fprintf( stderr, "directory '%s' not found\n",
				directory );
			exit( 1 );
		}
	}
	if ( chmod( directory, 0777 ) ) {
		fprintf( stderr, "directory '%s' chmod failed\n", directory );
	}
	if ( access( MACHINES_FILE, F_OK ) != 0 ) {
		fprintf( stderr, "configuration file '%s' missing\n",
			MACHINES_FILE );
		exit( 1 );
	}
	Log( "BED: Configuration file exists" );
	
#ifdef BACKGROUND
	if ( fork() > 0 ) {
		exit( 0 );		/* grandparent exits		*/
	}
	
	if ( opt.logtype == ALOGFILE ) {
		/* disassociate the server from the invoker's terminal	*/
		/* errors must be logged via syslog(--) or Log() from now on */
		for ( i = 0; i < 3; i++ ) {
			close( i );
		}
		open( "/", O_RDONLY );
		dup2( 0, 1 );
		dup2( 0, 2 );

#ifndef HPPA
		
		i = open( "/dev/tty", O_RDWR );
		if ( i >= 0 ) {
			ioctl( i, TIOCNOTTY, 0 );
			close( i );
		}

#endif
		
	}
#endif
	
	startovercount = 0;
restart:
	if ( fork () > 0 ) {		/* parent monitors child and	*/
		pid = wait( &status );	/* restarts the child (bed) if	*/
		/* it dies for some reason	*/
		if ( status.w_T.w_Retcode == REALLYDIE ) {
			Error( "bed killed by 'bedkill' command" );
		}
		if ( status.w_T.w_Retcode == RESTART ) { 
			/* restart from scratch */
			execv( BED_EXEC_PATH, rgsb );
			Error( "error: execl(bed) failed" );
		}
		if ( ++startovercount > MAXRESTARTS ) {
			Error( "bed restarted too many times" );
		}
		Log( "bed died: I (parent BED) automatically restarting it" );
		goto restart;		/* it dies for some reason	*/
	}
	Log( "bed restarted" );	
	if ( ( sock = ListenUdp( KNOWN_UDP_PORT ) ) < 0 ) {
		Error( "cannot open well known port" );
	}
	if ( gethostname( machinename, MAXMACHINENAME ) != 0 ) {
		Log( "error getting host name" );
		strcpy( machinename, "unknown" );
	}
	
	VERBOSE(Log( "reading machines database...%s", MACHINES_FILE) );
	backends = readbedata( machines_file );
	for (;;) {
		VERBOSE( Log("waiting for request") );
		if ( bmrecvfrom( sock, &bm, lbm, 0, &sa, &lsaT ) == lbm ){
			deadchild();
			ProcessRequest( &sa, &bm );
		}
		else {
			deadchild();
			BedReply( &sa, RESP_ERR,
				 "wrong size request structure" );
		}
	}
}


/*---------------------------------------------------------------------------
 * ProcessRequest (psa, pbm) -- process the network request
 *---------------------------------------------------------------------------
 */
static 
ProcessRequest ( psa, pbm )
     struct sockaddr_in *psa;
     struct bm *pbm;
{
	char *sb;
	char msg[ MAXDETAIL ];
	struct lockreq *lreq;
	struct genericreq *req;
	struct bedata *beconfig;
	int cmd;
	
	if ( sb = IsBogus( pbm ) ) {
		strcpy( msg, sb );
		BedReply( psa, RESP_ERR, msg );
		return;
	}
	switch ( cmd = atoi( pbm->cmd ) ) {
	      case REQ_QUIT:
		VERBOSE(Log( "received a QUIT request" ));
		BedReply( psa, RESP_OK, machinename );
		Quit();
		break;
		
	      case REQ_REBOOT:
		VERBOSE(Log( "received a REBOOT request" ));
		BedReply ( psa, RESP_OK, "rebooting" );
		Reboot();
		break;
		
	      case REQ_STATUS:
		VERBOSE(Log( "received a STATUS request" ));
		makestatus( (struct statresp *) msg );
		BedReply( psa, RESP_OK, msg );
		break;
		
	      case REQ_ODT_CONNECT:
	      case REQ_DNLD_CONNECT:
		VERBOSE(Log( "received a CONNECT request" ));
		req = (struct genericreq *) pbm->detail;
		beconfig = getbeconfig( backends, req->bename );
		if ( beconfig == NULL ) {
			BedReply( psa, RESP_ERR, "illegal back-end name" );
			break;
		}
		if ( checkuid( cmd, beconfig, req->uid, msg ) == SYSERR ) {
			BedReply( psa, RESP_ERR, msg );
			break;
		}
		if ( cmd == REQ_ODT_CONNECT ) {
			VERBOSE(Log( "received an 'odt connect' request" ));
			beconfig->lock.pidodt = handle_connect( beconfig,
							 psa, req, cmd );
		}
		else {
			VERBOSE(Log( "received a 'download connect' request"));
			beconfig->lock.piddown = handle_connect( beconfig,
							psa, req, cmd );
		}
		break;
		
	      case REQ_LOCK:
		VERBOSE(Log( "received a LOCK request" ));
		lreq = (struct lockreq *) pbm->detail;
		beconfig = getbeconfig( backends, lreq->bename );
		if ( beconfig == NULL ) {
			BedReply( psa, RESP_ERR, "illegal back-end name" );
			break;
		}
		if ( getlock( beconfig, lreq->uid, lreq->hostid ) == SYSOK ){
			BedReply( psa, RESP_OK, (char *)lreq );
		}
		else {
			BedReply( psa, RESP_LOCK_REFUSED, (char *)lreq );
		}
		break;
		
	      case REQ_UNLOCK:
		VERBOSE(Log( "received a UNLOCK request" ));
		lreq = (struct lockreq *) pbm->detail;
		beconfig = getbeconfig( backends,  lreq->bename );
		if ( beconfig == NULL ) {
			BedReply( psa, RESP_ERR, "illegal back-end name" );
			break;
		}
		if ( unlock( beconfig, lreq->uid, lreq->hostid ) == SYSOK ) {
			BedReply( psa, RESP_OK, (char *)lreq );
		}
		else {
			BedReply( psa, RESP_ERR, (char *)lreq );
		}
		break;
		
	      default:
		VERBOSE(Log( "received an INVALID request" ));
		BedReply( psa, RESP_ERR, "invalid command" );
		break;
	}
	return( 1 );
}

/*---------------------------------------------------------------------------
 * Quit - kill backend daemon
 *---------------------------------------------------------------------------
 */
static 
Quit()
{
	CleanUp();
	exit( REALLYDIE );			/* tell monitoring process	*/
	/* not to restart the bed	*/
}

/*---------------------------------------------------------------------------
 * makestatus - get status and fill in status message contents
 *---------------------------------------------------------------------------
 */
static
makestatus( stats )
     struct statresp *stats;
{
	int i;				/* counter variable		*/
	struct bedata *mdata;		/* machines/odt data structure	*/
	double ldaver[ 5 ];		/* load averages		*/
	time_t time, getidletime(); 
	
	strcpy( stats->festatus.fename, machinename );
	sprintf( stats->festatus.numusers, "%d", getnumusers() );
	getloadaver( ldaver );
	sprintf( stats->festatus.loadav, "%5.2f %5.2f %5.2f",
		ldaver[ 0 ], ldaver[ 1 ], ldaver[ 2 ] );
	for (mdata = backends, i=0; mdata != NULL; mdata = mdata->next, i++){
		strcpy( stats->bestatus[ i].bename, mdata->data.bename );
		strcpy( stats->bestatus[ i].ttyname, mdata->data.ttyname );
		strcpy( stats->bestatus[ i].class, mdata->data.beclass );
		time = getidletime( mdata );
		settime( time, stats->bestatus[ i ].idle );
		strcpy( stats->bestatus[ i ].user, mdata->lock.user );
	}
	sprintf( stats->festatus.numbends, "%d", i );
}


/*---------------------------------------------------------------------------
 * Reboot - restart the backend daemon
 *---------------------------------------------------------------------------
 */
static 
Reboot()
{
	CleanUp();
	exit( RESTART );	/* parent will restart the BED	*/
}

/*---------------------------------------------------------------------------
 * Cleanup - Clean things up a bit before exiting
 *---------------------------------------------------------------------------
 */
static 
CleanUp()
{
	struct bedata *machine;
	
	close( sock );
	for( machine = backends; machine != NULL; machine = machine->next ) {
		if ( machine->lock.locked ) {
			if ( machine->lock.pidodt != BADPID ) {
				kill( machine->lock.pidodt, SIGKILL );
			}
			if( machine->lock.piddown != BADPID ) {
				kill( machine->lock.piddown, SIGKILL );
			}
		}
	}
	Log( "Bed process exiting" );
}


/*---------------------------------------------------------------------------
 * deadchild - collect status of any children that might have fininshed
 *	       recently
 *---------------------------------------------------------------------------
 */
static 
deadchild()
{
	struct rusage rusage;
	int childpid = 1;
	int waitoptions = WNOHANG;
	struct bedata *machine;
	int i;
	union wait status;
	
	
	while ( ( childpid != 0 ) && ( childpid != -1 ) ) {
		childpid = wait3( &status, waitoptions, &rusage );
		if ( childpid > 0 ) {
			VERBOSE(Log( "child process %d has died ", childpid));
			for( machine = backends; machine != NULL;
			    machine = machine->next) {
				
				if ( machine->lock.locked ) {
					if ( machine->lock.pidodt == childpid){
						machine->lock.pidodt = BADPID;
					}
					if (machine->lock.piddown == childpid){
						machine->lock.piddown = BADPID;
					}
				}
			}
		}
	}
}

/*---------------------------------------------------------------------------
 * BedReply -- build and send bm
 *---------------------------------------------------------------------------
 */
static 
BedReply( psa, resp, detail )
     struct sockaddr_in *psa;
     int resp;
     char *detail;
{
	struct bm bm;
	
	bm.ver = CURVER;
	sprintf( bm.cmd, "%d", resp );
	bcopy( detail, bm.detail, MAXDETAIL );
	
	if ( bmsendto( sock, &bm, lbm, 0, psa, lsa ) <= 0 ) {
		Error( "error: bmsendto failed" );
	}
}


/*---------------------------------------------------------------------------
 * handle_connect - handle connection
 *---------------------------------------------------------------------------
 */
static int
handle_connect( beconfig, psa, req, cmd )
     struct bedata *beconfig;
     struct sockaddr_in *psa;
     struct genericreq * req;
     int cmd;
{
	u_short localport = 0;
	int fdTcp;
	char fdTcp_str[ 10 ];
	char msg[ MAXDETAIL ];
	int pid;
	char *argv[ NUM_BED_DATA + 3 ], **ptr;
	int i;
	
	VERBOSE(Log( "Fork and handle 'connect' request" ));
	if ( ( pid = fork() ) == 0 ) {
		fdTcp = ListenTcp( &localport );
		sprintf( msg, "%d : local TCP port", localport );
		BedReply( psa, RESP_OK, msg );
		close( sock );
		nice( CHILD_PRIO );
		
		sprintf( fdTcp_str, "%d", fdTcp );
		argv[ 1 ] = fdTcp_str;
		
		ptr = (char **) &( beconfig->data );
		for(  i = 0; i < NUM_BED_DATA; i++, ptr++ ) {
			argv[ i + 2] = *ptr;
		}
		argv[ i+2 ] = NULL;
		
		if ( cmd == REQ_ODT_CONNECT ) {
			argv[ 0 ] = beconfig->data.odtdpath;
			execv( argv[ 0 ], argv );
			Error( "error: execv(odtd) failed" );
		}
		else {
			argv[ 0 ] = beconfig->data.dloaddpath;
			execv( argv[ 0 ], argv );
			Error( "error: execv(downloadd) failed" );
		}
	}
	VERBOSE(Log( "Fork pid = %d", pid ));
	return( pid );
}

/*---------------------------------------------------------------------------
 * quit - 
 *---------------------------------------------------------------------------
 */
quit()
{
	CleanUp();
	exit( 0 );		/* tell monitoring process	*/
	/* not to restart the Bed	*/
}

