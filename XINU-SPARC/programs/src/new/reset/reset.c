/* reset the xinu console device */

#include <tty.h>


/*
 * Reset The Xinu Console Device
 */
main()
{
	xcontrol(0, TCMODEC);
	xcontrol(0, TCECHO);
}
