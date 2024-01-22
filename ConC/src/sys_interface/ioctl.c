/* ioctl.c - ioctl */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>
#include <errno.h>
#include <C_ioctl.h>
#include <sys/ioctl.h>

ioctl(d,request,pbuf)
int d, request;
char *pbuf;
{
    struct sys_hdr sys_hdr;

    if (!is_serv_init)
	return(sys_ioctl(check_fd(d),request,pbuf));

    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_ioctl;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;
    sys_hdr.sys_val_len[0] 	= d;
    sys_hdr.sys_val_len[1] 	= request;
    sys_hdr.sys_val_len[2] 	= IOC_BUF_SIZE(request);

    proctab[currpid].sys_command = SYS_ioctl;
    proctab[currpid].psys_args[0] = (long) pbuf;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
	sys_write(SYS_PIPE,pbuf,sys_hdr.sys_val_len[2]);
    ssignal(sem_sys_write);

    swait(proctab[currpid].sys_sem);

    proctab[currpid].sys_command = SYS_null;

    return(proctab[currpid].sys_ret_val);
}
