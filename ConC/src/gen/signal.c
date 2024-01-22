/****************************************
 *					*
 *    signal.c				*
 *    Modified for mutual exclusion	*
 *					*
 *    Local static vars mask and flags	*
 *    moved to global statics so both	*
 *    routines can access them.		*
 *					*
 *    sys_signal kept here for fun.	*
 *    It is a different case from SYS_.	*
 *					*
 ****************************************/
#include <kernel.h>
#undef NULL

/*
 * Copyright (c) 1985 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)signal.c	5.2 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

/*
 * Almost backwards compatible signal.
 */
#include <signal.h>

static int mask[NSIG];
static int flags[NSIG];

int (*
signal(s, a))()
	int s, (*a)();
{
	struct sigvec osv, sv;

    swait(sem_sys_gen);
	sv.sv_handler = a;
	sv.sv_mask = mask[s];
	sv.sv_flags = flags[s];
	if (sigvec(s, &sv, &osv) < 0)
		{ssignal(sem_sys_gen); return (BADSIG);}
	if (sv.sv_mask != osv.sv_mask || sv.sv_flags != osv.sv_flags) {
		mask[s] = sv.sv_mask = osv.sv_mask;
		flags[s] = sv.sv_flags = osv.sv_flags;
		if (sigvec(s, &sv, 0) < 0)
			{ssignal(sem_sys_gen); return (BADSIG);}
	}
    ssignal(sem_sys_gen);
	return (osv.sv_handler);
}

int (*
sys_signal(s, a))()
	int s, (*a)();
{
	struct sigvec osv, sv;

    swait(sem_sys_gen);
	sv.sv_handler = a;
	sv.sv_mask = mask[s];
	sv.sv_flags = flags[s];
	if (sys_sigvec(s, &sv, &osv) < 0)
		{ssignal(sem_sys_gen); return (BADSIG);}
	if (sv.sv_mask != osv.sv_mask || sv.sv_flags != osv.sv_flags) {
		mask[s] = sv.sv_mask = osv.sv_mask;
		flags[s] = sv.sv_flags = osv.sv_flags;
		if (sys_sigvec(s, &sv, 0) < 0)
			{ssignal(sem_sys_gen); return (BADSIG);}
	}
    ssignal(sem_sys_gen);
	return (osv.sv_handler);
}
