/* close.c - close */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>
#include <errno.h>

close(d)
int d;
{
    int		   ret_val;
    struct sys_hdr sys_hdr;

    if (!is_serv_init)
	return(sys_close(check_fd(d)));

    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_close;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;
    sys_hdr.sys_val_len[0] 	= d;

    proctab[currpid].sys_command = SYS_close;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    ssignal(sem_sys_write);

    swait(proctab[currpid].sys_sem);

    proctab[currpid].sys_command = SYS_null;

    return(proctab[currpid].sys_ret_val);
}
