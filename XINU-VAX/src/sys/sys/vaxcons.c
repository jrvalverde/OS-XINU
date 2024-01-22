/* vaxcons.c -- vaxcons */

#include <conf.h>
#include <procreg.h>

#define ALERTCONS	0xf05

/*----------------------------------------------------------------------------
 * vaxcons -- awake VAX console system for console commands -- Xinu suspends
 *	      (use with caution)
 *----------------------------------------------------------------------------
*/

vaxcons()
{
#ifndef VAX780
	mtpr(ALERTCONS, TXDB);	/* tell VAX to run console command system */
#endif
}
