/* killpg.c - killpg */

#include <kernel.h>
#include <errno.h>
#include <signal.h>

killpg(pgrp, sig)
int pgrp, sig;
{
    if (isbadsig(sig)) {
	errno = CEILLSIG;
	return(SYSERR);
	}

    return(sys_killpg(pgrp, sig));
}
