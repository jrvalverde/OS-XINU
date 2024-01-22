/* lseek.c - lseek */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>
#include <errno.h>

lseek(d,offset,whence)
int d, offset, whence;
{
    struct sys_hdr sys_hdr;

    if (!is_serv_init)
	return(sys_lseek(check_fd(d),offset,whence));

    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_lseek;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;
    sys_hdr.sys_val_len[0] 	= d;
    sys_hdr.sys_val_len[1] 	= offset;
    sys_hdr.sys_val_len[2] 	= whence;

    proctab[currpid].sys_command = SYS_lseek;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    ssignal(sem_sys_write);

    swait(proctab[currpid].sys_sem);

    proctab[currpid].sys_command = SYS_null;

    return(proctab[currpid].sys_ret_val);
}
