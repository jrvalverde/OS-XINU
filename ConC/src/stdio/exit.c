/****************************************/
/*					*/
/*	Modified from original source	*/
/*	to do _cleanup correctly.	*/
/*					*/
/****************************************/

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)exit.c	5.2 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

/*	exit.c	1.1	83/06/23	*/

#include <kernel.h>
#undef NULL
#include <syscall.h>
#include <proc.h>
#include <stdio.h>
#include <sem.h>

exit(code)
int code;
{
    struct sys_hdr sys_hdr;
    long ps;

    disable(ps);

    _cleanup();			/* This bypasses any stdio semaphores */

    if (!is_serv_init) {
	_exit(code);
	}

    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_exit;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= 0;
    sys_hdr.sys_val_len[0] 	= code;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));

    _exit(code);
}
