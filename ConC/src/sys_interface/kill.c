/* kill.c - kill */

#include <kernel.h>
#include <errno.h>
#include <signal.h>

kill(pid, sig)
int pid, sig;
{
    if (isbadsig(sig)) {
	errno = CEILLSIG;
	return(SYSERR);
	}

    return(sys_kill(pid,sig));
}
