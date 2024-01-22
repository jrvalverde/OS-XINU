/* read.c - read */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>
#include <errno.h>

read(d,buf,nbytes)
int d;
char *buf;
int nbytes;
{
    struct sys_hdr sys_hdr;

    if (!is_serv_init)
	return(sys_read(check_fd(d),buf,nbytes));

    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_read;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;
    sys_hdr.sys_val_len[0] 	= d;
    sys_hdr.sys_val_len[1] 	= nbytes;

    proctab[currpid].sys_command  = SYS_read;
    proctab[currpid].psys_args[0] = (long) buf;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    ssignal(sem_sys_write);

    swait(proctab[currpid].sys_sem);

    proctab[currpid].sys_command = SYS_null;

    return(proctab[currpid].sys_ret_val);
}
