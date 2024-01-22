/* fstat.c - fstat */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

fstat(fd,pbuf)
int fd;
struct stat *pbuf;
{
    struct sys_hdr sys_hdr;

    if (!is_serv_init)
	return(sys_fstat(check_fd(fd),pbuf));

    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_fstat;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;
    sys_hdr.sys_val_len[0] 	= fd;
    sys_hdr.sys_val_len[1] 	= sizeof(struct stat);

    proctab[currpid].sys_command = SYS_fstat;
    proctab[currpid].psys_args[0] = (long) pbuf;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    ssignal(sem_sys_write);

    swait(proctab[currpid].sys_sem);

    proctab[currpid].sys_command = SYS_null;

    return(proctab[currpid].sys_ret_val);
}
