/* umask.c - umask */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>

umask(numask)
int numask;
{
    struct sys_hdr sys_hdr;

    if (!is_serv_init)
	return(sys_umask(numask));

    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_umask;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;
    sys_hdr.sys_val_len[0] 	= numask;

    proctab[currpid].sys_command = SYS_umask;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    ssignal(sem_sys_write);

    swait(proctab[currpid].sys_sem);

    proctab[currpid].sys_command = SYS_null;

    return(proctab[currpid].sys_ret_val);
}
