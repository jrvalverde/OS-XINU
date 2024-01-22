
/* 
 * downloadd.h - Header file for downloader deamon
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Feb 25 1988
 * Copyright (c) 1988 Jim Griffioen
 */


#define DOWNLOADDIR	"/tftpboot/"	/* default download directory	*/

#define GENERIC_ERR	1		/* generic error constant	*/
#define GENERIC_OK	0		/* generic ok constant		*/

#define BIGBUFSIZE 2000		        /* size of a large buffer       */

#define SMALLSTR	32		/* generic small string length	*/
#define MEDSTR		128		/* generic medium str length	*/
#define LONGSTR		512		/* generic long str length	*/
#define XLONGSTR	1024		/* generic xtra-long str length	*/
#define XSTIMEOUT	1		/* x-small time-out period	*/
#define STIMEOUT	10		/* small time-out period	*/
#define MTIMEOUT	60		/* medium time-out period	*/
#define LTIMEOUT	120		/* long time-out period		*/
#define XLTIMEOUT	240		/* x-long time-out period	*/
#define S_MAXTRIES	2		/* small maximum number retries	*/
#define M_MAXTRIES	5		/* medium number of retries	*/
#define L_MAXTRIES	10		/* large number of retries	*/
#define S_PAUSE_TIM	1		/* small pause between retries	*/
#define M_PAUSE_TIM	2		/* medium pause between retries	*/
#define L_PAUSE_TIM	5		/* large pause between retries	*/

#define ACK		"Y"		/* positive acknowledgement	*/
#define NACK		"N"		/* negative acknowledgement	*/
#define EACK		"X"		/* error acknowledgement	*/

char *sockgetstr();		        /* get a string from a socket   */
