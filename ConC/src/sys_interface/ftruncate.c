/* ftruncate.c - ftruncate */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>
#include <errno.h>

ftruncate(fd,length)
int fd, length;
{
    struct sys_hdr sys_hdr;

    if (!is_serv_init)
	return(sys_ftruncate(check_fd(fd),length));

    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_ftruncate;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;
    sys_hdr.sys_val_len[0] 	= fd;
    sys_hdr.sys_val_len[1] 	= length;

    proctab[currpid].sys_command = SYS_ftruncate;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    ssignal(sem_sys_write);

    swait(proctab[currpid].sys_sem);

    proctab[currpid].sys_command = SYS_null;

    return(proctab[currpid].sys_ret_val);
}
