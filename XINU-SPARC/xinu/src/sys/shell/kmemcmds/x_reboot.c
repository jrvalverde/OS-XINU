/* x_reboot.c - x_reboot */

#include <conf.h>
#include <kernel.h>
#include <vmem.h>

/*------------------------------------------------------------------------
 *  x_reboot  -  (builtin command reboot) restart the system from scratch
 *		MACHINE DEPENDENT
 *------------------------------------------------------------------------
 */
BUILTIN	x_reboot(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
    reboot_machine();
}
