/* user.c - main */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <user.h>
#include <proc.h>

struct user utab[] = {
	{ 0, "root", "Xinu Root", 0 },
	{ 3954, "dls", "David L Stevens", 0 },
	{ 2, "dds", "Dave (Go HAWKS) Starkey", 0 },
	{ 316, "lin", "John C. Lin", 0 },
	{ 1, "comer", "Douglas E. Comer", 0 },
	{ -1, 0, 0, 0 }
};

/*-------------------------------------------------------------------------
 * user1 - 
 *-------------------------------------------------------------------------
 */
user1()
{
    if (open(TTY0, SERIAL0) == SYSERR)
	kprintf("open(tty0, serial0) failed\n");

    while(TRUE) {
	shell(TTY0);
    }
}

/*------------------------------------------------------------------------
 *  main  --  user main program
 *------------------------------------------------------------------------
 */
main()
{
    extern int console_dev;

/*    resume(create(user1, 8192, INITPRIO, "tty1", 0));*/

    
    open(CONSOLE, console_dev);

    while(TRUE) {
	shell(CONSOLE);
    }
}
