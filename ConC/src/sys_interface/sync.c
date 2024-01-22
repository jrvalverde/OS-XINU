/* sync.c - sync */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>

sync()
{
    struct sys_hdr sys_hdr;

    if (!is_serv_init)
	return(sys_sync());

    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_sync;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;

    proctab[currpid].sys_command = SYS_sync;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    ssignal(sem_sys_write);

    swait(proctab[currpid].sys_sem);

    proctab[currpid].sys_command = SYS_null;

    return(proctab[currpid].sys_ret_val);
}
