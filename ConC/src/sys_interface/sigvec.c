/* sigvec.c - sigvec */

#include <kernel.h>
#include <errno.h>
#include <signal.h>

sigvec(sig, vec, ovec)
int sig;
struct sigvec *vec, *ovec;
{
    if (isbadsig(sig)) {
	errno = CEILLSIG;
	return(SYSERR);
	}

    return(sys_sigvec(sig, vec, ovec));
}
