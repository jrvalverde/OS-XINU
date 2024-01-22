/* test.c - main */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>
#include <vmem.h>
#include <xpppgio.h>
#include <tp.h>
#include <mem.h>

/*------------------------------------------------------------------------
 *  main  -- let the XINU magic start! 
 *------------------------------------------------------------------------
 */
main()
{
    while (TRUE) {
	login(CONSOLE);
	shell(CONSOLE);
    }
}
