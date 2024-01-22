/*------------------------------------------------------------------------
 *	panic  --  panic and abort XINU
 *------------------------------------------------------------------------
 */
#include <conf.h>
#include <kernel.h>
#include <proc.h>

panic (msg,a1,a2,a3,a4,a5)
char *msg;
{
        STATWORD ps;

        disable(ps);
        kprintf("\n\nPanic: ");
        kprintf(msg,a1,a2,a3,a4,a5);
        kprintf("\n");
        ret_mon();
        restore(ps);
}
