/********************************
 *				*
 *   usleep.c			*
 *   Not allowed in ConcurrenC	*
 *   (ConcurrenC works in	*
 *   tenths of seconds -- 	*
 *   see SLEEP(2CCC)).		*
 *				*
 ********************************/

#include <kernel.h>
#undef NULL
#include <errno.h>

usleep(n)
	unsigned n;
{
    errno = CENOTALLOWED;
    return(SYSERR);
}
