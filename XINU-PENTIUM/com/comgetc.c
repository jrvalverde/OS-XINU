
#include <conf.h>
#include <kernel.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  comgetc -- read a character from a serial line
 *------------------------------------------------------------------------
 */
comgetc(pdev )
struct	devsw	*pdev;
{
	kprintf("COMGETC:\n");
}

/*------------------------------------------------------------------------
 *  comread -- read from a serial line
 *------------------------------------------------------------------------
 */
comread(pdev, buf, count)
struct	devsw	*pdev;
char *buf;
int count;
{
	kprintf("COMREAD:\n");
}
